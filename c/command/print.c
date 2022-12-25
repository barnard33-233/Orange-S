#include "stdio.h"

int length(char *s)
{
  int len = 0;
  while (s[len] != '\0')
  {
    len++;
  }
  return len;
}

int char2int(char *s, int len)
{
  int num = 0, i, place = 1;
  for (i = len - 1; i >= 0; i--)
  {
    num += (s[i] - '0') * place;
    place *= 10;
  }
  return num;
}

int main(int agrc, char *agrv[]){
    int i,num;
    if(agrc>3) 
    {
	printf("ERROR:agrc out of range!\n");
	return 0;
    }
    num=char2int(agrv[agrc-1],length(agrv[agrc-1]));
    for (i = 0; i<num ;i++){
        printf("%s",agrv[agrc-2]);
    }
    printf("\nthe print of %s * %s finished! \n",agrv[agrc-1],agrv[agrc-2]);
    return 0;
}
