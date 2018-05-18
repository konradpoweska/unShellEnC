#ifndef UTIL_H
#define UTIL_H

char* readLine(void);

void checkMalloc(void* p);

void removeNewline(char* str);

void extractInOutFile(char* input, char** inF, char** outF);

int redirectStdio(char* inF, char* outF);

#endif
