#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/IR/Analysis.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Pass.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

#include <functional>

using namespace llvm;

namespace {

// void visitor(Function &F) {
//   errs() << "(llvm-tutor) Hello from: " << F.getName() << "\n";
//   errs() << "(llvm-tutor)   number of arguments: " << F.arg_size() << "\n";
// }

struct HelloWorld : PassInfoMixin<HelloWorld> {
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &) {
    M.print(errs(), nullptr, false, false);
    // for (Function &f : M.functions()) {
    //   visitor(f);
    // }
    return PreservedAnalyses::all();
  }

  static bool isRequired() { return true; }
};

} // namespace

llvm::PassPluginLibraryInfo getHelloWorldPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "HelloWorld", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerFullLinkTimeOptimizationEarlyEPCallback(
                [](ModulePassManager &Mgr, OptimizationLevel Level) {
                  Mgr.addPass(HelloWorld());
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getHelloWorldPluginInfo();
}

