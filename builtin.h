
/*int cd(char **args);

int SeCExit(char **args);

char *builtinStr[2];

int (*builtinFunc[2]) (char **);

int nbBuiltins();*/

int tryBuiltin(char** argv, int * returnValue);

int (*getBuiltin(char* command))(char**);
