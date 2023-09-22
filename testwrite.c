#include "types.h"
#include "stat.h"
#include "user.h"


int
main(int argc, char *argv[])
{

  char *s = "hello";
  int temp = -1;

  setwritecount(0);
  printf(1, "%s","Druv likes to play with OS");

  temp = writecount();

  printf(1,"%d\n", temp);
  printf(1, "%s","Druv likes to play with OS");

  setwritecount(0);

  write(1, s, 5);

  temp = writecount();

  printf(1,"%d\n", temp);

  exit();
}
