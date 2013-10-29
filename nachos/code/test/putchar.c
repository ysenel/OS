#include "syscall.h"

void print(char c, int n)
{
	int i;

	#if 1
		for (i = 0; i < n; i++)
		{
			PutChar(c + i);
		}
			
		PutChar('\n');
	#endif
}

void print_string(char *s)
{
	PutString(s);
}


int main()
{

	/*char c = GetChar();
	PutChar(c);*/

	//print_string("hello world\n");
	//char buf[100];
	//GetString(buf,10);
	PutString("Hi\n");
	PutString("Hell\n");
	PutString("Helloworld\n");

	/*int a;
	GetInt(&a);

	PutInt(a);*/

	
	//Halt();
}
