#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];
int lines = 0;
int
readline(int fd, char *buf, int maxlen)
{
  int n;
  char c;
  int i = 0;

  /* Read one character at a time from fd */
  while((n = read(fd, &c, 1)) > 0){
    buf[i] = c;
    /* Look for the newline character */
    if (c == '\n'){
      /* We are at the end of the line, so stop reading */
      break;
    }
    i += 1;
    /* We don't want to read more characters than we have room */
    if(i >= (maxlen - 1)){
      /* We can't recover, so just print a message and exit */
      fprintf(2, "readline() - line too long\n");
      exit(-1);
    }
  }
  /* This is a little tricky. If read() returns 0 AND we didn't
     read previous characters for this line, then we want to return 0.
     Also, if read returns a value less than 0, we want to return this
     error condition. */
  if(((n == 0) && (i == 0)) || (n < 0))
    return n;

  /* Add the null terminator to the end for the string buffer */
  i += 1;
  buf[i] = '\0';
  return i;
}

void
cat(int fd)
{
  int n;
  while((n = readline(fd, buf, sizeof(buf))) > 0) {
    if(lines) {
      if (lines < 10) printf("     ");
      else if (lines < 100) printf("    ");
      else if (lines < 1000) printf("   ");
      else if (lines < 10000) printf("  ");
      else if (lines < 100000) printf(" ");
      printf("%d", lines);
      lines++;
      printf("  ");
    }
    if (write(1, buf, n) != n) {
      fprintf(2, "cat: write error\n");
      exit(1);
    }
    
  }
  if(n < 0){
    fprintf(2, "cat: read error\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;
  int index = 0;
  for(i = 1 ; i < argc; i++ ) {
     if(argv[i][0] =='-') {
        for(int j = 1 ; argv[i][j] != 0; j++) {
             if(argv[i][j] == 'n') lines = 1;
        }
     }else {
         index = i;
         break;
     }
  }

  if(argc <= i){
    cat(0);
    exit(0);
  }

  for(i = index; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "cat: cannot open %s\n", argv[i]);
      exit(1);
    }
    cat(fd);
    close(fd);
  }
  exit(0);
}
