#include "syscall.h"




/*void testd(int titi)
{
	
	PutString("salut\n");
	int j;
	for (j = 0; j < 9; j++)
	{

		titi++;
		PutInt(titi);
	}

	PutChar('q');

	ThreadExit();
}*/

void test(void *toto)
{
	/*volatile int cpt = 0;
	int i;
	for(i = 0; i < 10; i++)
		PutInt(cpt++);*/

	//PutInt(i);
	/*int i;
	for (i = 0; i < 9; i++)
	{
		toto++;
		PutInt(toto);
	}
*/
	int i;
	for(i=0; i<50; i++)
		PutChar('a');
	ThreadExit();
}



int main()
{
	int j = 0;
	//int i = 40000;
	
	ThreadCreate(&test, &j);
	//ThreadCreate(&testd, &j);
	
	PutChar('m');


	/*while(i>0)
	{
		i--;
	};*/
	
	return 0;
}
