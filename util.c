#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#define BUFFER_USIZE 16



void checkMalloc(void* p) {
  if(p == NULL) {
    fprintf(stderr, "Error: out of memory\n");
    exit(EXIT_FAILURE);
  }
}



char* readLine(void) {
  int c;
  unsigned int bufferCurrentSize = BUFFER_USIZE, pos = 0;

  char* buffer = malloc(bufferCurrentSize*sizeof(char));
  checkMalloc(buffer);

  while(1) {
    c = getchar();

    if(c==EOF) {
      putchar('\n');
      exit(EXIT_SUCCESS);
    }

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
      checkMalloc(buffer);
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
