#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main(){

    int arraySize = 5;

    int *ptr = mmap(
        NULL,
        arraySize * sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        0,0
    );

    if (ptr == MAP_FAILED) {
        printf("Could not map file\n");
        return 1;
    }

    for (int i = 0; i < arraySize; i++) {
        ptr[i] = i + 1;
    }

    printf("Initial array: ");
    for (int i = 0; i < arraySize; i++ ){
        printf("%d ", ptr[i] );
    }
    printf("\n");

    pid_t child_pid = fork();

    if (child_pid == 0) {
        //child
        for (int i = 0; i < N; i++){
            ptr[i] = ptr[i] * 10;
        }
    } else{
        //parent
        waitpid(child_pid, NULL, 0);
        printf("\nParent:\n");

        printf("Updated array: ");
        for (int i = 0; i < arraySize; i++){
            printf("%d ", ptr[i] );
        }
        printf("\n");
    }

    int err = munmap(ptr, arraySize * sizeof(int));

    if (err != 0) {
        printf("Could not unmap file\n");
        return 1;
    }
    return 0;
}