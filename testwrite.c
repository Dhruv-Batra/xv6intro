#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{

  char *s = "hello";
  int temp = -1;

  setwritecount(0);

  temp = writecount();

  printf(1,"%d\n", temp);

  setwritecount(0);

  write(1, s, 5);

  temp = writecount();

  printf(1,"%d\n", temp);

  printf(1, "%s","This is a test - Hello ");

  temp = writecount();

  printf(1,"%d\n", temp);

  exit();

  
}
