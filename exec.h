#ifndef EXEC_H_
#define EXEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <signal.h>

#include <signal.h>

extern char p_arg[100];
extern int time1;

extern int PPid;
extern int countt;

typedef struct Proc{
    int pid;
    int status;
    int bg;
    char command[20];
    
} Proc;

extern Proc P[50];

int hash(int pid);
void storecomm(char cmd[], int pid);
void check_background_processes();
int exec(char command[], int amp);
int activities();
void sigintHandler(int signo);
void sigtstpHandler(int signo);

#endif
