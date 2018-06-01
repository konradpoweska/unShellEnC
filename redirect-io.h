#ifndef REDIRECT_IO_H
#define REDIRECT_IO_H

void openIOFiles(char** files, int* fd);

void openCommandIOFiles(execCommand* c, int* fd);

void redirectIO(int* fd);

void revertRedirectIO();

#endif
