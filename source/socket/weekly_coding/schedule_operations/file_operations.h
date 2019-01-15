#ifndef FILE_OPERATIONS
#define FILE_OPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 250

struct Schedule {
    char Code[10];
    char Course[100];
    char Week_day[20];
} schedule;

struct Schedule schedule[1000];
int schedule_size = 0;
void copyToken(char* destination,
               char* source,
               size_t maxLen,
               char const* delimiter);

void with_helper(char* filename);

#endif


