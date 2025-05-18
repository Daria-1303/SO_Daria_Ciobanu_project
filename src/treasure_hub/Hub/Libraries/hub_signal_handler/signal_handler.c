#include "signal_handler.h"

void on_sigchld(int signo) {
    int status;
    pid_t pid = wait(&status);

    if (pid == hub_monitor.monitor_pid) {
        printf("Monitor exited with status %d\n", WEXITSTATUS(status));
        hub_monitor.monitor_status = OFF;
    }

}

void handle_sigchld() {
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = on_sigchld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    
    sigaction(SIGCHLD, &sa, NULL);

    // the end of a pipe
    if(fd_for_pipe != -1) {
        close(fd_for_pipe);
        fd_for_pipe = -1;
    }
}