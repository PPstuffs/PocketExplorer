/*
** EPITECH PROJECT, 2025
** timers_grave.c
** File description:
** Functions that get called when a tween or timer ends.
*/

#include "../include/main_header.h"

void timer_grave(char *tag)
{
    file_t *file = get_file(tag);

    if (file == NULL)
        return;
    make_tween(NULL, &file->sprite->alpha, 1, 1);
    make_tween(NULL, &file->sprite->pos.y,
        file->sprite->pos.y - FILE_RISE_AMOUNT, 1);
    make_tween(NULL, &file->text->alpha, 1, 1);
    make_tween(NULL, &file->text->pos.y,
        file->text->pos.y - FILE_RISE_AMOUNT, 1);
    return;
}
