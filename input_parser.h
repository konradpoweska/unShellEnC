#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H


typedef struct execCommand {
  unsigned int argc;
  char **argv;
  char *inF;
  char *outF;
  char *errF;
  struct execCommand* next;
} execCommand;



execCommand* execCommand_init(void);

void execCommand_free(execCommand* el);

execCommand *parseInput(char *input);

void execCommand_removeEmpty(execCommand** headPtr);

void execCommand_print(execCommand* l);

void execCommand_demo(void);

#endif
