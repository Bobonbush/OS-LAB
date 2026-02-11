#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/getproc.h"
#include "user/user.h"

// State names for display
char *states[] = {
  [UNUSED]    "unused",
  [USED]      "used",
  [SLEEPING]  "sleep",
  [RUNNABLE]  "runble",
  [RUNNING]   "run",
  [ZOMBIE]    "zombie"
};

int
main(int argc, char *argv[])
{
  struct procinfo procs[64];
  int n;

  // Call the system call to get process information
  n = getprocs(procs);
  
  if(n < 0){
    fprintf(2, "ps: getprocs failed\n");
    exit(1);
  }

  // Print header
  printf("PID  STATE   SIZE     NAME\n");

  // Print each process
  for(int i = 0; i < n; i++){
    char *state;
    if(procs[i].state >= 0 && procs[i].state < 6)
      state = states[procs[i].state];
    else
      state = "???";
    
    // Manual padding for aligned columns
    printf("%d", procs[i].pid);
    if(procs[i].pid < 10) printf("    ");
    else if(procs[i].pid < 100) printf("   ");
    else if(procs[i].pid < 1000) printf("  ");
    else printf(" ");
    
    printf("%s", state);
    int state_len = strlen(state);
    for(int j = state_len; j < 8; j++) printf(" ");
    
    printf("%lu", procs[i].sz);
    // Assuming size won't exceed 99999
    if(procs[i].sz < 10) printf("        ");
    else if(procs[i].sz < 100) printf("       ");
    else if(procs[i].sz < 1000) printf("      ");
    else if(procs[i].sz < 10000) printf("     ");
    else if(procs[i].sz < 100000) printf("    ");
    else printf(" ");
    
    printf("%s\n", procs[i].name);
  }

  exit(0);
}
