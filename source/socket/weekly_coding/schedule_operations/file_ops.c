#include "file_operations.h"

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

