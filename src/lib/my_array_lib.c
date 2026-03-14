/*
** EPITECH PROJECT, 2024
** my_array_lib.c
** File description:
** The array handle functions.
*/

#include "../../include/main_header.h"

int my_sort_str_array(char **array)
{
    char *tmp = NULL;

    if (array == NULL || array[0] == NULL)
        return SUCCESS;
    for (int i = 0; array[i + 1] != NULL; i++) {
        if (my_strcmp(array[i], array[i + 1]) >= 0) {
            tmp = array[i];
            array[i] = array[i + 1];
            array[i + 1] = tmp;
            my_sort_str_array(array);
        }
    }
    return SUCCESS;
}
