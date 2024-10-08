#include "wrappers.h"

void* Malloc(size_t size)
{
  void* ret;

  if (!(ret = malloc(size)))
    error("malloc error", 1);
  return ret;
}

void* Calloc(size_t nmemb, size_t size)
{
  void* ret;
  if (!(ret = calloc(nmemb, size)))
    error("calloc error", 1);
  return ret;
}

void error(char* msg, int status)
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(status);
}
