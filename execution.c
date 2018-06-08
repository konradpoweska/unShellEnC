#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "input_parser.h"
#include "redirect-io.h"
#include "util.h"
#include "builtin.h"


void safeClose(int *fd) {
  if(*fd>=0) close(*fd);
  *fd = -1;
}


void executeCommandSequence(execCommand* command) {
  int pipeFd[2][2] = {{-1, -1}, {-1, -1}};
  int fileFd[3] = {-1, -1, -1};
  int i = 0; // used for alternating pipes
  // i%2 is the index of the pipe between current and next
  // 1-i%2 is the index of the pipe between previous and current
  int (*builtinFunction)(char**);
  pid_t pid;


  while(command) {
    // redirect input from previous pipe
    fileFd[0] = pipeFd[1-i%2][0];

    // init pipe & redirect output if next command & no redirections
    if(command->next && !command->outF && !command->next->inF) {
      pipe(pipeFd[i%2]);
      // printf("---Ressource leak test: %d %d ---\n", pipeFd[i%2][0], pipeFd[i%2][1]);
      fileFd[1] = pipeFd[i%2][1];
    }


    if((builtinFunction = getBuiltin(command->argv[0]))) {
      // if matching builtin function exists, execute in main process
      openCommandIOFiles(command, fileFd);
      redirectIO(fileFd);

      builtinFunction(command->argv);

      revertRedirectIO();
      safeClose(&pipeFd[i %2][1]);
      safeClose(&pipeFd[1-i%2][0]);

    } else {

      // execute external program in a child process
      if((pid=fork())==0) {
        // close output of the output pipe in child process
        close(pipeFd[i%2][0]);

        openCommandIOFiles(command, fileFd);
        redirectIO(fileFd);

        execvp(command->argv[0], command->argv);

        // realized only if execvp did not work
        fprintf(stderr, "Error: command not found\n");
        exit(EXIT_FAILURE);
      }

      safeClose(&pipeFd[i%2][1]);
      safeClose(&pipeFd[1-i%2][0]);
      waitpid(pid, NULL, 0);
    }

    i++;
    command = command->next;
  }

}
