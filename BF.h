#ifndef BF_H_
#define BF_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "exec.h"

int bg(int pid);
int fg(int pid);

#endif