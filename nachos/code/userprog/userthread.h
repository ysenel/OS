#ifndef USERTHREAD_H
#define USERTHREAD_H

extern int do_ThreadCreate(int f, int arg);

extern void do_ThreadExit();
static void StartUserThread(void *schmurtz);

#endif