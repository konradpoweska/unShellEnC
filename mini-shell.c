#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>
#include <ctype.h>
#define BUFFER_SIZE 512
#define DELIM " "



//////////// UTIL ////////////

void removeNewline(char* str) {
  while(*str!='\n' && *str!='\0') str++;
  *str = '\0';
}


void extractInOutFile(char* input, char** inF, char** outF) {
  // reading from end
  if(*input==0) return;
  char *start = input, *c; // copy + another pointer used for loops
  while(*input!='\0') input++; // go to end

  while((--input)!=start)
    if(*input=='<' || *input=='>') {
      c = input; // copy for internal loops
      while(isspace(*(++c))); // skipping whitespace

      if(*input=='<')
        *inF = c;
      else if(*input=='>')
        *outF = c;

      while(!isspace(*(++c))); // skipping not-whitespace (filename)
      *c = '\0'; // ending the string after the filename
      *input = '\0'; // ending the string before the < or >
    }

}


int redirectStdio(char* inF, char* outF) {
  if(inF) {
    int inFildes = open(inF, O_RDONLY);
    if(inFildes<0) {
      fprintf(stderr, "Error: can't open %s.\n", inF);
      return -1;
    }
    dup2(inFildes, 0);
  }
  if(outF) {
    int outFildes = open(outF, O_WRONLY | O_CREAT | O_EXCL);
    if(outFildes<0) {
      fprintf(stderr, "Error: can't write into %s.\n", outF);
      return -1;
    }
    dup2(outFildes, 1);
  }
  return 0;
}





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
