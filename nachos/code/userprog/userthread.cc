#include "userthread.h"
#include "thread.h"
#include "machine.h"
#include "system.h"
#include "synch.h"

static void StartUserThread(void *schmurtz);

typedef struct 
{
	int f;
	int arg;
    int stackAdresse;

}threadInfo;

Semaphore *mainStop = new Semaphore("mainStop", 1);


static int thread_count = 0;


int do_ThreadCreate(int f, int arg)
{

    if(thread_count == 0)
        mainStop->P();
    

    threadInfo *t = (threadInfo *)malloc(sizeof(*t));
	thread_count++;
    
	t->f = f;
	t->arg = arg;

	Thread *newThread = new Thread("newThread");
    newThread->space = currentThread->space;

    int bitmapIndex = currentThread->space->AllocateUserStack();
    t->stackAdresse = currentThread->space->stackTop() - bitmapIndex * 256;

    if(bitmapIndex > 0)
    {
        newThread->stackSlot = bitmapIndex;
        newThread->Start(StartUserThread, t);
    }
    else
    {
        free(t);
        delete newThread;
        interrupt->Halt();
    }
   
    return bitmapIndex;
}

static void StartUserThread(void *schmurtz)
{

    threadInfo *t = (threadInfo *)schmurtz;
	int i;


    for (i = 0; i < NumTotalRegs; i++)
	   machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, t->f);


    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the functions argument
    machine->WriteRegister(4, t->arg);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!

    
    machine->WriteRegister (StackReg, t->stackAdresse);

    DEBUG ('a', "Initializing stack register to 0x%x\n", t->stackAdresse);

    free(t);

    machine->Run();
}

void do_ThreadExit()
{
    if(thread_count == 1)
        mainStop->V();
    
    currentThread->space->stackMap->Clear(currentThread->stackSlot);
    thread_count--;
    currentThread->Finish();
}