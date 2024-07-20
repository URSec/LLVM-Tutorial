//===- LLVMTutorial.h - Skeleton code for LLVM Tutorial ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides skeleton code for a transformation that adds calls to
// every memory access operation and every branch.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_UTILS_LLVMTUTORIAL_H
#define LLVM_TRANSFORMS_UTILS_LLVMTUTORIAL_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Instructions.h"

namespace llvm {

class LLVMTutorialPass : public PassInfoMixin<LLVMTutorialPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);

  StringRef getPassName() const { return "LLVM Tutorial Pass"; }

private:
  // Declare a callee that will check memory.
  FunctionCallee checkMemoryFn;

  // Methods for transforming difference instructions
  void visitLoadInst(LoadInst *LI);
  void visitStoreInst(StoreInst *SI);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_LLVMTUTORIAL_H

