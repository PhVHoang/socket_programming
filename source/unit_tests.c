#include <stdio.h>

void delete_file(char* filename) {
    remove(filename);
}

int main(int argc, char const *argv[])
{
    char* filename = "socket/11.pdf";
    delete_file(filename);
    return 0;
}

