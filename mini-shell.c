#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>
#include "util.h"
#include "builtin.h"
#define BUFFER_SIZE 512
#define DELIM " "


//////////// MAIN ////////////

int main() {
  printf("Welcome to mini-shell! Type *nothing* to exit.\n");

  // input handling
  char buffer[BUFFER_SIZE];
  char* argv[128];
  unsigned short argc;
  char *inF, *outF;


  while(1) {
    // reading from user
    printf("> ");
    fgets(buffer, BUFFER_SIZE, stdin);
    if(*buffer == '\n') break; // if no input, exit
    removeNewline(buffer);


    // in/out redirection
    inF = outF = NULL;
    extractInOutFile(buffer, &inF, &outF);

    // command and args
    argv[argc = 0] = strtok(buffer, DELIM);
    while((argv[++argc] = strtok(NULL, DELIM)));

    // printf("InF = %s\nOutF = %s\n", inF, outF);

    // launching process
    pid_t pid = fork();
    if(pid==0) {
      if(redirectStdio(inF, outF)<0) return -1;
      execvp(argv[0], argv);
      fprintf(stderr, "%s not found\n", argv[0]);
      return 0;
    } else waitpid(pid, NULL, 0);

  }

  printf("Bye :)\n");
  return 0;
}
