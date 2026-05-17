#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace ento;

namespace {

class MISRAC_7_1_Checker : public Checker<check::ASTCodeBody> {
  mutable std::unique_ptr<BugType> BT;

  class WalkAST : public RecursiveASTVisitor<WalkAST> {
    const MISRAC_7_1_Checker *Checker;
    BugReporter &BR;
    AnalysisDeclContext *AC;

  public:
    WalkAST(const MISRAC_7_1_Checker *checker, BugReporter &br, AnalysisDeclContext *ac)
        : Checker(checker), BR(br), AC(ac) {}

    bool VisitIntegerLiteral(IntegerLiteral *I) {
      SourceLocation Loc = I->getLocation();
      // Skip macros to avoid false positives or complications, 
      // though true MISRA might require macro checks.
      if (Loc.isMacroID())
        return true;

      SourceManager &SM = BR.getSourceManager();
      bool Invalid = false;
      StringRef TokenText = Lexer::getSourceText(
          CharSourceRange::getTokenRange(Loc), SM, BR.getContext().getLangOpts(), &Invalid);

      if (!Invalid && TokenText.size() > 1 && TokenText.starts_with("0") && 
          !TokenText.starts_with("0x") && !TokenText.starts_with("0X") &&
          !TokenText.starts_with("0b") && !TokenText.starts_with("0B")) {
        // Contains '8' or '9' could mean it's invalid octal or something else,
        // but typically compiler catches that. We flag valid octals.
        Checker->reportBug(I, BR, AC);
      }
      return true;
    }
  };

public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &mgr,
                        BugReporter &BR) const {
    WalkAST walker(this, BR, mgr.getAnalysisDeclContext(D));
    walker.TraverseDecl(const_cast<Decl *>(D));
  }

  void reportBug(const Stmt *S, BugReporter &BR, AnalysisDeclContext *AC) const {
    if (!BT)
      BT.reset(new BugType(this, "MISRA C:2012 Rule 7.1", "MISRA C"));

    PathDiagnosticLocation Location =
        PathDiagnosticLocation::createBegin(S, BR.getSourceManager(), AC);
    auto Report = std::make_unique<BasicBugReport>(
        *BT, "Octal constants shall not be used (Rule 7.1)", Location);
    BR.emitReport(std::move(Report));
  }
};

} // end anonymous namespace

void ento::registerMISRAC_7_1(CheckerManager &mgr) {
  mgr.registerChecker<MISRAC_7_1_Checker>();
}

bool ento::shouldRegisterMISRAC_7_1(const CheckerManager &mgr) {
  return true;
}
