#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include "util.h"
#include "builtin.h"
#define DELIM " "


//////////// MAIN ////////////

int main() {
  printf("Welcome to mini-shell!\n");


  // input handling
  char* input = NULL;
  char* argv[128];
  unsigned short argc;
  char *inF, *outF;
  char cwd[1024];


  while(1) {
    getcwd(cwd, 1024);
    // reading from user
    printf("%s@%s:%s> ", getenv("LOGNAME"), getenv("NAME"), cwd);
    input = readLine();

    // in/out redirection
    inF = outF = NULL;
    extractInOutFile(input, &inF, &outF);

    // command and args
    argv[argc = 0] = strtok(input, DELIM);
    while((argv[++argc] = strtok(NULL, DELIM)));

    // printf("InF = %s\nOutF = %s\n", inF, outF);

    // launching process
    if(tryBuiltin(argv, NULL)<0) {
      pid_t pid = fork();
      if(pid==0) {
        if(redirectStdio(inF, outF)<0) return -1;
        execvp(argv[0], argv);
        fprintf(stderr, "%s not found\n", argv[0]);
        return 0;
      } else waitpid(pid, NULL, 0);
    }
    if(input) {free(input); input = NULL;}
  }

  printf("Bye :)\n");
  return 0;
}
