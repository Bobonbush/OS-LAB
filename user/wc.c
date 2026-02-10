#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];
int print_lines = 0;
int print_words = 0;
int print_chars = 0;

void
wc(int fd, char *name)
{
  int i, n;
  int l, w, c, inword;

  l = w = c = 0;
  inword = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      c++;
      if(buf[i] == '\n')
        l++;
      if(strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if(!inword){
        w++;
        inword = 1;
      }
    }
  }
  if(n < 0){
    printf("wc: read error\n");
    exit(1);
  }
  if(print_lines) printf("%d ", l);
  if(print_words) printf("%d ", w);
  if(print_chars) printf("%d ", c);
  printf("%s\n", name);
}

int
main(int argc, char *argv[])
{
  int fd, i;
  int index = 1;
  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-') {
      for(int j = 1 ; argv[i][j] != 0; j++) {
         if(argv[i][j] == 'l')  print_lines = 1;
         else if(argv[i][j] == 'w') print_words = 1;
         else if(argv[i][j] == 'c') print_chars = 1;
         else {
            printf("wc: invalid option -- '%c' \n", argv[i][j]);
            exit(1);
         }
      }
    }else {
        index = i;
        break;
    }
  }

  if (!print_lines && !print_words && !print_chars) {
    print_lines = print_words = print_chars = 1;
  }

  if(i >= argc) {
    wc(0, "");
    exit(0); 
  }

  for(i = index; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }
    wc(fd, argv[i]);
    close(fd);
  }
  exit(0);
}
