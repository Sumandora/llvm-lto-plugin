#include <llvm/IR/Analysis.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Pass.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/Compiler.h>
#include <llvm/Support/raw_ostream.h>

#include <functional>

using namespace llvm;

namespace {

	struct HelloWorld : PassInfoMixin<HelloWorld> {
		PreservedAnalyses run(Module& M, ModuleAnalysisManager&)
		{
			M.print(errs(), nullptr, false, false);
			return PreservedAnalyses::all();
		}

		static bool isRequired() { return true; }
	};

}

llvm::PassPluginLibraryInfo getHelloWorldPluginInfo()
{
	return { LLVM_PLUGIN_API_VERSION, "HelloWorld", LLVM_VERSION_STRING,
		[](PassBuilder& PB) {
			PB.registerFullLinkTimeOptimizationEarlyEPCallback(
				[](ModulePassManager& Mgr, OptimizationLevel) {
					Mgr.addPass(HelloWorld());
				});
		} };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
	return getHelloWorldPluginInfo();
}
