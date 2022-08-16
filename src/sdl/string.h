#ifndef __GISH_UTILS_H__
#define __GISH_UTILS_H__

#include <stddef.h>

//TODO: I don't know how to prefix these, and they're not worth adding
//      complexity to the build system just to name it 'strlcpy'
size_t gstrlcpy(char *dst,const char *src,size_t len);
//size_t gstrlcat(char *dst,const char *src,size_t len);

#endif//__GISH_UTILS_H__
