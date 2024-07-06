#ifndef PROCLORE_H_
#define PROCLORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void print_process_info(const char *pid) ;
int proclore(int argc, char *argv[]) ;


#endif