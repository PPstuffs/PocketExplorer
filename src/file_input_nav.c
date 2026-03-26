/*
** EPITECH PROJECT, 2025
** file_input_nav.c
** File description:
** input navigation with file.
*/

#include "../include/main_header.h"

void scroll_files(sfMouseWheelScrollEvent mouse)
{
    file_t *cur = *get_filelist();
    char *temp = NULL;
    static float scroll = 0.0;

    if (mouse.delta > 0 && CAM->center.y <= WINH / 2)
        return;
    CAM->center.y += -mouse.delta * SCROLL_SPEED;
}

void reset_files(void)
{
    while (*get_tweenlist())
        DESTROY(*get_tweenlist(), get_tweenlist(), free_tween);
    while (*get_filelist())
        DESTROY(*get_filelist(), get_filelist(), free_file);
    *get_hovered_file() = NULL;
}

static bool is_file_clicked(file_t *file, int x, int y)
{
    float pos = 0.0;
    tween_t *tween = NULL;
    char *temp = NULL;

    if (sprite_rect_contains(file->sprite, x, y)) {
        if (file->type != FOLDER) {
            pos = get_file_pos(LISTLEN(get_filelist()) - GETNODEORD(file,
                get_filelist()) - 1).y;
            tween = make_tween("fileposx", &file->sprite->pos.y, pos - 5, 0.1);
            tween->start = pos;
            tween->method = FETCH;
            return false;
        }
        temp = my_strdup(file->name);
        reset_files();
        setup_files(temp);
        OMNIFREE(temp, 1);
        return true;
    }
    return false;
}

void move_directory(int x, int y)
{
    file_t *current = *get_filelist();

    while (current != NULL) {
        if (is_file_clicked(current, x, y) == true) {
            CAM->center.y = WINH / 2;
            return;
        }
        current = current->next;
    }
}
