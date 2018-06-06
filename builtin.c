#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "builtin.h"
#include "util.h"


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
  //parsing options
  char* dirName = NULL;
  int option = 0, hidden = 0, details = 0;
  unsigned int i=1;
  while(args[i]!=NULL){
    while(strcmp(args[i],"")!=0){
      if(*args[i]!='-' && !option){
        dirName=args[i];
        break;
      }
      option = 1;
      if(*args[i]=='l') details = 1;
      if(*args[i]=='a') hidden = 1;
      args[i]++;
    }
    option = 0;
    i++;
  }

  DIR* dir = opendir(dirName ? dirName : ".");
  if (dir==NULL){
    fprintf(stderr, "Couldn't open directory %s\n", dirName);
    return -1;
  }
  struct dirent* ent = readdir(dir);
  while(ent != NULL) {
    if(!details){
      if ((!hidden && *ent->d_name != '.') || hidden){
        printf ("%s\n", ent->d_name);
      }
    }
    else {
      if ((!hidden && *ent->d_name != '.') || hidden){
        struct stat* buffer = malloc(sizeof(struct stat));
        stat(ent->d_name,buffer);
        char* perms = malloc(10*sizeof(char));
        char* modificationDate = asctime(gmtime(&buffer->st_mtime));
        removeNewline(modificationDate);
        printf("%s %u\t%s\t%s\t%li\t%s %s\n",
          printMode(buffer->st_mode,perms),
          (unsigned int)buffer->st_nlink,
          getpwuid(buffer->st_uid)->pw_name,
          getgrgid(buffer->st_gid)->gr_name,
          buffer->st_size,
          modificationDate,
          ent->d_name);
        free(buffer);
        free(perms);
      }
    }
    ent=readdir(dir);
  }
  closedir (dir);
  return 1;
}


char* printMode(mode_t permissions, char* perms){
  unsigned int i = 0;
  perms[i++]=(S_ISDIR(permissions)) ? 'd' : '-';
  perms[i++]=(permissions & S_IRUSR) ? 'r' : '-';
  perms[i++]=(permissions & S_IWUSR) ? 'w' : '-';
  perms[i++]=(permissions & S_IXUSR) ? 'x' : '-';
  perms[i++]=(permissions & S_IRGRP) ? 'r' : '-';
  perms[i++]=(permissions & S_IWGRP) ? 'w' : '-';
  perms[i++]=(permissions & S_IXGRP) ? 'x' : '-';
  perms[i++]=(permissions & S_IROTH) ? 'r' : '-';
  perms[i++]=(permissions & S_IWOTH) ? 'w' : '-';
  perms[i]=(permissions & S_IXOTH) ? 'x' : '-';

  return perms;
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

//search of builtinFunc
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
