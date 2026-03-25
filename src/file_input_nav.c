/*
** EPITECH PROJECT, 2025
** file_input_nav.c
** File description:
** input navigation with file.
*/

#include "../include/main_header.h"

int *scroll_position(void)
{
    static int factor = 0;

    return &factor;
}

static void scroll_file(sfMouseWheelScrollEvent mouse, file_t *current, char *temp)
{
    float pos = current->sprite->pos.y;
    float pos_text = current->text->pos.y;

    if (get_tween(temp) != NULL)
        pos = get_tween(temp)->dest;
    make_tween(temp, &current->sprite->pos.y,
        pos + mouse.delta * SCROLL_SPEED, 0.5);
    if (get_tween(current->name) != NULL)
        pos_text = get_tween(current->name)->dest;
    make_tween(current->name, &current->text->pos.y,
        pos_text + mouse.delta * SCROLL_SPEED, 0.5);
}

void scroll_files(sfMouseWheelScrollEvent mouse)
{
    file_t *cur = *get_filelist();
    char *temp = NULL;

    if (cur == NULL || (mouse.delta > 0 && *scroll_position() <= 0) ||
            (mouse.delta < 0 && cur->sprite->pos.y <= FILE_SIZE))
        return;
    for (; cur != NULL; cur = cur->next) {
        if (get_tween(cur->name) != NULL)
            return;
    }
    *scroll_position() = MAX(0, *scroll_position() + mouse.delta * -SCROLL_SPEED);
    for (cur = *get_filelist(); cur != NULL; cur = cur->next) {
        temp = MERGESTR("s", cur->name);
        if (temp == NULL)
            continue;
        scroll_file(mouse, cur, temp);
        OMNIFREE(temp, 1);
    }
}

static int change_current_dir(file_t *file)
{
    char *temp = NULL;
    char *dir = (*get_current_dir());

    if (my_strcmp(file->name, "..") != 0) {
        temp = MERGESTR(*get_current_dir(), "/", file->name);
        if (temp == NULL)
            return ERROR;
        OMNIFREE(*get_current_dir(), 1);
        *get_current_dir() = temp;
        return SUCCESS;
    }
    if (*get_current_dir() == NULL)
        return ERROR;
    for (int i = my_strlen(dir); i > 0; i--) {
        if (dir[i] == '/') {
            dir[i] = '\0';
            break;
        }
    }
    return SUCCESS;
}

static void reset_files(void)
{
    while (*get_tweenlist())
        DESTROY(*get_tweenlist(), get_tweenlist(), free_tween);
    while (*get_filelist())
        DESTROY(*get_filelist(), get_filelist(), free_file);
    *get_hovered_file() = NULL;
    *scroll_position() = 0;
}

static bool is_file_clicked(file_t *file, int x, int y)
{
    float pos = 0.0;
    tween_t *tween = NULL;

    if (sprite_rect_contains(file->sprite, x, y)) {
        if (file->type != FOLDER) {
            pos = get_file_pos(LISTLEN(get_filelist()) - GETNODEORD(file,
                get_filelist()) - 1).y;
            tween = make_tween("fileposx", &file->sprite->pos.y, pos - 5, 0.1);
            tween->start = pos;
            tween->method = FETCH;
            return false;
        }
        if (change_current_dir(file) == ERROR)
            return false;
        reset_files();
        setup_files(*get_current_dir());
        return true;
    }
    return false;
}

void move_directory(int x, int y)
{
    file_t *current = *get_filelist();

    while (current != NULL) {
        if (is_file_clicked(current, x, y) == true)
            return;
        current = current->next;
    }
}
