#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_ID_LEN 16
#define MAX_USERNAME_LEN 32
#define MAX_CLUETEXT_LEN 128

typedef struct Coordonata{
    float latitude;
    float longitude;
}Coordonata_T;

typedef struct Treasure{
    char id[MAX_ID_LEN];
    char userName[MAX_ID_LEN];
    Coordonata_T GPSCoordinate;
    char clueText[MAX_CLUETEXT_LEN];
    int value;
}Treasure_T;

int main(int argc, char **argv){


    return 0;
}