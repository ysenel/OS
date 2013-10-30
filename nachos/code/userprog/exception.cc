// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include <limits.h>
#include "userthread.h"
#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    switch (which)
    {

	case SyscallException:
    {
	    switch (type)
	    {
			case SC_Halt:
			{
			    DEBUG ('a', "Shutdown, initiated by user program.\n");
			    interrupt->Halt ();
			    break;
			}

		    #ifdef CHANGED
		  	case SC_PutChar:
			{
			  	synchconsole->SynchPutChar(machine->ReadRegister(4));
			    DEBUG ('a', "PutChar\n");
			    break;
			}

			case SC_Exit:
			{
				DEBUG ('a', "Shutdown, initiated by user program.\n");
				printf("\nEnd of program: return value %d\n", machine->ReadRegister(4));
			    interrupt->Halt ();
			    break;
			}

			case SC_PutInt:
			{
				
				char buffer[MAX_STRING_SIZE+1];

				snprintf(buffer, MAX_STRING_SIZE+1, "%d", machine->ReadRegister(4));
				synchconsole->SynchPutString(buffer);

				break;

			}

			case SC_GetInt:
			{
				int i = machine->ReadRegister(4);
				int max_int_length = 9;

				char buffer[max_int_length];
				synchconsole->SynchGetString(buffer, max_int_length);

				int tmp;
				sscanf(buffer, "%d", &tmp);

				/* 4 because it's the size of a int. */
				/* We checked this with the sizeof operator */
				machine->WriteMem(i,4,tmp);
				break;

			}


			case SC_ThreadCreate:
			{
				
				int i = machine->ReadRegister(4);
				int j = machine->ReadRegister(5);
				do_ThreadCreate(i, j);

				break;

			}


			case SC_ThreadExit:
			{
				do_ThreadExit();

				break;

			}
			


			case SC_GetChar:
			{
			  	int c = synchconsole->SynchGetChar();
			  	machine->WriteRegister(2, c);
			    break;
			}



			case SC_GetString:
			{
				char buffer[MAX_STRING_SIZE + 1];
				int debut = machine->ReadRegister(4);
      			int size = machine->ReadRegister(5);

      			/* If the string is longer than the MAX_STRING_SIZE,       */
      			/* only the MAX_STRING_SIZE first caracters while be read. */

      			if(size>MAX_STRING_SIZE)
	  				size=MAX_STRING_SIZE;

				synchconsole->SynchGetString(buffer,size);
				copyStringToMachine(buffer,debut,size);

			    break;
			}

		  	case SC_PutString:
			{
				int c = machine->ReadRegister(4);
			  	char buffer[MAX_STRING_SIZE+1];

			  	/* When calling copyStringFromMachine, the size has to be: number of caracter+1 */
			  	/* for the '\0'.                                                                */

			  	while(copyStringFromMachine(c, buffer, MAX_STRING_SIZE) == MAX_STRING_SIZE)
			  	{
			  		synchconsole->SynchPutString(buffer);
			  		c = c + MAX_STRING_SIZE;
			  	}

			  	/* One last call because buffer possibly contains a part of a string. */
			  	synchconsole->SynchPutString(buffer);
			  	
			    DEBUG ('a', "PutString\n");
			    break;
			}
			#endif

			default:
			{
			    printf("Unimplemented system call %d\n", type);
			    ASSERT(FALSE);
			}
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	}

	case PageFaultException:
	  if (!type) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }

	default:
	  printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
	  ASSERT (FALSE);
    }
}
