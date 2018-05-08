#ifndef UTIL_H
#define UTIL_H

char* readLine(void);

void removeNewline(char* str);

void extractInOutFile(char* input, char** inF, char** outF);

int redirectStdio(char* inF, char* outF);

#endif
