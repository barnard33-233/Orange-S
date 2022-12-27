#include "stdio.h"
#include "string.h"

void shellcode() {
    printf("you are pwned\n");
    int i = 10000;
    while (i--);
    exit(0);
}

void input() {
    char buf[8] = {
        1, 2, 3, 4, 5, 6, 7, 0
    };
    char payload[] = {
        // i don't know why this fxxking filler needs 16 bytes...
        // fill the buffer
        0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41,
        // fill old ebp
        0x41, 0x41, 0x41, 0x41,
        0x00, 0x00, 0x00, 0x00
    };
    int target = (int)&shellcode;
    memcpy(payload + sizeof(payload) - 4, &target, 4);
    memcpy(buf, payload, sizeof(payload));
    return;
}


int main(int argc, char** argv) {
    input();
    return 0;
}
