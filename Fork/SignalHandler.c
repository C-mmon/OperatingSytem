#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void sigchld_handler(int signo) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Child process %d terminated\n", pid);
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    for (int i = 0; i < 5; i++) {
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
            printf("Parent process created child (PID: %d)\n", pid);
        }
    }

    // Parent continues to run
    for (int i = 0; i < 10; i++) {
        printf("Parent process is working...\n");
        sleep(1);
    }

    return 0;
}
