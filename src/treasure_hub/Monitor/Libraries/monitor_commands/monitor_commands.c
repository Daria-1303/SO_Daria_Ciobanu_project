#include "monitor_commands.h"

void exec_treasure_manager(char *command, char *hunt_id, char *treasure_id){
    pid_t pid = fork();

    if(pid < 0){
        perror("Error forking");
        return;
    }

    if(pid == 0){
        if(treasure_id){
            execl(TREASURE_MANAGER_EXEC, TREASURE_MANAGER_EXEC, command, hunt_id, treasure_id, NULL);
            write(2, "Error executing treasure manager\n", 34);
            exit(1);
        }
        else{
            execl(TREASURE_MANAGER_EXEC, TREASURE_MANAGER_EXEC, command, hunt_id, NULL);
            write(2, "Error executing treasure manager\n", 34);
            exit(1);
        }
    }

    // Parent process
    waitpid(pid, NULL, 0);
}

void exec_calculate_score(char *hunt_id){
    pid_t pid = fork();

    if(pid < 0){
        perror("Error forking");
        return;
    }

    if(pid == 0){
        execl(TREASURE_CALCULATOR_EXEC, TREASURE_CALCULATOR_EXEC, hunt_id, NULL);
        write(2, "Error executing calculate score\n", 34);
        exit(1);
    }

    // Parent process
    waitpid(pid, NULL, 0);
}

void process_command(){
    char buffer[BUFFER_SIZE];

    // I use an additional file descriptor
    // so i can read the command from the terminal
    int fd = open(COMMAND_FILE_PATH, O_RDONLY);

    if(fd < 0){
        perror("Can't read command file");
        return;
    }

    int rd = read(fd, buffer, sizeof(buffer) - 1);

    close(fd);

    if(rd< 0){
        perror("Error reading command");
        return;
    }

    buffer[rd] = '\0';

    if(strcmp(buffer, "stop_monitor") == 0){
        write(1, "Monitor stop command, exit in 5s\n", 33);
        usleep(5000000);
        exit(0);
    }
    else if(strcmp(buffer, "list_hunts") == 0) {
        list_hunts();
    }
    else if (strncmp(buffer, "list_treasures ", 15) == 0) {
      char hunt_id[10];
      sscanf(buffer, "list_treasures %s", hunt_id);
      
      exec_treasure_manager("--list", hunt_id, NULL);
    }
    else if(strncmp(buffer, "view_treasure ", 14) == 0){
        char hunt_id[10], treasure_id[10];
        if(sscanf(buffer + 14, "%s %s", hunt_id, treasure_id) == 2){
            exec_treasure_manager("--view", hunt_id, treasure_id);
        }
        else{
            write(1, "Invalid format\n", 15);
        }
    }
    else if(strcmp(buffer, "calculate_score") == 0){
        // the logic for finding every hunt of treasure_hunts folder
        // and then exec the calculator
        DIR *dir = opendir(TREASURE_HUNTS_PATH);

        if(dir == NULL){
            write(2, "Error opening directory\n", 22);
            return;
        }

        struct dirent *entry;
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_DIR){
                // skip the . and .. directories
                if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
                    char hunt_id[10];
                    strcpy(hunt_id, entry->d_name);
                    exec_calculate_score(hunt_id);
                }
            }
        }

        closedir(dir);      
    }
    else{
        write(1, "Monitor invalid command\n", 25);
    }
}