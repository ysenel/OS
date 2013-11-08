#include "syscall.h"




/*void testd(void *titi)
{
	
	PutString("salut\n");
	ThreadExit();
}*/

void test(void * j)
{
	int i = (char) j;

	PutChar('-');
	PutChar(i);
	PutChar('-');

	ThreadExit();
}



int main()
{
	int j = 'a';
	int i = 9;

	for(i = 0; i < 6; i++)
		ThreadCreate(test, (void *)j);


	return 0;
}	
