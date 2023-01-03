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

char op[5] = {
    '#', '+', '-', '*', '/',
};
float cal(float x, float y, int op)
{
  switch (op)
    {
    case 1:
      return x + y;
    case 2:
      return x - y;
    case 3:
      return x * y;
    case 4:
      return x / y;
    }
}

float calculate_model1(float i, float j, float k, float t, int op1, int op2, int op3)
{
  float r1, r2, r3;
  r1 = cal(i, j, op1);
  r2 = cal(r1, k, op2);
  r3 = cal(r2, t, op3);
  return r3;
}

float calculate_model2(float i, float j, float k, float t, int op1, int op2, int op3)
{
  float r1, r2, r3;
  r1 = cal(j, k, op2);
  r2 = cal(i, r1, op1);
  r3 = cal(r2, t, op3);
  return r3;
}

float calculate_model3(float i, float j, float k, float t, int op1, int op2, int op3)
{
  float r1, r2, r3;
  r1 = cal(k, t, op3);
  r2 = cal(j, r1, op2);
  r3 = cal(i, r2, op1);
  return r3;
}

float calculate_model4(float i, float j, float k, float t, int op1, int op2, int op3)
{
  float r1, r2, r3;
  r1 = cal(j, k, op2);
  r2 = cal(r1, t, op3);
  r3 = cal(i, r2, op1);
  return r3;
}

float calculate_model5(float i, float j, float k, float t, int op1, int op2, int op3)
{
  float r1, r2, r3;
  r1 = cal(i, j, op1);
  r2 = cal(k, t, op3);
  r3 = cal(r1, r2, op2);
  return r3;
}

int get24(int i, int j, int k, int t)
{
  int op1, op2, op3;
  int flag = 0;
  for (op1 = 1; op1 <= 4; op1++)
    for (op2 = 1; op2 <= 4; op2++)
      for (op3 = 1; op3 <= 4; op3++)
        {
          if (calculate_model1(i, j, k, t, op1, op2, op3) == 24)
            {
              printf("((%d%c%d)%c%d)%c%d=24\n", i, op[op1], j, op[op2], k, op[op3], t);
              flag = 1;
            }
          if (calculate_model2(i, j, k, t, op1, op2, op3) == 24)
            {
              printf("(%d%c(%d%c%d))%c%d=24\n", i, op[op1], j, op[op2], k, op[op3], t);
              flag = 1;
            }
          if (calculate_model3(i, j, k, t, op1, op2, op3) == 24)
            {
              printf("%d%c(%d%c(%d%c%d))=24\n", i, op[op1], j, op[op2], k, op[op3], t);
              flag = 1;
            }
          if (calculate_model4(i, j, k, t, op1, op2, op3) == 24)
            {
              printf("%d%c((%d%c%d)%c%d)=24\n", i, op[op1], j, op[op2], k, op[op3], t);
              flag = 1;
            }
          if (calculate_model5(i, j, k, t, op1, op2, op3) == 24)
            {
              printf("(%d%c%d)%c(%d%c%d)=24\n", i, op[op1], j, op[op2], k, op[op3], t);
              flag = 1;
            }
        }
  return flag;
}

int main(int agrc, char *agrv[])
{
  char s[10];
  int len, n[4], i;
  for (i = 0; i < 4; i++)
    {
      printf("input %dth num:", i + 1);
      len = read(0, s, 10);
      n[i] = char2int(s, len);
    }
  printf("Your input four integer:%d %d %d %d\n", n[0], n[1], n[2], n[3]);
  if (n[0] < 1 || n[0] > 13 || n[1] < 1 || n[1] > 13 || n[2] < 1 || n[2] > 13 || n[3] < 1 ||
      n[3] > 13)
    {
      printf("Input illege, Please input again\n");
    }
  if (n[0] == n[1] || n[0] == n[2] || n[0] == n[3] || n[1] == n[2] || n[1] == n[3] || n[2] == n[3])
    {
      printf("Input wrong, Please input again\n");
    }
  if (get24(n[0], n[1], n[2], n[3]))
    ;
  else
    {
      printf("Sorry, the four integer cannot be calculated to get 24\n");
    }
  return 0;
}
