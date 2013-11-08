#include "userthread.h"
#include "thread.h"
#include "machine.h"
#include "system.h"
#include "synch.h"

static void StartUserThread(void *schmurtz);


/* Structure used to pass parameters to StartUserThread. */
typedef struct 
{
	int f; /* the function */
	int arg; /* its parameters */
    int stackAdresse; /* it's stack top */

}threadInfo;


/* Semaphore to stop the main thread from finishing before the user threads. */
Semaphore *mainStop = new Semaphore("mainStop", 1);

static int thread_count = 0;


int do_ThreadCreate(int f, int arg)
{
    /* if another thread is created from the main one. */
    if(thread_count == 0)
        mainStop->P();
    

    threadInfo *t = (threadInfo *)malloc(sizeof(*t));
	thread_count++;
    
	t->f = f;
	t->arg = arg;

	Thread *newThread = new Thread("newThread");

    /* Every thread will share an adresse space. */
    newThread->space = currentThread->space;

    int bitmapIndex = currentThread->space->AllocateUserStack();
    t->stackAdresse = currentThread->space->stackTop() - bitmapIndex * 256;

    if(bitmapIndex > 0)
    {
        /* If there was a free stack slot. */
        newThread->stackSlot = bitmapIndex;
        newThread->Start(StartUserThread, t);
    }
    else
    {
        /* If no slots were found, free the structure and stop Nachos. */
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
    /* If the main thread is the only one left, unblock the semaphore. */
    if(thread_count == 1)
        mainStop->V();
    
    /* Free the slot in the stackMap. */
    currentThread->space->stackMap->Clear(currentThread->stackSlot);
    thread_count--;
    currentThread->Finish();
}