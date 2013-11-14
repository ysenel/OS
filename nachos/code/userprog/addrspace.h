// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"

#ifdef CHANGED
    #include "bitmap.h"
#endif

#define UserStacksAreaSize		1024*4	// increase this as necessary!

class AddrSpace:dontcopythis
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 
   
    #ifdef CHANGED

    static void ReadAtVirtual(OpenFile *executable, int virtualaddr, int numBytes, int position, TranslationEntry *pageTable, unsigned numPages);


    /* Called when a thread is created. */
    int AllocateUserStack ();

    /* Called when a thread finishes. */
    void clearStackSlot(int index);

    /* Returns the top of the stack. */
    int stackTop();

    /* This is the the map used for the user thread stacks.   */
    /* public for direct acces, should maybe be private with. */
    BitMap *stackMap;
    #endif


  private:
    TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space


 
};

#endif // ADDRSPACE_H
