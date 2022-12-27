/*************************************************************************/ /**
 *****************************************************************************
 * @file   mm/main.c
 * @brief  Orange'S Memory Management.
 * @author Forrest Y. Yu
 * @date   Tue May  6 00:33:39 2008
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "config.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

PUBLIC void do_fork_test();

PRIVATE void init_mm();

PRIVATE char *mm_alloc_page(u32 *);
PRIVATE void mm_free_page(u32 *, char *);
/*****************************************************************************
 *                                task_mm
 *****************************************************************************/
/**
 * <Ring 1> The main loop of TASK MM.
 * 
 *****************************************************************************/
PUBLIC void task_mm()
{
  u32 bitmap = 0;
  init_mm();

  while (1)
    {
      send_recv(RECEIVE, ANY, &mm_msg);
      int src = mm_msg.source;
      int reply = 1;

      int msgtype = mm_msg.type;

      switch (msgtype)
        {
        case FORK:
          mm_msg.RETVAL = do_fork();
          break;
        case EXIT:
          do_exit(mm_msg.STATUS);
          reply = 0;
          break;
        case EXEC:
          mm_msg.RETVAL = do_exec();
          break;
        case WAIT:
          do_wait();
          reply = 0;
          break;
        case MM_ALLOC:
          mm_msg.ADDRESS = mm_alloc_page(&bitmap);
          break;
        case MM_FREE:
          mm_free_page(&bitmap, mm_msg.ADDRESS);
          break;
        default:
          dump_msg("MM::unknown msg", &mm_msg);
          assert(0);
          break;
        }

      if (reply)
        {
          mm_msg.type = SYSCALL_RET;
          send_recv(SEND, src, &mm_msg);
        }
    }
}

/*****************************************************************************
 *                                init_mm
 *****************************************************************************/
/**
 * Do some initialization work.
 * 
 *****************************************************************************/
PRIVATE void init_mm()
{
  struct boot_params bp;
  get_boot_params(&bp);

  memory_size = bp.mem_size;

  /* print memory size */
  printl("{MM} memsize:%dMB\n", memory_size / (1024 * 1024));
}

/*
 * a demo of allocate page.
 * only works in ring 1.
*/
PRIVATE char *mm_alloc_page(u32 *bitmap)
{
  if (*bitmap == 0xffffffff)
    {
      return NULL;
    }
  // find the first free slot:
  u32 mask = 1;
  int i;
  for (i = 0; i < 32; i++)
    {
      mask << 1;
      if (!(mask & *bitmap))
        { // if selected bit == 1
          break;
        }
    }
  char *address = free_mem_area + i * PAGE_SIZE;
  *bitmap |= mask;
  return address;
}

PRIVATE void mm_free_page(u32 *bitmap, char *address)
{
  int position = (address - free_mem_area) / PAGE_SIZE;
  if (position >= 0 && position < 32)
    {
      u32 mask = ~(1 << position);
      *bitmap &= mask;
    }
}

/*****************************************************************************
 *                                alloc_mem
 *****************************************************************************/
/**
 * Allocate a memory block for a proc.
 * 
 * @param pid  Which proc the memory is for.
 * @param memsize  How many bytes is needed.
 * 
 * @return  The base of the memory just allocated.
 *****************************************************************************/
PUBLIC int alloc_mem(int pid, int memsize)
{
  assert(pid >= (NR_TASKS + NR_NATIVE_PROCS));
  if (memsize > PROC_IMAGE_SIZE_DEFAULT)
    {
      panic("unsupported memory request: %d. "
            "(should be less than %d)",
            memsize, PROC_IMAGE_SIZE_DEFAULT);
    }

  int base = PROCS_BASE + (pid - (NR_TASKS + NR_NATIVE_PROCS)) * PROC_IMAGE_SIZE_DEFAULT;

  if (base + memsize >= memory_size) panic("memory allocation failed. pid:%d", pid);

  return base;
}

/*****************************************************************************
 *                                free_mem
 *****************************************************************************/
/**
 * Free a memory block. Because a memory block is corresponding with a PID, so
 * we don't need to really `free' anything. In another word, a memory block is
 * dedicated to one and only one PID, no matter what proc actually uses this
 * PID.
 * 
 * @param pid  Whose memory is to be freed.
 * 
 * @return  Zero if success.
 *****************************************************************************/
PUBLIC int free_mem(int pid)
{
  return 0;
}
