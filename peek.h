#ifndef PEEK_H_
#define PEEK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define COLOR_BLUE "\x1b[34m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_WHITE "\x1b[0m"

void list_files(char path[], int show_hidden, int show_details) ;
int peek(char path[],int A,int L,char shl[]);




#endif