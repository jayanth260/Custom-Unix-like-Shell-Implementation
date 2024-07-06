#ifndef SEEK_H_
#define SEEK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fnmatch.h>


char *remove_extension(char *filename);
int seek_recursive(const char *target, const char *currentDirectory, int isFileOnly, int isDirOnly, int isExecute);
int seek(const char *target, const char *targetDirectory, int isFileOnly, int isDirOnly, int isExecute);
int Seek_proc(int argc, char *argv[]);


#endif