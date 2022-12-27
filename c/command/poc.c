#include "stdio.h"
#include "string.h"

#define DEBUG

// to fix some amazing bug:
// but fail
#ifdef DEBUG
#define breakpoint __asm__ __volatile__("xchg %bx, %bx")
#else
#define breakpoint __asm__ __volatile__("xchg %ax, %ax")
#endif

void input() {
    int i = 0x11223344;
    char buf[8] = "1234567";
    breakpoint;
    // *buf = 'A';
    // printf("%d", buf);
    char payload[] = {
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0xb5, 0x10, 0x00};  // printf("%s\n", buf);
    strcpy(buf, payload);
    printf("%s", buf);
    breakpoint;
    return;
}

void shellcode() {
    printf("you are pwned\n");
    int i = 10000;
    while (i--)
        ;
    exit(0);
}

int main(int argc, char** argv) {
    breakpoint;
    // shellcode();
    input();
    return 0;
}
