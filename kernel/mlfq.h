#ifndef MLFQ_H
#define MLFQ_H

// Number of scheduler priority levels.
#define MLFQ_LEVELS 4

// Default level assigned to brand new processes.
#define MLFQ_DEFAULT_LEVEL 0

// Default base priority used when boosting or resetting a process.
#define MLFQ_DEFAULT_PRIORITY 0

// Number of ticks between global priority boosts.
#define MLFQ_BOOST_INTERVAL 200

// Maximum length (including trailing nul) for process names mirrored in user space.
#define PROCINFO_NAME_MAX 16

#endif // MLFQ_H

