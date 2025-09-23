#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ticks = uptime();   // system call
  printf("%d ticks\n", ticks);
  exit(0);
}
