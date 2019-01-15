// #include <file_operations.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 250

typedef struct Schedule {
    char Code[50];
    char Course[100];
    char Week_day[100];
    // char AM_PM[20];
    // char Period[10];
    // char Week[100];
    // char Room[20];
} Schedule;

struct Schedule schedule[1000];

// helper function
void copyToken(char* destination,
               char* source,
               size_t maxLen,
               char const* delimiter) {
    char* token = strtok(source, delimiter);
    if ( token != NULL )
    {
       destination[0] = '\0';
       strncat(destination, token, maxLen-1);
    }
}
// From here

Schedule schedule[1000];
int schedule_size = 0;

void with_helper(char* filename) {
    FILE* f = fopen(filename, "r");
    char line[LINE_SIZE];
    int i = 0;
    while (fgets(line, sizeof(line), f) != NULL) {
        copyToken(schedule[i].Code, line, sizeof(schedule[i].Code), "\t");
        copyToken(schedule[i].Course, NULL, sizeof(schedule[i].Course), "\t");
        copyToken(schedule[i].Week_day, NULL, sizeof(schedule[i].Week_day), "\n");
        i++;
    }
    schedule_size = i;
    fclose(f);

}

// int main() {
//     // test_on_new_file("test_ex1.txt");
//     // test_readfile("np_ex1.txt");
//     with_helper("np_ex1.txt");
//     for (int i = 0; i < schedule_size; i++) {
//         printf("%s\n", schedule[i].Code);
//         printf("%s\n", schedule[i].Course);
//         printf("%s\n", schedule[i].Week_day);
//     }
//     return 0;
// }
