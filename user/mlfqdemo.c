#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

static void
cpu_worker(void)
{
  volatile int junk = 0;
  for(;;){
    for(int i = 0; i < 100000; i++)
      junk += i;
  }
}

static void
io_worker(void)
{
  for(;;){
    pause(2);
  }
}

static void
report(const char *label, int pid)
{
  struct procinfo info;
  if(getprocinfo(pid, &info) < 0)
    return;
  printf("%s(pid=%d): level=%d budget=%d total=%d\n",
         label, pid, info.current_level, info.time_slice_budget,
         (int)info.total_runtime);
}

int
main(int argc, char *argv[])
{
  int cpu_pid = fork();
  if(cpu_pid == 0)
    cpu_worker();

  int io_pid = fork();
  if(io_pid == 0)
    io_worker();

  for(int i = 0; i < 10; i++){
    pause(5);
    report("cpu", cpu_pid);
    report("io", io_pid);
  }

  kill(cpu_pid);
  kill(io_pid);
  wait(0);
  wait(0);
  exit(0);
}

