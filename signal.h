#ifndef SIGNAL_H_
#define SIGNAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int ping(int pid,int sig);
int II(const char *str);


#endif