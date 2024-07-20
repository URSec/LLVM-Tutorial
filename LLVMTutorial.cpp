//===- LLVMTutorial.cpp - Skeleton code for LLVM Tutorial ------------------===//
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

#include "llvm/Transforms/Utils/LLVMTutorial.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Type.h"

using namespace llvm;

#define DEBUG_TYPE "LLVMTutorial"
#define MEMORY_CHECK_FN "checkMemory"

//
// Create statistics that the opt and clang tools can report
//
STATISTIC(NumLoads,  "Number of load instructions instrumented");
STATISTIC(NumStores, "Number of store instructions instrumented");

//
// Function: createRuntimeCheckFunc()
//
// Description:
//  This function creates the declaration of the checkMemory function that
//  checks the pointer dereferenced in a load or store instruction at run-time.
//  If the checkMemory function is already declared or defined within the
//  specified Module, a pointer to it is returned.
//
// Inputs:
//  M - A reference to the Module to which the checkMemory function declaration
//      should be added.
//
// Outputs:
//  M - The Module modified (if necessary) to contain a checkMemory function
//      declaration.
//
// Return value:
//  A pointer to the checkMemory function is returned.
//
static FunctionCallee createRuntimeCheckFunc (Module & M) {
  LLVMContext &C = M.getContext();
  // Create the types needed for checkMemory's signature.
  Type *voidTy = Type::getVoidTy(C);
  // Note that LLVM does not allow creating a pointer to the void type. We need
  // to create a pointer to a 8-bit integer type, which corresponds to "void *".
  Type *voidPtrType = PointerType::getUnqual(IntegerType::get(C, 8));
  FunctionType *FnTy = FunctionType::get(voidTy,
                                         ArrayRef<Type *>(voidPtrType),
                                         false
                                         );
  return M.getOrInsertFunction(MEMORY_CHECK_FN, FnTy);
}

//
// Method: visitLoadInst()
//
// Description:
//  Add a call to a library function before every load instruction.  This
//  method is called by the InstVisitor class whenever it encounters a load
//  instruction.
//
// Inputs:
//  LI - A pointer to the load instruction to transform.
//  checkMemoryFn - A pointer to the library function that checks for memory.
//
// Outputs:
//  None.
//
// Return value:
//  None
//
void LLVMTutorialPass::visitLoadInst(LoadInst *LI) {
  Value *targetPtr = LI->getPointerOperand();
  CallInst::Create(checkMemoryFn, ArrayRef<Value *>(targetPtr), "", LI);

  ++NumLoads; 
}

//
// Method: visitStoreInst()
//
// Description:
//  Add a call to a library function before every store instruction.  This
//  method is called by the InstVisitor class whenever it encounters a store
//  instruction.
//
// Inputs:
//  SI - A pointer to the store instruction to transform.
//  checkMemoryFn - A pointer to the library function that checks for memory.
//
// Outputs:
//  None.
//
// Return value:
//  None
//
void LLVMTutorialPass::visitStoreInst(StoreInst *SI) {
  Value *targetPtr = SI->getPointerOperand();
  CallInst::Create(checkMemoryFn, ArrayRef<Value *>(targetPtr), "", SI);

  ++NumStores; 
}

PreservedAnalyses LLVMTutorialPass::run(Module &M,
                                        ModuleAnalysisManager &AM) {
                                        
  checkMemoryFn = createRuntimeCheckFunc(M);
  assert(checkMemoryFn.getCallee() && "Failed to create the prototype for checkMemoryFn");

  // Scan all of the instructions within the module to determine if we need to
  // transform them.  If so, call the appropriate visit method to transform
  // the instruction.
  for (Function &F : M) {
    errs() << "Instrumenting function " << F.getName() << "\n";
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        // If this is a load instruction, instrument it with a call to the
        // function that checks the pointer used in the load.
        if (LoadInst *LI = dyn_cast<LoadInst>(&I)) {
          visitLoadInst(LI);
        }

        // If this is a store instruction, instrument it with a call to the
        // function that checks the pointer used in the store.
        if (StoreInst *SI = dyn_cast<StoreInst>(&I)) {
          visitStoreInst(SI);
        }
      }
    }
  }

  return PreservedAnalyses::all();
}

