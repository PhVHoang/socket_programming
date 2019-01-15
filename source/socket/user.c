
#include"user.h"
User *findUser(char *id) {
	for(int i =0 ;i<no_user; i++)
	{
		if(!strcmp(user[i].id, id))
			return (user+i);
	}
	return NULL;
}
int indexofUser( char *id)
{
	for(int i=0;i<no_user;i++)
	{
		if(!strcmp(user[i].id, id))
			return i;
	}

}
void writeUser (char *file_name,User *us ) {
    FILE *f = openFile(file_name, "a");


        fprintf(f, "%s %s \n", us->id, us->pass);
    

    fclose(f);
}
void readUser(char *file_name) 
{
	FILE *f =openFile(file_name, "r");
	while(!(feof(f)))
	{
		fscanf(f, "%s %s",user[no_user].id,user[no_user].pass);
		if (user[no_user].id[0] == '\0' || user[no_user].pass[0] == '\0') break;
        
        no_user++;
    }
    fclose(f);
}


FILE *openFile (char *file_name, char *mode) {
    FILE *f = fopen(file_name, mode);

    if (f == NULL) {
        printf("ERROR: Can't open file %s!\n", file_name);
        exit(EXIT_FAILURE);
    }

    return f;
}
User *addUser (char *id, char *pass)
{
    
    User *us = (User *) malloc(sizeof(User));
    strcpy(us->id ,id);
    strcpy(us->pass,pass);
   
return us;
}



	
