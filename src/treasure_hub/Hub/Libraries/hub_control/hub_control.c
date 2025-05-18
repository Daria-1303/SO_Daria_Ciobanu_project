#include "hub_control.h"

// ---------------------------------------------------------
// INITIAL STATE
// ---------------------------------------------------------

Hub_Monitor_T hub_monitor = {-1, OFF};

void send_command_to_monitor(const char *command){
    int fd = open(COMMAND_FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        perror("open command file");
        return;
    }

    write(fd, command, strlen(command));
    close(fd);

    kill(hub_monitor.monitor_pid, SIGUSR1);
}

int is_monitor_running(void){
    if (hub_monitor.monitor_pid > 0 && hub_monitor.monitor_status == RUNNING) {
        return 1;
    }

    return 0;
}

// 0 -> fail 1 -> success
int start_monitor(void){
    if (is_monitor_running()) {
        write(1, "Monitor is already running\n", 26);
        return 0;
    }

    hub_monitor.monitor_pid = fork();
    if (hub_monitor.monitor_pid < 0) {
        write(1, "Failed to fork monitor process\n", 31);
        return 0;
    }

    if (hub_monitor.monitor_pid == 0) {
        // Child process - the monitor
        execl(TREASURE_MONITOR_EXEC, TREASURE_MONITOR_EXEC, NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    hub_monitor.monitor_status = RUNNING;
    return 1;

}

int stop_monitor(void){
    if (!is_monitor_running()) {
        write(1, "Monitor is not running.\n", 24);
        return 0;
    }

    send_command_to_monitor("stop_monitor");

    write(1, "Monitor is shutting down...\n", 28);
    return 1;
}

int list_hunts(){
    if (!is_monitor_running()) {
        write(1, "Monitor is not running.\n", 24);
        return 0;
    }

    send_command_to_monitor("list_hunts");
    return 1;
}

int list_treasures(char* input){
    if (!is_monitor_running()) {
        write(1, "Monitor is not running.\n", 24);
        return 0;
    }

    char *hunt_id = input + 15;
    char command[BUFFER_SIZE];

    snprintf(command, sizeof(command), "list_treasures %s", hunt_id);
    send_command_to_monitor(command);

    return 1;
}

int view_treasure(char* input){
    if (!is_monitor_running()) {
        write(1, "Monitor is not running.\n", 24);
        return 0;
    }

    char command[BUFFER_SIZE];
    
    snprintf(command, sizeof(command), "%s", input);
    send_command_to_monitor(command);

    return 1;
}