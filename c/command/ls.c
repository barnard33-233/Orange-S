#include "stdio.h"

int main(int args, char* argv[]) {
    char* result;
    result = list_file("/");
    printf("%s\n", result);
    return 0;
}
