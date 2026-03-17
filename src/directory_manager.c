/*
** EPITECH PROJECT, 2025
** directory_manager.c
** File description:
** Manage all the files.
*/

#include "../include/main_header.h"
#include <sys/stat.h>

int setup_files(char *directory)
{
    char **content = open_directory(directory);
    char *temp = NULL;
    struct stat statbuf;

    my_sort_str_array(content);
    if (content == NULL)
        return ERROR;
    for (int i = 0; content[i] != NULL; i++) {
        temp = MERGESTR(directory, "/", content[i]);
        if (temp == NULL)
            return ERROR;
        stat(temp, &statbuf);
        OMNIFREE(temp, 1);
        if (make_file(content[i], directory, S_ISDIR(statbuf.st_mode)) == NULL)
            return ERROR;
    }
    OMNIFREE(content, 2);
    return SUCCESS;
}

static int change_dir(file_t *file)
{
    char *temp = temp = MERGESTR(*get_current_dir(), "/", file->name);

    if (temp == NULL)
        return ERROR;
    OMNIFREE(*get_current_dir(), 1);
    *get_current_dir() = temp;
    while (*get_filelist())
        DESTROY(*get_filelist(), get_filelist(), free_file);
    while (*get_tweenlist())
        DESTROY(*get_tweenlist(), get_tweenlist(), free_tween);
    *get_current_hovered_file() = NULL;
    *scroll_position() = 0;
    return SUCCESS;
}

static int detect_file_mouse_clicked(file_t *file, int x, int y)
{
    sfFloatRect bounds = sfSprite_getGlobalBounds(file->sprite->sprite);
    char *temp = NULL;

    if (sfFloatRect_contains(&bounds, x, y)) {
        if (file->is_dir == false) {
            make_tween(NULL, &file->sprite->pos.y,
                file->sprite->pos.y - 5, 0.1)->method = FETCH;
            return TRUE;
        }
        if (change_dir(file) == ERROR)
            return TRUE;
        setup_files(*get_current_dir());
        return TRUE;
    }
    return FALSE;
}

void move_directory(int x, int y)
{
    file_t *current = *get_filelist();

    while (current != NULL) {
        if (detect_file_mouse_clicked(current, x, y) == TRUE)
            return;
        current = current->next;
    }
}
