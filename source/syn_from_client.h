#include<stdio.h>
#include<string>
#include<errno.h>
#include<libgen.h>

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
