#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int cd(char **args) {
  if(chdir(args[1]?args[1]:getenv("HOME")) != 0)
    printf("cd failed\n");

  return 1;
}

int SeCExit(char **args) {
  return 0;
}

char *builtinStr[2] = {
  "cd",
  "exit"
};

int (*builtinFunc[2]) (char **) = {
  &cd,
  &SeCExit
};

int nbBuiltins() {
  return sizeof(builtinStr) / sizeof(char *);
}

//recherche de la builtinFunc
int tryBuiltin(char** argv, int * returnValue) {
  for(unsigned int i=0; i<nbBuiltins(); i++) {
    if(strcmp(argv[0], builtinStr[i])==0) {
      int a = (*builtinFunc[i])(argv);
      if(returnValue) *returnValue = a;
      return 0;
    }
  }
  return -1;
}
