/*
** EPITECH PROJECT, 2026
** my_getline.c
** File description:
** getline function.
*/

#include "../../include/header_lib.h"

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream)
{
    char *new_ptr = NULL;
    size_t pos = 0;
    int c;

    if (*lineptr == NULL) {
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) return -1;
    }
    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            *n *= 2;
            new_ptr = realloc(*lineptr, *n);
            if (!new_ptr)
                return -1;
            *lineptr = new_ptr;
        }
        (*lineptr)[pos++] = (char)c;
        if (c == '\n')
            break;
    }
    if (pos == 0 && c == EOF)
        return -1;
    (*lineptr)[pos] = '\0';
    return pos;
}
