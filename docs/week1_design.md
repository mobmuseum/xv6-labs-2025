# Week 1 – MLFQ Design & Setup

## Default xv6 scheduler recap
- xv6 currently keeps all runnable processes in a single implicit queue and schedules them round-robin with a fixed one-tick time slice.
- There is no notion of priority, accounting of per-queue runtime, or starvation prevention.
- The switcher simply scans the process table for `RUNNABLE` entries, which makes it easy to layer in a priority structure by reusing the same table but organizing runnable processes per level.

## Week 1 deliverables
1. **`getprocinfo` syscall** – implemented in `kernel/proc.c` / `kernel/sysproc.c` with a shared `struct procinfo` definition in `kernel/procinfo.h`.  
   - Provides: pid, state (`enum procstate`), base priority, current queue level, remaining budget, total runtime, and per-queue runtime counters.  
   - User tool `procinfo` (see `user/procinfo.c`) dumps the data to validate the syscall wiring before changing the scheduler.
2. **Queue scaffolding** – new `kernel/mlfq.h` describes shared constants, `struct proc` now records bookkeeping for MLFQ, and `proc.c` defines queue helpers plus initialization hooks.  
   - No behavior change yet; helpers are compiled in so Week 2 can start plugging them into `scheduler()`.
3. **Design doc (this file)** – documents the intended behavior so implementation can proceed incrementally.

## MLFQ configuration
- **Levels:** `MLFQ_LEVELS = 4` (0 = highest priority, 3 = lowest). `MLFQ_DEFAULT_LEVEL`/`PRIORITY` are zero.
- **Time quanta:** `{4, 8, 16, 32}` timer ticks per level; each lower-priority level doubles the slice length to amortize context switches for CPU-bound work without starving interactive jobs.
- **Demotion:** whenever a process consumes its full time slice without yielding or sleeping, it will be enqueued one level lower (capped at level 3) and its slice reset to that level’s quantum.
- **Promotion / boosting:** every 200 ticks (configurable constant to be added in Week 2) a global boost will move all runnable processes back to level 0 and reset their budgets. This prevents starvation for interactive or newly-arrived processes.
- **Yield / sleep handling:** voluntary yield or blocking on I/O preserves the current queue level but refreshes the remaining budget so interactive tasks retain high priority.
- **Queue data structure:** each level is a FIFO maintained via `struct procqueue { head, tail; }` with per-proc `mlfq_next` pointers. All queue operations live in `proc.c` so they can be shared between scheduler and bookkeeping routines.

## Testing & tooling plan
- **Syscall smoke test:** run `make qemu`, then from the shell execute `procinfo` (optionally pass a pid). Successful output shows current state/queue/budget fields.
- **Regression check:** `usertests` should continue to pass since scheduler semantics are unchanged in Week 1.
- **Future tests:** custom workloads (CPU bound loop, interactive shell spam, I/O waiters) will rely on `procinfo` plus trace prints to confirm demotion/promotion once the MLFQ is wired in.

## Next steps (Week 2 preview)
- Replace the scan-based scheduler with queue-aware selection.
- Charge ticks to the active process to update `total_runtime` / `queue_runtime`.
- Trigger demotions when `time_slice_budget` reaches zero and ensure `yield()`/`sleep()` reinserts processes at the head/tail per policy.
- Implement the periodic boost timer along with a potential `boostproc()` syscall for deterministic testing.

