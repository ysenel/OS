#include "userthread.h"
#include "thread.h"
#include "machine.h"
#include "system.h"




typedef struct 
{
	int f;
	int arg;

}threadInfo;

Semaphore *mainStop = new Semaphore("mainStop", 1);

static int thread_count = 0;

int ff;

int do_ThreadCreate(int f, int arg)
{
    if(thread_count == 0)
        mainStop->P();
    

    threadInfo *t = (threadInfo *)malloc(sizeof(*t));
	thread_count++;
    
	t->f = f;
	t->arg = arg;

    ff = f;

	Thread *newThread = new Thread("newThread");
    newThread->space = currentThread->space;

	newThread->Start(StartUserThread, t);
   
    return 1;

}

static void StartUserThread(void *schmurtz){

    threadInfo *t = (threadInfo *)schmurtz;
	int i;


    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    //printf("init reg: %d\n", ff);
    machine->WriteRegister (PCReg, t->f);


    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);



    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!

    machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack() - 16);
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   currentThread->space->AllocateUserStack() - 16);

    free(t);

    machine->Run();

    
}

void do_ThreadExit()
{
    if(thread_count == 1)
        mainStop->V();
    

    currentThread->Finish();
    thread_count--;
    
}