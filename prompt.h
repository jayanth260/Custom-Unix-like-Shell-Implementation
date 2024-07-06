#ifndef __PROMPT_H
#define __PROMPT_H

#include <sys/utsname.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void prompt(char shl[]);
void getrelpath(char curr[],char shl[]);

#endif