#include "monitor.h"

// ---------------------------------------------------------
// the HUB will fork the monitor 
// ---------------------------------------------------------

//  ---------------------------------------------------------
//  Global State
//  ---------------------------------------------------------

volatile sig_atomic_t is_running = 1;

void monitor_loop(){
    char message[BUFFER_SIZE];

    while(is_running){
        // read data from stdin
        ssize_t bytes_read = read(STDIN_FILENO, message, sizeof(message) - 1);

        if(bytes_read < 0){
            break;
        }
        
        message[bytes_read] = '\0'; // null terminate the string

        write(STDOUT_FILENO, "\r", 1); // clear the line

        execute_treasure_manager(message);
    }
}

int main(void){
    char message[BUFFER_SIZE];

    // setup signal handlers
    setup_signal_handlers();

    // start the monitor loop
    snprintf(message, sizeof(message), "Monitor started. Type commands to execute the treasure manager.\n");
    write(STDOUT_FILENO, message, strlen(message));

    // start the monitor loop
    monitor_loop();

    sleep(5);

    // stop the monitor
    snprintf(message, sizeof(message), "Monitor stopped. Exiting...\n");
    write(STDOUT_FILENO, message, strlen(message));
    
    return 0;
}