#include "userthread.h"
#include "thread.h"


struct threadInfo{
	int f;
	int arg;

};

int do_ThreadCreate(int f, int arg){

	struct threadInfo t;
	t.f = f;
	t.arg = arg;
	Thread *newThread = new Thread("newThread"); 
	newThread->Start(StartUserThread, &t);

}

static void StartUserThread(void *schmurtz){

	int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, t->f);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16 -256);
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   numPages * PageSize - 16);
}
