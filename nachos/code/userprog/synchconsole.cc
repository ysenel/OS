#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

/* Semaphores for protecting input and output of the threads. */
static Semaphore *threadPutSem;
static Semaphore *threadGetSem;

static void ReadAvail(void *arg) { (void) arg; readAvail->V(); }
static void WriteDone(void *arg) { (void) arg; writeDone->V(); }


SynchConsole::SynchConsole(const char *in, const char *out)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);

	threadPutSem = new Semaphore("thread put", 1);
	threadGetSem = new Semaphore("thread get", 1);

	console = new Console(in, out, ReadAvail, WriteDone, NULL);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
	delete threadPutSem;
	delete threadGetSem;
}

void SynchConsole::SynchPutChar(int ch)
{
	threadPutSem->P();
	console->PutChar (ch);
	writeDone->P ();
	threadPutSem->V();
}

int SynchConsole::SynchGetChar()
{
	
	threadGetSem->P();
	readAvail->P();
	int c = console->GetChar();
	threadGetSem->V();
	return c;
}

void SynchConsole::SynchPutString(const char s[])
{
	threadPutSem->P();
	int taille = strlen(s);
	for (int i = 0; i < taille; i++)
	{
		console->PutChar((int)s[i]);
		writeDone->P ();
	}
	threadPutSem->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
	threadGetSem->P();
	char c;
	for(int i = 0; i < n; i++)
	{
		c = (int)console->GetChar();
		if(c == EOF || c == '\n')
			break;
		s[i] = c;
	}
	threadGetSem->V();
}

/* Copies a string from the MIPS virtual memory to a Linux string. */
/* The function reads 'size' caracters from 'from'. This function  */
/* assumes the user has allocated enough memory for the caracters  */
/* and the '\0'.                                                   */

int copyStringFromMachine(int from, char *to, unsigned size)
{
	int tmp;
	unsigned i = 0;

	while(i != size)
	{
		machine->ReadMem(from + i, 1, &tmp);

		if(tmp == '\0')
			break;
		
		to[i++] = tmp;
	}
	
	to[i] = '\0';

	return i;
}


/* Copies a string from the Linux memory to MIPS virtual memory. */
/* size has to be equal to the numbers of caracters including    */
/* the '\0' */
void copyStringToMachine(char *from, int to, unsigned size)
{
  unsigned i;

  for(i = 0; i < size; i++)
  	machine->WriteMem(to + i, 1, from[i]);
}




#endif
