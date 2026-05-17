#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/AST/Decl.h"

using namespace clang;
using namespace ento;

namespace {

class MISRAC_8_11_Checker : public Checker<check::ASTDecl<VarDecl>> {
  mutable std::unique_ptr<BugType> BT;

public:
  void checkASTDecl(const VarDecl *VD, AnalysisManager &mgr,
                    BugReporter &BR) const {
    // 只检查具有外部链接的变量
    if (VD->getFormalLinkage() != ExternalLinkage)
      return;

    // 获取变量的类型
    QualType T = VD->getType();

    // 检查是否是不完整数组类型（即没有明确指定大小的数组）
    if (T->isIncompleteArrayType()) {
      if (!BT)
        BT.reset(new BugType(this, "MISRA C:2012 Rule 8.11", "MISRA C"));

      PathDiagnosticLocation Location =
          PathDiagnosticLocation::createBegin(VD, BR.getSourceManager());
      auto Report = std::make_unique<BasicBugReport>(
          *BT, "When an array with external linkage is declared, its size should be explicitly specified (Rule 8.11)", Location);
      BR.emitReport(std::move(Report));
    }
  }
};

} // end anonymous namespace

void ento::registerMISRAC_8_11(CheckerManager &mgr) {
  mgr.registerChecker<MISRAC_8_11_Checker>();
}

bool ento::shouldRegisterMISRAC_8_11(const CheckerManager &mgr) {
  return true;
}
