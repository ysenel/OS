#ifdef CHANGED
#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"

class SynchConsole:dontcopythis 
{
  public:
    SynchConsole(const char *readFile, const char *writeFile);
		// initialize the hardware console device
    ~SynchConsole();
    // clean up console emulation

    void SynchPutChar(int ch);
    int SynchGetChar();

    void SynchPutString(const char *s);	// Unix puts(3S)
    void SynchGetString(char *s, int n);
    // Unix fgets(3S)
  private:
     Console * console;
};

int copyStringFromMachine(int from, char *to, unsigned size);
void copyStringToMachine(char *from, int to, unsigned size);



#endif				// SYNCHCONSOLE_H
#endif				// CHANGED

