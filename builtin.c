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
  unsigned int i=0;
  while(args[++i]!=NULL){
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


int find (char** args){
  //parsing options and path
  char* initialPath=malloc(512*sizeof(char));
  char* searchName=malloc(512*sizeof(char));
  int option = 0, print =0;
  unsigned int i=0;
  if(*args[1]!='-'){
    initialPath=args[1];
    i++;
  }else initialPath=".";
  while(args[++i]!=NULL){
    if(strcmp(args[i],"-name")==0){
      option=1;
    }
    else if(strcmp(args[i],"-print")==0){
      print=1;
    }
    else if(*args[i]=='-'){
      printf("Option %s not supported\n", args[i]);
      return -1;
    }
    else if (option){
      searchName = args[i];
      option = 0;
    }
    else {
      printf("You must enter an option\n");
      return -1;
    }
  }


  /*if (dir==NULL){
    fprintf(stderr, "Couldn't open initial directory %s\n", initialPath);
    return -1;
  }*/
  //struct dirent* ent = readdir(dir);

  getFile(initialPath,searchName,print);
  //closedir(dir);

  return 1;
}


//Used in ls to print permissions
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



void getFile(char* path, char* searchName,int print){

  DIR* dir = opendir(path);
  if(dir==NULL) {
    fprintf(stderr,"couldn't open file\n");
    return;
  }
  struct dirent* ent = readdir(dir);
  if(ent==NULL) {
    fprintf(stderr, "cannot read directory\n" );
    return;
  }
  struct stat buffer;
  stat(ent->d_name,&buffer);

  if(!(S_ISDIR(buffer.st_mode)) && strcmp(ent->d_name,searchName)==0){
    if(print){
      char* newPath = malloc(1024*sizeof(char));
      strcpy(newPath,path);
      strcat(newPath,"/");
      strcat(newPath,ent->d_name);
      printf("%s\n",newPath);
      free(newPath);
      closedir(dir);
      return;
    }
  }
  else if(!(S_ISDIR(buffer.st_mode)) && strcmp(ent->d_name,searchName)!=0){
    return;
  }
  else{
    if(print && strcmp(ent->d_name,searchName)==0){
      printf("%s/%s\n",getenv("CWD"),searchName);
    }

    while(ent != NULL) {
      char* newPath = malloc(1024*sizeof(char));
      strcpy(newPath,path);
      strcat(newPath,"/");
      strcat(newPath,ent->d_name);
      if(strcmp(ent->d_name,searchName)==0){
        if(print){
          printf("%s\n",newPath);
        }
      }
      stat(ent->d_name,&buffer);
      if((strcmp(ent->d_name,searchName)!=0)&&(*ent->d_name!='.') && (S_ISDIR(buffer.st_mode))){
        getFile(newPath,searchName,print);
      }
      ent=readdir(dir);
      free(newPath);
    }

    return;
  }
}



char *builtinStr[5] = {
  "cd",
  "exit",
  "cat",
  "ls",
  "find"
};



int (*builtinFunc[5]) (char **) = {
  &cd,
  &SeCExit,
  &cat,
  &ls,
  &find
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
