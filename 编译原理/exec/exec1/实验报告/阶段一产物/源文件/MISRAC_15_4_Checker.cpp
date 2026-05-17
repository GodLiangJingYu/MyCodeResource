#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;
using namespace ento;

namespace {

class MISRAC_15_4_Checker : public Checker<check::ASTCodeBody> {
  mutable std::unique_ptr<BugType> BT;

  class LoopExitVisitor : public RecursiveASTVisitor<LoopExitVisitor> {
    int ExitCount;

  public:
    LoopExitVisitor() : ExitCount(0) {}

    int getExitCount() const { return ExitCount; }

    bool VisitBreakStmt(BreakStmt *S) {
      ExitCount++;
      return true;
    }

    bool VisitGotoStmt(GotoStmt *S) {
      ExitCount++;
      return true;
    }

    // 为了避免将嵌套循环和嵌套 switch 内部的 break/goto 算作外层循环的退出点，
    // 这里我们截断遍历，不进入嵌套的循环或 switch。
    // 注意：如果是 goto 跳出内层循环直接结束外层，理论上也应该算外层退出，
    // 但根据 AST 的层级关系，简单处理就是拦截内部的遍历。
    bool TraverseForStmt(ForStmt *S) { return true; }
    bool TraverseWhileStmt(WhileStmt *S) { return true; }
    bool TraverseDoStmt(DoStmt *S) { return true; }
    bool TraverseSwitchStmt(SwitchStmt *S) { return true; }
  };

  class MainVisitor : public RecursiveASTVisitor<MainVisitor> {
    const MISRAC_15_4_Checker *Checker;
    BugReporter &BR;
    AnalysisDeclContext *AC;

    void checkLoop(const Stmt *LoopBody, const Stmt *LoopNode) {
      if (!LoopBody) return;
      LoopExitVisitor ExitCounter;
      ExitCounter.TraverseStmt(const_cast<Stmt *>(LoopBody));

      if (ExitCounter.getExitCount() > 1) {
        Checker->reportBug(LoopNode, BR, AC);
      }
    }

  public:
    MainVisitor(const MISRAC_15_4_Checker *checker, BugReporter &br, AnalysisDeclContext *ac)
        : Checker(checker), BR(br), AC(ac) {}

    bool VisitForStmt(ForStmt *S) {
      checkLoop(S->getBody(), S);
      return true;
    }

    bool VisitWhileStmt(WhileStmt *S) {
      checkLoop(S->getBody(), S);
      return true;
    }

    bool VisitDoStmt(DoStmt *S) {
      checkLoop(S->getBody(), S);
      return true;
    }
  };

public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &mgr,
                        BugReporter &BR) const {
    MainVisitor visitor(this, BR, mgr.getAnalysisDeclContext(D));
    visitor.TraverseDecl(const_cast<Decl *>(D));
  }

  void reportBug(const Stmt *S, BugReporter &BR, AnalysisDeclContext *AC) const {
    if (!BT)
      BT.reset(new BugType(this, "MISRA C:2012 Rule 15.4", "MISRA C"));

    PathDiagnosticLocation Location =
        PathDiagnosticLocation::createBegin(S, BR.getSourceManager(), AC);
    auto Report = std::make_unique<BasicBugReport>(
        *BT, "There should be no more than one break or goto statement used to terminate any iteration statement (Rule 15.4)", Location);
    BR.emitReport(std::move(Report));
  }
};

} // end anonymous namespace

void ento::registerMISRAC_15_4(CheckerManager &mgr) {
  mgr.registerChecker<MISRAC_15_4_Checker>();
}

bool ento::shouldRegisterMISRAC_15_4(const CheckerManager &mgr) {
  return true;
}
