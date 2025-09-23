#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXBUF 512

void memdump(char *fmt, char *data);

int
main(int argc, char *argv[])
{
  char buf[MAXBUF];
  int n;

  if(argc == 1){
    // Run built-in examples if no argument is provided
    printf("Example 1:\n");
    int a[2] = {61810, 2025};
    memdump("ii", (char*)a);

    printf("Example 2:\n");
    memdump("S", "a string");

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char*)&s);

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char*)&example);

    printf("Example 5:\n");
    memdump("sccccc", (char*)&example);

    exit(0);
  }

  // If there is an argument, treat it as format string
  char *fmt = argv[1];
  n = read(0, buf, sizeof(buf));
  if(n < 0){
    fprintf(2, "memdump: read error\n");
    exit(1);
  }
  memdump(fmt, buf);
  exit(0);
}

void
memdump(char *fmt, char *data)
{
  while(*fmt){
    switch(*fmt){
      case 'i': { // 32-bit integer
        int val = *(int*)data;
        printf("%d\n", val);
        data += sizeof(int);
        break;
      }
      case 'p': { // 64-bit integer (print as hex)
        uint64 val = *(uint64*)data;
        printf("%lx\n", val);
        data += sizeof(uint64);
        break;
      }
      case 'h': { // 16-bit integer
        short val = *(short*)data;
        printf("%d\n", val);
        data += sizeof(short);
        break;
      }
      case 'c': { // character
        char val = *data;
        printf("%c\n", val);
        data += sizeof(char);
        break;
      }
      case 's': { // pointer to string
        char *ptr = *(char**)data;
        if(ptr)
          printf("%s\n", ptr);
        else
          printf("(null)\n");
        data += sizeof(char*);
        break;
      }
      case 'S': { // inline null-terminated string
        char *str = (char*)data;
        printf("%s\n", str);
        data += strlen(str) + 1;
        break;
      }
      default:
        printf("Unknown format: %c\n", *fmt);
        break;
    }
    fmt++;
  }
}
