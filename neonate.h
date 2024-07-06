#ifndef __NEONATE_H
#define __NEONATE_H

#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>


int III(const char *str);
int kbhit(void);
int neonate(int argc, char *argv[]) ;

#endif