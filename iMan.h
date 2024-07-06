#ifndef IMAN_H_
#define IMAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFFER_SIZE 4096

void error(const char *msg);
void remove_pre_tags(char *input) ;
int iMan(int argc, char *argv[]);


#endif