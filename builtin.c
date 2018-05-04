#include "builtin.h"
#include <stdio.h>


int cd(char **args)
{
  if (args[1] == NULL) {
    chdir(getenv("HOME"));
  } else {
    if (chdir(args[1]) != 0) {
      perror("cd failed");
    }
  }
  return 1;
}

int exit(char **args) {
  return 0;
}

builtinFunc[] = {
  {"cd",&cd},
  {"exit",&exit}
};
