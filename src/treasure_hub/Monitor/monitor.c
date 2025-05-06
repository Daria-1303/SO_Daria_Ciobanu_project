#include "monitor.h"

void handle_sigterm(int signum){
    char message[BUFFER_SIZE];

    // clear current line
    write(STDOUT_FILENO, "\r", 1);

    snprintf(message, sizeof(message), "Received signal %d. Shutting down...\n", signum);

    write(STDOUT_FILENO, message, strlen(message));

    // set the flag to 0
    is_running = 0;
}

static void setup_sigchld_handler(){
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = SIG_IGN; // ignore child process termination
    sa.sa_flags = SA_NOCLDWAIT; // do not create zombie processes
    sigaction(SIGCHLD, &sa, NULL); // set the signal handler
}

void setup_signal_handlers() {
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    //catch shutdown signal
    sa.sa_handler = handle_sigterm;
    sigaction(SIGTERM, &sa, NULL);  // catch termination signal
    sigaction(SIGINT, &sa, NULL);   // catch interrupt signal
    sigaction(SIGHUP, &sa, NULL);   // catch hangup signal

    // prevent zombie processes

    setup_sigchld_handler();
}

void execute_treasure_manager(const char *command){
    char message[BUFFER_SIZE];

    // fork the treasure manager
    pid_t pid = fork();

    if(pid < 0){
        // clear the line
        write(STDOUT_FILENO, "\r", 1);
        snprintf(message, sizeof(message), "Error: fork failed: %s\n", strerror(errno));
        write(STDOUT_FILENO, message, strlen(message));

        return;
    }

    if(pid == 0){
        // in child process -> we execute the treasure manager
        execlp(TREASURE_MANAGER_EXEC, TREASURE_MANAGER_EXEC, NULL);

        // if execlp fails
        // clear the line
        write(STDOUT_FILENO, "\r", 1);
        snprintf(message, sizeof(message), "Error: execlp failed: %s\n", strerror(errno));
        write(STDOUT_FILENO, message, strlen(message));

        exit(EXIT_FAILURE);
    }

    // in parent process -> we wait for the child to finish
    waitpid(pid, NULL, 0);
    kill(pid, SIGUSR1); // kill the child process
}