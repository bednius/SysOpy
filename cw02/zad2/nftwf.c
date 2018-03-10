//
// Created by kreska on 25.03.17.
//

#include <stdio.h>
#include <ftw.h>
#include <time.h>
#include <stdlib.h>
#include <linux/limits.h>

int max_size;

int display_info(const char *fpath, const struct stat *file_stat,
                 int typeflag, struct FTW *ftwbuf) {
    if (file_stat->st_size < max_size && typeflag == FTW_F) {
        char resolved_path[PATH_MAX];
        realpath(fpath, resolved_path);
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
    return 0;
}

void init_max(int max) {
    max_size = max;
}
