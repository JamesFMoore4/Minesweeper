#ifndef WRAPPERS
#define WRAPPERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void* Malloc(size_t);
void* Calloc(size_t, size_t);
void error(char*, int);

#endif
