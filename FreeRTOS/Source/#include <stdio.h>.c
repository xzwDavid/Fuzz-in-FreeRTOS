#include <stdio.h>
#include <pthread.h>

int main() {
    printf("PTHREAD_STACK_MIN is %zu bytes\n", PTHREAD_STACK_MIN);
    return 0;
}
