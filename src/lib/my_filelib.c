/*
** EPITECH PROJECT, 2026
** my_filelib.c
** File description:
** The C functions for the files.
*/

#include "../../include/header_lib.h"
#include <stdio.h>

int print_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    if (file == NULL)
        return ERROR;
    while (read != -1) {
        if (read != 0)
            write(1, line, read);
        read = my_getline(&line, &len, file);
    }
    free(line);
    fclose(file);
    return SUCCESS;
}
