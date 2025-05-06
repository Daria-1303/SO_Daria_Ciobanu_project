#include "hub.h"

// ---------------------------------------------------------
// INITIAL STATE
// ---------------------------------------------------------

Hub_Monitor_T hub_monitor = {-1, OFF};

int is_monitor_running(){
    if(hub_monitor.monitor_pid > 0 && hub_monitor.monitor_status == RUNNING){
        return 1;
    }
    return 0;
}

// 0 -> fail 1 -> success
int start_monitor(){
    // if the monitor is already running we'll return 0 -> fail
    if(is_monitor_running()){
        write(1, "Monitor is already running\n", 26);
        return 0;
    }

    // fork a new process
    hub_monitor.monitor_pid = fork();

    if(hub_monitor.monitor_pid < 0){
        write(1, "Failed to fork monitor process\n", 32);
        return 0;
    }

    if(hub_monitor.monitor_pid == 0){
        // child process

        char *args[] = {TREASURE_MONITOR_EXEC, NULL};

        execv(TREASURE_MONITOR_EXEC, args);

        // if execv fails
        write(1, "Failed to exec monitor process\n", 32);
        exit(1);
    }

    // parent process
    hub_monitor.monitor_status = RUNNING;

    return 1;    
}


// 0 -> fail 1 -> success
int stop_monitor(){
    if (!is_monitor_running()) {    // 1-> monitor is running, 0 -> monitor is not running
        write(1, "Monitor is not running.\n", 25);
        return 0;
    }

    hub_monitor.monitor_status = SHUTTING_DOWN;
    kill(hub_monitor.monitor_pid, SIGTERM);

    write(1, "Sent termination signal to monitor.\n", 36);

    return 1;
}

void send_command_to_monitor(const char *command) {
    int fd = open("monitor_command.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open command file");
        return;
    }

    write(fd, command, strlen(command));
    close(fd);

    kill(hub_monitor.monitor_pid, SIGUSR1);
}



void on_sigchld(int signo){
    int saved_errno = errno;
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        if (pid == hub_monitor.monitor_pid) {
            hub_monitor.monitor_status = OFF;
            hub_monitor.monitor_pid = -1;
            write(1, "Monitor has terminated.\n", 25);
        }
    }
    errno = saved_errno;
}

void handle_sigchld(){
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_sigchld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}
