#ifndef UTIL_H
#define UTIL_H

void removeNewline(char* str);

void extractInOutFile(char* input, char** inF, char** outF);

int redirectStdio(char* inF, char* outF);

#endif
