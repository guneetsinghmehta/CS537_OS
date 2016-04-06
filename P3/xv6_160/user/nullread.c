#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#undef NULL
#define NULL ((void*)0)

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   exit(); \
}

int
main(int argc, char *argv[])
{
  
//        printf(1, "%x\n",shmem_access(0));
char *c=(char*) shmem_access(0);
printf(1,"return valur of shmem_access=%p\n",c);
printf(1,"return value of shmem_access=%p =%c\n",c,*c);
  exit();
}
