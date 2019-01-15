#include <stdio.h>

void delete_file(char* filename) {
    remove(filename);
}

int create_folder(char* folder_name) {
	return mkdir(folder_name, 0755);
}

int main(int argc, char const *argv[])
{
    char* filename = "socket/11.pdf";
    char* subfolder_name = "highland/highland1";
    // delete_file(filename);
    create_folder(subfolder_name);
    return 0;
}

