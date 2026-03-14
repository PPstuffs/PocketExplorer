/*
** EPITECH PROJECT, 2025
** header_lib.h
** File description:
** Header for the C lib.
*/

#ifndef CLIB_H
    #define CLIB_H

    #include <stddef.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <stdio.h>

    // defines:
    #define SUCCESS 0
    #define ERROR 84
    #define TRUE 1
    #define FALSE 0
    #define PRINT(format, ...) mini_printf(format, __VA_ARGS__)


// --------- LIB FUNCTIONS ---------

// --- lib_c_functions.c ---
int bst_getnbr(char const *str, char c, int *pos);
int char_in_str(char ch, char const *str);
int my_getnbr(char const *str);
int mini_printf(const char *format, ...);
int debug_print(char const *format, ...);

// --- my_array_lib.c ---
int my_sort_str_array(char **array);

// --- my_filelib.c ---
int print_file(char *filename);

// --- my_stringlib.c ---
int my_strlen(char const *str);
char *my_strdup(char const *src);
char *my_strcpy(char *dest, char const *src);
int my_strcmp(char const *s1, char const *s2);
int my_strncmp(char const *s1, char const *s2, int n);

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream);


#endif /* CLIB_H */
