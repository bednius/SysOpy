//
// Created by kreska on 25.03.17.
//



#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>


void print_stat(struct stat *file_stat, char *directory_path) {
    char resolved_path[PATH_MAX];
    realpath(directory_path, resolved_path);
    printf("%s\n", resolved_path);
    printf("Size: \t\t\t\t%ld bytes\n", file_stat->st_size);
    printf("Permissions: \t\t\t");
    printf((S_ISDIR(file_stat->st_mode)) ? "d" : "-");
    printf((file_stat->st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat->st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat->st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat->st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat->st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat->st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat->st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat->st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat->st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %X, &", gmtime(&(file_stat->st_mtime)));
    printf("Last modification: \t\t%s\n", buffer);
    printf("\n");


}

void tree_walk(char *path, int max_size) {
    char *new_path;
    DIR *dir = opendir(path);
    struct dirent *file;

    struct stat file_stat;

    if (dir == NULL) {
        perror("dirp:");
        return;
    }

    while ((file = readdir(dir)) != NULL) {
        if (strcmp(file->d_name, "..") != 0 && strcmp(file->d_name, ".") != 0) {

            new_path = calloc(strlen(path) + strlen(file->d_name) + 2, 1);
            strcpy(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, file->d_name);

            lstat(new_path, &file_stat);
            if (S_ISDIR(file_stat.st_mode)) {
                tree_walk(new_path, max_size);
            } else if (S_ISREG(file_stat.st_mode) && file_stat.st_size < max_size) {
                print_stat(&file_stat, new_path);

            }
            free(new_path);
        }
    }

    closedir(dir);
}

