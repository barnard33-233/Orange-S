#include "stdio.h"
#include "type.h"

int main(int argc, char **argv)
{
  char *address1;
  char *address2;

  printf("mm_test\n");

  address1 = (char *)alloc();
  printf("address1=%x\n", address1);

  address2 = (char *)alloc();
  printf("address2=%x\n", address2);

  free(address1);
  address1 = (char *)0;

  address1 = alloc();
  printf("address1=%x\n", address1);

  free(address1);
  free(address2);
  return 0;
}