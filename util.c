#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#define BUFFER_USIZE 16

char* readLine(void) {
  int c;
  unsigned int bufferCurrentSize = BUFFER_USIZE, pos = 0;

  char* buffer = malloc(bufferCurrentSize*sizeof(char));
  if(buffer == NULL) {
    fprintf(stderr, "ERROR: out of memory\n");
    exit(EXIT_FAILURE);
  }

  while(1) {
    c = getchar();

    if(c==EOF) exit(EXIT_FAILURE);

    if(c=='\n') { // when user pressed enter
      if(pos>0 && buffer[pos-1]=='\\' && c=='\n') { // if last character is backslash
        printf("> ");
        buffer[--pos] = 0; // go back
        continue; // and continue getting chars
      }
      else break; // or stop getting characters
    }

    // check if buffer large enough, and realloc if not
    if(pos >= bufferCurrentSize) {
      bufferCurrentSize += BUFFER_USIZE;
      buffer = realloc(buffer, bufferCurrentSize*sizeof(char));
      if(buffer == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        exit(EXIT_FAILURE);
      }
    }

    buffer[pos++] = (char)c;
  }

  buffer[pos] = 0;
  return buffer;
}

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
