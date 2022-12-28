#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void main() {
    char *const paramList[] = {"/bin/ls", "-l", NULL};
    execv("/bin/ls", paramList);
    fork();
    fork();
    printf("fork\n");
}

