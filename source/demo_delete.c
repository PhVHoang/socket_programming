#include<stdio.h> 
#include<stdlib.h> 
  
int main(int c, char *argv[]) 
{ 
    printf("By the time you will compile me I will be destroyed \n"); 
      
   // array of pointers to command line arguments 
    remove(argv[1]);     
  
  // Note: argv[0] will contain the executable file i.e. 'a.out' 
      
return 0; 
} 
  