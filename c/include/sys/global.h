
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* global variables declared here */
#ifndef _ORANGES_GLOBAL_H_
#define _ORANGES_GLOBAL_H_

extern	int	ticks;

extern	int	disp_pos;

extern	u8			gdt_ptr[6];	/* 0~15:Limit  16~47:Base */
extern	struct descriptor	gdt[GDT_SIZE];
extern	u8			idt_ptr[6];	/* 0~15:Limit  16~47:Base */
extern	struct gate		idt[IDT_SIZE];

extern	u32	k_reenter;
extern	int	current_console;

extern	int	key_pressed; /**
			      * used for clock_handler
			      * to wake up TASK_TTY when
			      * a key is pressed
			      */

extern	struct tss	tss;
extern	struct proc*	p_proc_ready;

extern	char		task_stack[];
extern	struct proc	proc_table[];
extern  struct task	task_table[];
extern  struct task	user_proc_table[];
extern	irq_handler	irq_table[];
extern	TTY		tty_table[];
extern  CONSOLE		console_table[];

/* MM */
extern	MESSAGE			mm_msg;
extern	u8 *			mmbuf;
extern	const int		MMBUF_SIZE;
extern	int			memory_size;

/* FS */
extern	struct file_desc	f_desc_table[NR_FILE_DESC];
extern	struct inode		inode_table[NR_INODE];
extern	struct super_block	super_block[NR_SUPER_BLOCK];
extern	u8 *			fsbuf;
extern	const int		FSBUF_SIZE;
extern	MESSAGE			fs_msg;
extern	struct proc *		pcaller;
extern	struct inode *		root_inode;
extern	struct dev_drv_map	dd_map[];

/* for test only */
extern	char *			logbuf;
extern	const int		LOGBUF_SIZE;
extern	char *			logdiskbuf;
extern	const int		LOGDISKBUF_SIZE;
extern char * free_mem_area;
extern int FREE_MEM_SIZE;

#endif