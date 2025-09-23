#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // Check number of arguments
    if (argc != 2) {
        fprintf(2, "Usage: sleep <ticks>\n");
        exit(1);
    }

    // Convert argument string to integer
    int ticks = atoi(argv[1]);

    if (ticks < 0) {
        fprintf(2, "sleep: ticks must be non-negative\n");
        exit(1);
    }

    // Call system sleep (pauses for ticks clock interrupts)
    pause(ticks);

    // Exit with success
    exit(0);
}
