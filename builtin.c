#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>



int cd(char **args) {
  if(chdir(args[1]?args[1]:getenv("HOME")) != 0)
    printf("cd failed\n");

  return 1;
}

int SeCExit(char **args) {
  exit(0);
  printf("exit failed\n");
  return 1;
}

int cat(char **args){
  FILE* startfile;
  if(args[1]){
    startfile = fopen(args[1],"r");}
  else{
    startfile=stdin;}

  int character=getc(startfile);
  while(character != EOF){
    putc((char)character,stdout);
    character=getc(startfile);
  }
  return 1;
}

int ls(char** args){
  DIR* dir = opendir(getenv("PWD"));
  struct dirent* ent = readdir(dir);
  while (ent != NULL) {
    printf ("%s\n", ent->d_name);
    ent=readdir(dir);
  }
  closedir (dir);
  return 1;
}

char *builtinStr[4] = {
  "cd",
  "exit",
  "cat",
  "ls"
};

int (*builtinFunc[4]) (char **) = {
  &cd,
  &SeCExit,
  &cat,
  &ls
};

int nbBuiltins() {
  return sizeof(builtinStr) / sizeof(char *);
}

//recherche de la builtinFunc
int tryBuiltin(char** argv, int * returnValue) {
  for(unsigned int i=0; i<nbBuiltins(); i++) {
    if(strcmp(argv[0], builtinStr[i])==0) {
      int a = (*builtinFunc[i])(argv);
      if(returnValue) *returnValue = a;
      return 0;
    }
  }
  return -1;
}


int (*getBuiltin(char* command))(char **) {
  for(unsigned int i=0; i<nbBuiltins(); i++)
    if(strcmp(command, builtinStr[i])==0)
      return builtinFunc[i];

  return NULL;
}
