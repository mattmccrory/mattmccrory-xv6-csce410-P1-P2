
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int low_pid;
  int high_pid;
  int first_pid;
  int status;

  printf("Starvation prevention test starting\n");

  // Create the low-priority process first.
  low_pid = fork();

  if (low_pid < 0) {
    printf("FAIL: could not fork low-priority child\n");
    exit(1);
  }

  if (low_pid == 0) {
    volatile int i;

    if (setpriority(2) < 0) {
      printf("FAIL: low-priority child could not set priority\n");
      exit(1);
    }

    // Finish a moderate amount of CPU work.
    for (i = 0; i < 1000000; i++)
      ;

    printf("LOW: completed CPU work at priority %d\n",
           getpriority());
    exit(0);
  }

  // Create a competing high-priority process.
  high_pid = fork();

  if (high_pid < 0) {
    printf("FAIL: could not fork high-priority child\n");
    kill(low_pid);
    wait(0);
    exit(1);
  }

  if (high_pid == 0) {
    volatile int i;

    if (setpriority(0) < 0) {
      printf("FAIL: high-priority child could not set priority\n");
      exit(1);
    }

    // Keep high-priority work runnable much longer.
    for (i = 0; i < 100000000; i++)
      ;

    printf("HIGH: completed CPU work at priority %d\n",
           getpriority());
    exit(0);
  }

  // The low-priority child should finish before the
  // much longer-running high-priority child.
  first_pid = wait(&status);

  if (first_pid == low_pid && status == 0) {
    printf("PASS: low-priority process completed first\n");
  } else {
    printf("FAIL: low-priority process did not finish first\n");

    // Reap the other child before exiting.
    wait(0);
    exit(1);
  }

  // Reap the remaining child.
  if (wait(&status) != high_pid || status != 0) {
    printf("FAIL: high-priority child did not exit successfully\n");
    exit(1);
  }

  printf("PASS: both children completed\n");
  exit(0);
}
