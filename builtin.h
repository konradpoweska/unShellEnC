#ifndef BUILT_IN_H
#define BUILT_IN_H

void getFile(char* path, char* searchName,int print);

char* printMode(mode_t permissions, char* perms);

int (*getBuiltin(char* command))(char**);

#endif
