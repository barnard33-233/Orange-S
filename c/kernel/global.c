/*************************************************************************//**
 *****************************************************************************
 * @file   global.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   2005
 *****************************************************************************
 *****************************************************************************/

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "fs.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"

int	ticks;

int	disp_pos;

u8			gdt_ptr[6];	/* 0~15:Limit  16~47:Base */
struct descriptor	gdt[GDT_SIZE];
u8			idt_ptr[6];	/* 0~15:Limit  16~47:Base */
struct gate		idt[IDT_SIZE];

u32	k_reenter;
int	current_console;

int	key_pressed; /**
			      * used for clock_handler
			      * to wake up TASK_TTY when
			      * a key is pressed
			      */

struct tss	tss;
struct proc*	p_proc_ready;

MESSAGE			mm_msg;
int			memory_size;

/* FS */
struct file_desc	f_desc_table[NR_FILE_DESC];
struct inode		inode_table[NR_INODE];
struct super_block	super_block[NR_SUPER_BLOCK];
MESSAGE			fs_msg;
struct proc *		pcaller;
struct inode *		root_inode;

PUBLIC	struct proc proc_table[NR_TASKS + NR_PROCS];

/* 注意下面的 TASK 的顺序要与 const.h 中对应 */
PUBLIC	struct task	task_table[NR_TASKS] = {
	/* entry        stack size        task name */
	/* -----        ----------        --------- */
	{task_tty,      STACK_SIZE_TTY,   "TTY"       },
	{task_sys,      STACK_SIZE_SYS,   "SYS"       },
	{task_hd,       STACK_SIZE_HD,    "HD"        },
	{task_fs,       STACK_SIZE_FS,    "FS"        },
	{task_mm,       STACK_SIZE_MM,    "MM"        }};

PUBLIC	struct task	user_proc_table[NR_NATIVE_PROCS] = {
	/* entry    stack size     proc name */
	/* -----    ----------     --------- */
	{Init,   STACK_SIZE_INIT,  "INIT" },
	{TestA,  STACK_SIZE_TESTA, "TestA"},
	{TestB,  STACK_SIZE_TESTB, "TestB"},
	{TestC,  STACK_SIZE_TESTC, "TestC"}};
/* PUBLIC	struct task	user_proc_table[NR_PROCS] = { */
/* 	{TestA, STACK_SIZE_TESTA, "TestA"}, */
/* 	{TestB, STACK_SIZE_TESTB, "TestB"}, */
/* 	{TestC, STACK_SIZE_TESTC, "TestC"}}; */

PUBLIC	char		task_stack[STACK_SIZE_TOTAL];

PUBLIC	TTY		tty_table[NR_CONSOLES];
PUBLIC	CONSOLE		console_table[NR_CONSOLES];

PUBLIC	irq_handler	irq_table[NR_IRQ];

PUBLIC	system_call	sys_call_table[NR_SYS_CALL] = {sys_printx,
						       sys_sendrec};

/* FS related below */
/*****************************************************************************/
/**
 * For dd_map[k],
 * `k' is the device nr.\ dd_map[k].driver_nr is the driver nr.
 *
 * Remeber to modify include/const.h if the order is changed.
 *****************************************************************************/
struct dev_drv_map dd_map[] = {
	/* driver nr.		major device nr.
	   ----------		---------------- */
	{INVALID_DRIVER},	/**< 0 : Unused */
	{INVALID_DRIVER},	/**< 1 : Reserved for floppy driver */
	{INVALID_DRIVER},	/**< 2 : Reserved for cdrom driver */
	{TASK_HD},		/**< 3 : Hard disk */
	{TASK_TTY},		/**< 4 : TTY */
	{INVALID_DRIVER}	/**< 5 : Reserved for scsi disk driver */
};

/**
 * 6MB~7MB: buffer for FS
 */
PUBLIC	u8 *		fsbuf		= (u8*)0x600000;
PUBLIC	const int	FSBUF_SIZE	= 0x100000;


/**
 * 7MB~8MB: buffer for MM
 */
PUBLIC	u8 *		mmbuf		= (u8*)0x700000;
PUBLIC	const int	MMBUF_SIZE	= 0x100000;


/**
 * 8MB~10MB: buffer for log (debug)
 */
PUBLIC	char *		logbuf		= (char*)0x800000;
PUBLIC	const int	LOGBUF_SIZE	= 0x100000;
PUBLIC	char *		logdiskbuf	= (char*)0x900000;
PUBLIC	const int	LOGDISKBUF_SIZE	= 0x100000;

PUBLIC char * free_mem_area = (char*)0xa00000 + NR_PROCS * PROC_IMAGE_SIZE_DEFAULT;
PUBLIC int FREE_MEM_SIZE = 32 * 0x1000;