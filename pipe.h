#ifndef PIPE_H_
#define PIPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "headers.h"


// extern int ccc;

char **arg(char args[]);
int create_child(int depth,int max_depth,int* pipe1,char* args[]);
char **check1(char args[]);
int manage_IO(char args[]);
int PIPE(char input[]);

#endif