void getFile(char* path, char* searchName,int print);

char* printMode(mode_t permissions, char* perms);

int tryBuiltin(char** argv, int * returnValue);

int (*getBuiltin(char* command))(char**);
