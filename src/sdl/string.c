#include "string.h"
#include <string.h>

size_t gstrlcpy(char *dst,const char *src,size_t len)
  {
  size_t slen = strlen(src);
  if (len--)
    {
    len=len>slen?slen:len;
    ((char *)memcpy(dst,src,len))[len]='\0';
    }
  return slen;
  }
