

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int original;
  int result;

  // Check the default priority before making any changes.
  original = getpriority();
  printf("Initial priority: %d\n", original);

  if (original != 1) {
    printf("FAIL: expected initial priority 1\n");
    exit(1);
  }

  // Set priority to 0 (highest priority).
  result = setpriority(0);
  if (result == 0 && getpriority() == 0) {
    printf("PASS: set priority to 0\n");
  } else {
    printf("FAIL: set priority to 0\n");
    exit(1);
  }

  // A negative priority should be rejected.
  result = setpriority(-1);
  if (result == -1 && getpriority() == 0) {
    printf("PASS: negative priority rejected\n");
  } else {
    printf("FAIL: negative priority rejected\n");
    exit(1);
  }

  // A priority at or above NB_PRIORITY_LEVELS should be rejected.
  // This test assumes NB_PRIORITY_LEVELS is 3.
  result = setpriority(3);
  if (result == -1 && getpriority() == 0) {
    printf("PASS: invalid high priority rejected\n");
  } else {
    printf("FAIL: invalid high priority rejected\n");
    exit(1);
  }

  // Setting the current priority should succeed without changing it.
  result = setpriority(0);
  if (result == 0 && getpriority() == 0) {
    printf("PASS: setting the same priority\n");
  } else {
    printf("FAIL: setting the same priority\n");
    exit(1);
  }

  // Restore the original priority.
  result = setpriority(original);
  if (result == 0 && getpriority() == original) {
    printf("PASS: restored original priority\n");
  } else {
    printf("FAIL: restored original priority\n");
    exit(1);
  }

  exit(0);
}
