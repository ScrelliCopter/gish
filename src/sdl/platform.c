#include "platform.h"

#ifdef DETLEF

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

//FIXME: this provides no way to check the result of the child's exec or the status code of the exec'd program
//       good enough for just opening website links for now
int gexec(const char *pathname,const char *arg)
{
  // check if pathname is executable
  struct stat sb;
  if (stat(pathname,&sb) || !(sb.st_mode&S_IXUSR))
  {
#ifdef DEBUG
    fprintf(stderr, "stat(): %s is not executable\n", pathname);
#endif
    return 1;
  }

  // deduce executable name from path
  const char *execname=strrchr(pathname,'/');
  execname=execname?execname+1:pathname;

  // fork & run the program
  pid_t pid=fork();
  if (pid==-1)
  {
#ifdef DEBUG
    fprintf(stderr, "fork(): returned -1\n");
#endif
    return 1;
  }
  else if (!pid) // we are the child
  {
    int err=execl(pathname,execname,arg,(char *)0);
#ifdef DEBUG
    fprintf(stderr,"execl(): returned %i errno is %s\n",err,strerror(errno));
#endif
    exit(1);
  }

  return 0;
}

#endif
