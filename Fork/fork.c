#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) is running\n", getpid());
        sleep(2);
        printf("Child process (PID: %d) is exiting\n", getpid());
        exit(0);
    } else {
        // Parent process
        printf("Parent process (PID: %d) created child (PID: %d)\n", getpid(), pid);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
