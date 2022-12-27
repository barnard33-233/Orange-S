/*************************************************************************/ /**
 *****************************************************************************
 * @file   lib/exec.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Tue May  6 14:26:09 2008
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

/*****************************************************************************
 *                                exec
 *****************************************************************************/
/**
 * Executes the program pointed by path.
 * 
 * @param path  The full path of the file to be executed.
 * 
 * @return  Zero if successful, otherwise -1.
 *****************************************************************************/
PUBLIC int exec(const char *path)
{
  MESSAGE msg;
  msg.type = EXEC;
  msg.PATHNAME = (void *)path;
  msg.NAME_LEN = strlen(path);
  msg.BUF = 0;
  msg.BUF_LEN = 0;

  send_recv(BOTH, TASK_MM, &msg);
  assert(msg.type == SYSCALL_RET);

  return msg.RETVAL;
}

/*****************************************************************************
 *                                execl
 *****************************************************************************/
PUBLIC int execl(const char *path, const char *arg, ...)
{
  va_list parg = (va_list)(&arg);
  char **p = (char **)parg;
  return execv(path, p);
}

/*****************************************************************************
 *                                execv
 *****************************************************************************/
PUBLIC int execv(const char *path, char *argv[])
{
  int i, num = 0;
  /* 遍历check[],找到文件名相同 */
  for (i = 1; i <= file_cnt; i++)
    {
      if (strcmp(path, check[i].name) == 0)
        {
          printf("\nFile name: %s (Size = %d Bytes, xor value = %d\n", check[i].name, check[i].size,
                 check[i].key);
          num = i;
        }
    }
  int n = check[num].size;
  int key = 0;
  int fd = open(path, O_RDWR);
  char buf[512];

  while (n > 0)
    {
      int chunk;
      chunk = min(n, 512);
      read(fd, buf, chunk);
      for (i = 0; i < chunk; i++)
        {
          key ^= buf[i];
        }
      n -= chunk;
    }
  printf("now xor value: %d\n", key);
  printf("initial xor value: %d\n", check[num].key);
  if (key != check[num].key)
    {
      printf("xor value changed! open file %s denied\n", check[num].name);
      exit(1);
    }
  printf("xor value correct!\n");
  close(fd);

  char **p = argv;
  char arg_stack[PROC_ORIGIN_STACK];
  int stack_len = 0;

  while (*p++)
    {
      assert(stack_len + 2 * sizeof(char *) < PROC_ORIGIN_STACK);
      stack_len += sizeof(char *);
    }

  *((int *)(&arg_stack[stack_len])) = 0;
  stack_len += sizeof(char *);

  char **q = (char **)arg_stack;
  for (p = argv; *p != 0; p++)
    {
      *q++ = &arg_stack[stack_len];

      assert(stack_len + strlen(*p) + 1 < PROC_ORIGIN_STACK);
      strcpy(&arg_stack[stack_len], *p);
      stack_len += strlen(*p);
      arg_stack[stack_len] = 0;
      stack_len++;
    }

  MESSAGE msg;
  msg.type = EXEC;
  msg.PATHNAME = (void *)path;
  msg.NAME_LEN = strlen(path);
  msg.BUF = (void *)arg_stack;
  msg.BUF_LEN = stack_len;

  send_recv(BOTH, TASK_MM, &msg);
  assert(msg.type == SYSCALL_RET);

  return msg.RETVAL;
}
