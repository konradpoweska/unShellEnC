#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#include "input_parser.h"
#include "execution.h"

//////////// MAIN ////////////

int main() {
  printf("Welcome to mini-shell!\n");


  // input handling
  char* input;
  char cwd[1024];
  execCommand* commandSeq;

  while(1) {
    getcwd(cwd, 1024);
    // reading from user
    printf("%s@%s:%s> ", getenv("LOGNAME"), getenv("NAME"), cwd);
    input = readLine();
    commandSeq = parseInput(input);
    execCommand_removeEmpty(&commandSeq);
    executeCommandSequence(commandSeq);
    execCommand_free(commandSeq);
    free(input);
  }

  printf("Bye :)\n");
  return 0;
}
