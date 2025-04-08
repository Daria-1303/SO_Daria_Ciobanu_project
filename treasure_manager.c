#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_ID_LEN 16
#define MAX_USERNAME_LEN 32
#define MAX_CLUETEXT_LEN 128

// GPS Coordinate struct
typedef struct Coordonata{
    float latitude;
    float longitude;
}Coordonata_T;

// the struct for the first requirement
typedef struct Treasure{
    char id[MAX_ID_LEN];
    char userName[MAX_ID_LEN];
    Coordonata_T GPSCoordinate;
    char clueText[MAX_CLUETEXT_LEN];
    int value;
}Treasure_T;

char * build_path(const char *hunt_id){
    static char path[128];

    snprintf(path, sizeof(path), "%s_treasure.txt", hunt_id);

    return path;
}

void add_treasure(const char *hunt_id){
    Treasure_T treasure_to_be_added;

    // read the treasure data

    printf("ID: ");
    scanf("%15s", treasure_to_be_added.id);
    printf("Username : ");
    scanf("%31s", treasure_to_be_added.userName);
    printf("Latitude: ");
    scanf("%f", &treasure_to_be_added.GPSCoordinate.latitude);
    printf("Longitude: ");
    scanf("%f", &treasure_to_be_added.GPSCoordinate.longitude);
    printf("Clue text: ");
    getchar();
    fgets(treasure_to_be_added.clueText, MAX_CLUETEXT_LEN, stdin);
    treasure_to_be_added.clueText[strcspn(treasure_to_be_added.clueText, "\n")] = 0;
    printf("Value: ");
    scanf("%d", &treasure_to_be_added.value);    

    // open the file
    char *file_name = build_path(hunt_id);

    int fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644); 
    // 0644 is the permission for the file
    // 6 -> read and write for the owner
    // 4 -> read for the group
    // 4 -> read for others

    if(fd == -1){
        perror("Error opening/appending file");
        return;
    }

    // write the treasure data to the file
    // number of bytes written must be equal to the size of the struct
    ssize_t bytes_written = write(fd, &treasure_to_be_added, sizeof(treasure_to_be_added));
    if(bytes_written == -1){
        perror("Error writing to file");
        close(fd);
        return;
    }

    // close the file
    if(close(fd) == -1){
        perror("Error closing file");
        return;
    }

    printf("Treasure added successfully!\n");
}

void list_treasure(const char *hunt_id){
    char *file_name = build_path(hunt_id);

    // hunt id printed
    printf("Hunt ID: %s\n", hunt_id);

    struct stat file_stat;

    if(stat(file_name, &file_stat) == -1){
        perror("Error getting file info");
        return;
    }

    printf("File size: %ld bytes\n", file_stat.st_size);
    printf("Last modification time: %ld\n", file_stat.st_mtime);


    int file = open(file_name, O_RDONLY);

    if(file == -1){
        perror("Error opening file");
        return;
    }

    Treasure_T treasure;

    ssize_t bytes_read;

    while((bytes_read = read(file, &treasure, sizeof(treasure))) > 0){
        // if the number of bytes read is not equal to the size of the struct, there is an error
        if(bytes_read != sizeof(treasure)){
            printf("Error reading file\n");
            close(file);
            return;
        }

        printf("ID: %s\n", treasure.id);
        printf("Username: %s\n", treasure.userName);
        printf("Latitude: %f\n", treasure.GPSCoordinate.latitude);
        printf("Longitude: %f\n", treasure.GPSCoordinate.longitude);
        printf("Clue text: %s\n", treasure.clueText);
        printf("Value: %d\n", treasure.value);
        printf("\n");
    }

    if(bytes_read == -1){
        perror("Error reading file");
        close(file);
        return;
    }

    if(close(file) == -1){
        perror("Error closing file");
        return;
    }

    printf("All treasures listed successfully!\n");

}

// will be used like : ./treasure_manager argv1(--add ) argv2(hunt_id) [argv3 -> id ->name]
int main(int argc, char **argv){
    if(argc < 3){
        printf("Incorrect ussage of arguments\n");
        return 1;
    }

    if(strcmp(argv[1], "--add") == 0){
        add_treasure(argv[2]);
    }
    else if(strcmp(argv[1], "--list") == 0){
        list_treasure(argv[2]);
    }
    else if(strcmp(argv[1], "--view") == 0){
        // view_treasure(argv[2], argv[3]);
    }
    else if(strcmp(argv[1], "--remove_treasure") == 0){
        // remove_treasure(argv[2], argv[3]);
    }
    else if(strcmp(argv[1], "--remove_hunt") == 0){
        // remove_hunt(argv[2]);
    }
    else{
        printf("Invalid command\n");
        return 1;
    }



    return 0;
}