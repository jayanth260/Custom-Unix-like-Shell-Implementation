#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#define BUFFER_SIZE 15
#define MAX_INPUT_LENGTH 1024
#define HISTORY_FILE "/home/jayanth/Code/OSN_proj/mini-project-1-jayanth260/history.txt"

int pastevents(char arg[]);
void store_entry(char* entry);
void print_history();
void save_history();
void load_history() ;
int I(const char *str) ;


#endif