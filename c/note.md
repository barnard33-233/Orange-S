# Note

## 格式变化

1. .clang-format
2. 将 .h 文件中的变量定义改到对应 .c 文件中，使它符合规范（并且不被编辑器使劲报错）

## 内存分配与释放

加到 mm 模块里。管理的是分配给进程的内存上面的 32*4096 byte 大小的内存。这块内存相关的变量在 kernel/global.c

服务加在 mm/main.c ，对外接口 alloc 和 free 追加在 lib/fork.c 里面（不想改Makefile了）

+ `void * alloc(void)`
+ `void free(void * address)`

测试程序在 command/mm_test.c

分配依赖 bitmap ，测试内容是发 33 个 alloc，然后 free 33次

