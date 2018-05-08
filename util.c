#include <ctype.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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
