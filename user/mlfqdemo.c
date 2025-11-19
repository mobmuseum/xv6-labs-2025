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

static void
usage(void)
{
  printf("usage: mlfqdemo [boost_iter|-1]\n");
  printf("  boost_iter: iteration (0-9) at which to invoke boostproc (default 5)\n");
  printf("  -1 disables the forced boost.\n");
  exit(1);
}

int
main(int argc, char *argv[])
{
  const int iterations = 10;
  int boost_iter = iterations / 2;

  if(argc > 2)
    usage();
  if(argc == 2){
    boost_iter = atoi(argv[1]);
    if(boost_iter < -1 || boost_iter >= iterations)
      usage();
  }

  int cpu_pid = fork();
  if(cpu_pid == 0)
    cpu_worker();

  int io_pid = fork();
  if(io_pid == 0)
    io_worker();

  for(int i = 0; i < iterations; i++){
    pause(5);
    printf("iteration %d:\n", i);
    report("  cpu", cpu_pid);
    report("  io ", io_pid);

    if(boost_iter >= 0 && i == boost_iter){
      printf("mlfqdemo: invoking boostproc() at iteration %d\n", i);
      if(boostproc() < 0)
        printf("mlfqdemo: boostproc failed\n");
    }
  }

  kill(cpu_pid);
  kill(io_pid);
  wait(0);
  wait(0);
  exit(0);
}

