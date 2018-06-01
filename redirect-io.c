#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"
#include "input_parser.h"


const char const* const stdioStreamNames[] = {"input", "output", "error"};

// process-global cuz redirections are process-global
int stdioBackup[] = {-1, -1, -1};



void openIOFiles(char** files, int* fd) { // fd used for output
  unsigned int i;
  for(i=0; i<=2; i++) {

    fd[i] = -1;
    if(!files[i]) continue; // skip if file not provided

    fd[i] = open(files[i], i?(O_WRONLY|O_CREAT|O_EXCL):O_RDONLY);
    if(fd<0) {
      fprintf(stderr, "Error: can't %s %s\n", i?"write into":"open", files[i]);
      continue;
    }
  }
}

// wrapper for execCommand IO files
void openCommandIOFiles(execCommand* c, int* fd) {
  char* files[3] = {c->inF, c->outF, c->errF};
  openIOFiles(files, fd);
}




void redirectIO(int* fd) {
  unsigned int i;
  for(i=0; i<=2; i++) {

    if(fd[i]<0) continue;
    if(stdioBackup[i]>=0) { // skip if stream already redirected
      fprintf(stderr, "Error: %s stream already redirected.\n", stdioStreamNames[i]);
      continue;
    }

    if((stdioBackup[i] = dup(i))<0) { // backup stdio stream
      fprintf(stderr, "Error: Couldn't redirect %s stream.\n", stdioStreamNames[i]);
      continue;
    }

    if(dup2(fd[i], i)<0) { // replace stdio with file
      close(stdioBackup[i]);
      stdioBackup[i] = -1;
      fprintf(stderr, "Error: Couldn't redirect %s stream to file.\n", stdioStreamNames[i]);
      continue;
    }

  }

  for(i=0; i<=2; i++) close(fd[i]);
}




void revertRedirectIO() {
  unsigned int i;
  for(i=0; i<=2; i++) {

    if(stdioBackup[i]<0) continue; // if not redirected

    if(i==1) fflush(stdout); // stdout is buffered

    // close(i) is automatically done by dup2
    dup2(stdioBackup[i], i);

    close(stdioBackup[i]);
    stdioBackup[i] = -1;

  }
}




/* USAGE DEMO:

  char* files[] = {NULL, "output", NULL};
  int fd[3];
  openIOFiles(files, fd); // open files
  redirectIO(fd);

  printf("hello file\n");

  revertRedirectIO();

  printf("hello terminal\n");

*/
