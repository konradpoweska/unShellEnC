#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"
#include "input_parser.h"
#define ARRAY_BUFFER_SIZE 4

/*
  The parser handles :
   - multiple pipes
   - input, output, and error redirection
   - quotes


  Example input :
    cat < input.txt  |  cut -f 1 -d " "  |  uniq > output.txt
  output (JSON-formatted) :
  [
    {
      ["cat"],
      inF: "input.txt"
    },
    {
      ["sort", "-f", "1", "-d", " "]
    },
    {
      ["uniq"],
      outF: "output.txt"
    }
  ]


*/



execCommand* execCommand_init(void) {
  execCommand* o = calloc(1, sizeof(execCommand)); // all pointers = 0
  return o;
}




void execCommand_free(execCommand* el) {
  if(el == NULL) return;
  execCommand_free(el->next);
  if(el->argv) free(el->argv);
  free(el);
}




char* findText(char* c) {
  // returns a pointer to the charachter after the found text

  int offset = 0; // for removing quotes by shifting
  char quotes = 0; // modes : 0, '\'' or '\"'

  while(
    (
      quotes
      || (
        !isspace(*c)
        && *c != '>'
        && *c != '<'
        && *c != '|'
      )
    ) && *c != 0
  ) {
    if(!quotes && (*c=='\"' || *c=='\'')) {
      quotes = *c;
      offset++;
    } else if(*c == quotes) {
      quotes = 0;
      offset++;
    }

    c++;
    if(offset) *(c-offset) = *c;
  }

  if(quotes) fprintf(stderr, "Error: missing closing quote.\n");
  if(offset) *(c-offset) = 0;
  return c;
}




execCommand *parseInput(char *input) {
  execCommand *currentCommand = execCommand_init();
  execCommand *execSeq = currentCommand; // remember first element of linked list

  unsigned int currentArgvSize = ARRAY_BUFFER_SIZE;
  currentCommand->argv = malloc(currentArgvSize*sizeof(char*));
  checkMalloc(currentCommand->argv);

  while(*input!=0) {

    // whitespace handling
    if(isspace(*input)) {
      *input = 0;
      do input++; while(isspace(*input));
      if(*input==0) break;
    }

    // redirection handling
    if(*input=='<' || *input=='>' || (*input=='2' && *(input+1)=='>')) {
      char* c = input;
      if(*c=='2') input++;
      do input++; while(isspace(*input));

      char* input2 = findText(input);
      if(input2 == input) continue; // if no filename found, abort

      if(*c=='<') currentCommand->inF = input;
      else if(*c=='>') currentCommand->outF = input;
      else if(*c=='2') currentCommand->errF = input;
      input = input2;
      *c = 0;
      continue;
    }

    // pipe handling
    if(*input == '|') {
      currentCommand->argv[currentCommand->argc] = NULL; // end the current

      currentCommand = currentCommand->next = execCommand_init();
      currentArgvSize = ARRAY_BUFFER_SIZE;
      currentCommand->argv = realloc(currentCommand->argv, currentArgvSize*sizeof(char*));
      checkMalloc(currentCommand->argv);

      *(input++) = 0; // end the string and go to next
      continue;
    }

    // store to argv
    currentCommand->argv[currentCommand->argc++] = input;
    input = findText(input);

    // reallocate more memory if all array used by data
    if(currentCommand->argc >= currentArgvSize) {
      currentArgvSize += ARRAY_BUFFER_SIZE;
      printf("Reallocating size %u, argc = %u\n", currentArgvSize, currentCommand->argc);
      currentCommand->argv = realloc(currentCommand->argv, currentArgvSize*sizeof(char*));
      checkMalloc(currentCommand->argv);
    }
  }

  currentCommand->argv[currentCommand->argc] = NULL; // end the current
  return execSeq;
}




void execCommand_removeEmpty(execCommand** headPtr) {
  if(!headPtr || !*headPtr) return;
  execCommand_removeEmpty(&((*headPtr)->next));
  if((*headPtr)->argc == 0) { // filter condition
    execCommand *l = *headPtr;
    *headPtr = l->next;
    l->next = NULL; // to avoid recursive freeing
    execCommand_free(l);
  }
}




void execCommand_print(execCommand* l) {
  char** c;
  printf("[\n");
  while(1) {
    printf("\t{\n\t\targc: %d,\n\t\targv: [", l->argc);
    printf("\"%s\"", *(c=l->argv));
    while(*(++c)) printf(", \"%s\"", *c);
    printf("]");
    if(l->inF) printf(",\n\t\tinF: \"%s\"", l->inF);
    if(l->outF) printf(",\n\t\toutF: \"%s\"", l->outF);
    if(l->errF) printf(",\n\t\terrF: \"%s\"", l->errF);
    printf("\n\t}");
    if((l=l->next)) printf(",\n");
    else {printf("\n]\n"); break;}
  }
}




void execCommand_demo(void) {
  char buffer[512];
  execCommand* seq;
  while(printf("input: ") && fgets(buffer, 512, stdin) && *buffer!='\n') {
    removeNewline(buffer);
    seq = parseInput(buffer);
    execCommand_removeEmpty(&seq);
    execCommand_print(seq);
    execCommand_free(seq);
  }
}
