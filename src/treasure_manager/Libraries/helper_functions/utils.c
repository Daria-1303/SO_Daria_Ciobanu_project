#include "utils.h"

// TREASURE

Treasure_T *create_treasure(){
    Treasure_T *treasure_to_be_added = malloc(sizeof(Treasure_T));

    if(treasure_to_be_added == NULL){
        perror("Error allocating memory");
        return NULL;
    }

    printf("Enter treasure data: \n");

    printf("ID (max %d chars): ", MAX_ID_LEN-1);

    if (scanf("%15s", treasure_to_be_added->id) != 1) {
        free(treasure_to_be_added);
        return NULL;
    }


    printf("Username (max %d chars): ", MAX_USERNAME_LEN-1);

    if (scanf("%31s", treasure_to_be_added->userName) != 1) {
        free(treasure_to_be_added);
        return NULL;
    }

    printf("Latitude: ");
    scanf("%f", &treasure_to_be_added->GPSCoordinate.latitude);

    printf("Longitude: ");
    scanf("%f", &treasure_to_be_added->GPSCoordinate.longitude);
    printf("Clue text: ");

    getchar(); 
    fgets(treasure_to_be_added->clueText, MAX_CLUETEXT_LEN, stdin);
    treasure_to_be_added->clueText[strcspn(treasure_to_be_added->clueText, "\n")] = 0;

    printf("Value: ");
    scanf("%d", &treasure_to_be_added->value);


    return treasure_to_be_added;
}

// DIR

Operation_ERROR_T DIR_exists(const char *dir_path){
    DIR *dir = opendir(dir_path);

    if(!dir){
        return OPERATION_FAILED;   // directory does not exist
    }

    closedir(dir);

    return NO_ERROR;   // directory exists
}

Operation_ERROR_T DIR_create(const char *dir_path) {
    if (DIR_exists(dir_path)) {
        return NO_ERROR; // Directory already exists
    }

    if (mkdir(dir_path, 0755) == -1){
        return OPERATION_FAILED; // Failed to create directory
    } 
    
    return NO_ERROR; // Directory created successfully
}

// FILES

int FILE_create(const char *file_path) {
    int file = open(file_path, CREATE_FILE, 0644);

    if(file == -1){
        printf("Error opening/appending file");
        return -1;
    }

    return file;
}

int symlink_create(const char *hunt_id) {
    // target -> root directory
    char target_path[MAX_PATH_LEN_DIR];
    snprintf(target_path, sizeof(target_path), "%s/%s/%s", TREASURE_DIR, hunt_id, TREASURE_LOG);

    char link_name[MAX_PATH_LEN_DIR];
    snprintf(link_name, sizeof(link_name), "%s-%s", TREASURE_SYMLINK, hunt_id);

    struct stat sb;

    if (lstat(link_name, &sb) == 0) {
        if (S_ISLNK(sb.st_mode)) {
            printf("Symlink already exists: %s\n", link_name);
            return 0;
        } else {
            printf("%s is not a symlink!\n", link_name);
            return -1;
        }
    }

    if(symlink(target_path, link_name) == -1){
        printf("Error creating symlink\n");
        return -1;
    }

    printf("Symlink created successfully: %s -> %s\n", link_name, target_path);

    return 1;
}