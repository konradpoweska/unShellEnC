typedef struct builtin_t {
  char* namefunc;
  int (*function)(char**);
} builtin_t;

int cd(char **args);

int exit(char **args);

builtin_t* builtinFunc;
