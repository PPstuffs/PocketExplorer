/*
** EPITECH PROJECT, 2025
** file_management.c
** File description:
** Manage all the files.
*/

#include "../include/main_header.h"
#include <dirent.h>

int setup_files(char *directory)
{
    DIR *dirp = opendir(directory);
    struct dirent *dp;

    if (dirp == NULL)
        return ERROR;
    dp = readdir(dirp);
    while (dp != NULL) {
        if (dp->d_name[0] == '.' && my_strlen(dp->d_name) == 1) {
            dp = readdir(dirp);
            continue;
        }
        if (make_file(dp->d_name, (dp->d_type == DT_DIR)) == NULL)
            return ERROR;
        dp = readdir(dirp);
    }
    closedir(dirp);
    return SUCCESS;
}
