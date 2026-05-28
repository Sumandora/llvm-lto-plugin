#include <llvm/ADT/ArrayRef.h>
#include <llvm/IR/Analysis.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Pass.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Plugins/PassPlugin.h>
#include <llvm/Support/Compiler.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {

	struct DumpPass : PassInfoMixin<DumpPass> {
		PreservedAnalyses run(Module& M, ModuleAnalysisManager&)
		{
			M.print(errs(), nullptr, false, false);
			return PreservedAnalyses::all();
		}

		static bool isRequired() { return true; }
	};

}

llvm::PassPluginLibraryInfo getDumpPassPluginInfo()
{
	return {
		.APIVersion = LLVM_PLUGIN_API_VERSION,
		.PluginName = "Dump Pass",
		.PluginVersion = LLVM_VERSION_STRING,
		.RegisterPassBuilderCallbacks = [](PassBuilder& PB) {
			PB.registerPipelineParsingCallback(
				[](StringRef Name,
					ModulePassManager& MPM,
					[[maybe_unused]] ArrayRef<llvm::PassBuilder::PipelineElement> pipeline) {
					if (Name == "DumpPass") {
						MPM.addPass(DumpPass());
						return true;
					}
					return false;
				});
			PB.registerFullLinkTimeOptimizationEarlyEPCallback(
				[](ModulePassManager& MPM, OptimizationLevel) {
					MPM.addPass(DumpPass());
				});
		},
	};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
	return getDumpPassPluginInfo();
}
