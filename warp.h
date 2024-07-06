#ifndef WARP_H_
#define WARP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int warp(char path[],char shl[]) ;
char* getactpath(char* path, char* shl);
void remove_consecutive_slashes(char *path) ;

#endif