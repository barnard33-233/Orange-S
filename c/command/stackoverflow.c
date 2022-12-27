/*
 * a demo of overflow
 */
#include "stdio.h"

void target()
{
  printf("returned to target\n");
  exit(0);
}

void function()
{
  char *buffer[1];
  buffer[2] = &target;
  return;
}

int main()
{
  printf("overflow test:\n");
  function();
  return 0;
}