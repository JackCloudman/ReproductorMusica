#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "Play_list.h"
int *get_filename_ext(const char *filename,char** name) {
    *name = strrchr(filename, '.');
    return 0;
}
int main(void)
{
    struct dirent *de;  // Pointer for directory entry
    int length = 0;
    char* name = 0;

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir("/home/jack/Desktop/Reproductor/");

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    char* home = "file:///";
    char destino[100];
    while ((de = readdir(dr)) != NULL){
        length = strlen(de->d_name);
        get_filename_ext(de->d_name,&name);
        if(name!=0){
          if(strcmp(name,".mp3")==0){
            strcpy(destino,"");
            strcat(destino,home);
            strcat(destino,de->d_name);
            printf("Archivo: %s\n",destino);
          }
        }
    }

    closedir(dr);
    return 0;
}
