#include <stdio.h>
#include<errno.h>

void delete_file(char* filename) {
    remove(filename);
}

int create_folder(char* folder_name) {
	return mkdir(folder_name, 0755);
}

char *combine_path(const char *dir, const char *name)
{
    /* Calculate the lengths of the path components.
       If the respective parameter is NULL, the length is zero .*/
    const size_t  dirlen = (dir) ? strlen(dir) : 0;
    const size_t  namelen = (name) ? strlen(name) : 0;

    char         *path, *p;

    /* We allocate <dir> + '/' + <name> + '\0'. */
    path = malloc(dirlen + namelen + 2);
    if (!path) {
        errno = ENOMEM;
        return NULL;
    }

    /* Let p point to the current position in the
       resulting path. */
    p = path;

    /* If there is a directory part,
       copy it, and append a '/' after it. */
    if (dirlen > 0) {
        memcpy(p, dir, dirlen);
        p += dirlen;
        *p = '/';
        p += 1;
    }

    /* If there is a name part, copy it. */
    if (namelen > 0) {
        memcpy(p, name, namelen);
        p += namelen;
    }

    /* Append a NUL char, '\0', to terminate the
       dynamically allocated buffer.
       This turns it into a C string. */
    *p = '\0';

    /* Return the pointer to the dynamically-allocated
       memory, containing the concatenated paths
       as a single string. */
    return path;
}

int copyfile1(char* infilename, char* outfileDir) {
    FILE* infile; //File handles for source and destination.
    FILE* outfile;
    char outfilename[1000];

    infile = fopen(infilename, "r"); // Open the input and output files.
    if (infile == NULL) {
      printf("%s not found\n", infilename);
      return 1;
    }
    sprintf(outfilename, "%s/%s", outfileDir, basename(infilename));

    outfile = fopen(outfilename, "w");
}

int main(int argc, char const *argv[])
{
    char* filename = "socket/11.pdf";
    char* subfolder_name = "highland/highland1";
    // delete_file(filename);
    // create_folder(subfolder_name);
    copyfile1("./abc.xyz", "phantrang/");
    return 0;
}

