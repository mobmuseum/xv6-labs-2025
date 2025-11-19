#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

static char *
state_name(int state)
{
  static char *states[] = {
    [UNUSED]   "unused",
    [USED]     "used",
    [SLEEPING] "sleep",
    [RUNNABLE] "runnable",
    [RUNNING]  "running",
    [ZOMBIE]   "zombie"
  };

  if(state >= 0 && state < (int)(sizeof(states)/sizeof(states[0])) && states[state])
    return states[state];
  return "unknown";
}

static void
print_queue_stats(struct procinfo *info)
{
  printf("  runtime(total): %d ticks\n", (int)info->total_runtime);
  for(int i = 0; i < MLFQ_LEVELS; i++) {
    printf("    q%d: %d ticks\n", i, (int)info->queue_runtime[i]);
  }
}

int
main(int argc, char *argv[])
{
  struct procinfo info;
  int pid;

  if(argc > 2){
    fprintf(2, "usage: procinfo [pid]\n");
    exit(1);
  }

  if(argc == 2){
    pid = atoi(argv[1]);
  } else {
    pid = getpid();
  }

  if(getprocinfo(pid, &info) < 0){
    fprintf(2, "procinfo: unable to inspect pid %d\n", pid);
    exit(1);
  }

  printf("procinfo(pid=%d name=%s)\n", info.pid, info.name);
  printf("  state: %s\n", state_name(info.state));
  printf("  base_priority: %d current_level: %d budget: %d\n",
         info.base_priority, info.current_level, info.time_slice_budget);
  print_queue_stats(&info);

  exit(0);
}

