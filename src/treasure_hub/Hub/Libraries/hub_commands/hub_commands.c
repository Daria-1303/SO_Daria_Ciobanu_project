#include "hub_commands.h"

void print_help(){
    write(1, "Commands:\n", 10);
    write(1, "start_monitor\n", 14);
    write(1, "stop_monitor\n", 13);
    write(1, "list_hunts\n", 12);
    write(1, "list_treasures\n", 16);
    write(1, "view_treasure\n", 14);
    write(1, "help\n", 6);
    write(1, "clear\n", 7);
    write(1, "exit\n", 6);
}

Hub_Command_T get_command_type(char *input){
    if(strcmp(input, "start_monitor") == 0){
        return START_MONITOR;
    } 
    else if(strcmp(input, "stop_monitor") == 0){
        return STOP_MONITOR;
    } 
    else if(strcmp(input, "list_hunts") == 0){
        return LIST_HUNTS;
    } 
    else if(strncmp(input, "list_treasures ", 15) == 0){
        return LIST_TREASURES;
    } 
    else if(strncmp(input, "view_treasure ", 14) == 0){
        return VIEW_TREASURE;
    } 
    else if(strcmp(input, "help") == 0){
        return HELP;
    } 
    else if(strcmp(input, "clear") == 0){
        return CLEAR;
    } 
    else if(strcmp(input, "exit") == 0){
        return EXIT;
    }
    else {
        return INVALID_OPERATION;
    }
}

void execute_command(Hub_Command_T command, char *input){
    switch(command){
        case START_MONITOR:
            start_monitor();
            break;
        case STOP_MONITOR:
            stop_monitor();
            break;
        case LIST_HUNTS:
            list_hunts();
            break;
        case LIST_TREASURES:
            list_treasures(input);
            break;
        case VIEW_TREASURE:
            view_treasure(input);
            break;
        case HELP:
            print_help();
            break;
        case CLEAR:
            system("clear");
            break;
        case EXIT:
            if (is_monitor_running()) {
                    write(1, "Error: monitor is still running. Stop it first.\n", 48);
                }
            else{
                exit(0);
            }
        default:
            write(1, "Invalid command. Type 'help' for options.\n", 42);
        
    }
}
