#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// characters that separate numbers
char *seps = " -\r\t\n./,";

// check if c is a separator
int issep(char c) {
  return (strchr(seps, c) != 0);
}

void process_file(char *filename) {
  int fd = open(filename, O_RDONLY);
  if(fd < 0){
    fprintf(2, "sixfive: cannot open %s\n", filename);
    return;
  }

  char buf[1];
  char numbuf[64];   // buffer to hold one number as string
  int idx = 0;

  while(read(fd, buf, 1) == 1){
    char c = buf[0];
    if(issep(c)){
      if(idx > 0){ // end of a number
        numbuf[idx] = '\0';
        int val = atoi(numbuf);
        if(val % 5 == 0 || val % 6 == 0){
          printf("%d\n", val);
        }
        idx = 0;
      }
    } else {
      if(idx < sizeof(numbuf)-1){
        numbuf[idx++] = c;
      }
    }
  }

  // handle last number if file doesn't end with a separator
  if(idx > 0){
    numbuf[idx] = '\0';
    int val = atoi(numbuf);
    if(val % 5 == 0 || val % 6 == 0){
      printf("%d\n", val);
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: sixfive files...\n");
    exit(1);
  }

  for(int i = 1; i < argc; i++){
    process_file(argv[i]);
  }

  exit(0);
}
