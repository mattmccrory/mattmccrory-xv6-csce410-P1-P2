
#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int original = getpriority();
  printf("Initial priority: %d\n", original);

  if (setpriority(0) != 0 || getpriority() != 0) {
    printf("FAIL: setting priority to 0\n");
    exit(1);
  }
  printf("PASS: set priority to 0\n");

  if (setpriority(-1) != -1 || getpriority() != 0) {
    printf("FAIL: invalid negative priority\n");
    exit(1);
  }
  printf("PASS: negative priority rejected\n");

  if (setpriority(3) != -1 ||
      getpriority() != 0) {
    printf("FAIL: invalid high priority\n");
    exit(1);
  }
  printf("PASS: invalid high priority rejected\n");

  if (setpriority(0) != 0 || getpriority() != 0) {
    printf("FAIL: setting the same priority\n");
    exit(1);
  }
  printf("PASS: setting the same priority\n");

  if (setpriority(original) != 0 || getpriority() != original) {
    printf("FAIL: restoring original priority\n");
    exit(1);
  }
  printf("PASS: restored original priority\n");

  exit(0);
}
