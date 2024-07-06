#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "pastevents.h"
#include "exec.h"
#include "seek.h"
#include "proclore.h"
#include "pipe.h"
#include "signal.h"
#include "setjmp.h"
#include "BF.h"
#include "neonate.h"
#include "iMan.h"

void cmd(char input[]);
// int processcmd(char cmd[]);
int runcmd(char* arg[],int n_args);
char* remove_extra_spaces(char input[]) ;
void exit_function();
int check(char cmd[]);

#endif