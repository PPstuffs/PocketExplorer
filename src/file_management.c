/*
** EPITECH PROJECT, 2025
** file_management.c
** File description:
** Manage all the files.
*/

#include "../include/main_header.h"

file_t **get_filelist(void)
{
    static file_t *file_list = NULL;

    return &file_list;
}

static char *get_file_sprite([[maybe_unused]] char *name, bool is_dir)
{
    return (is_dir) ? "folder" : "file";
}

int get_x_pos(int index)
{
    return (index - 1) % ((WINH + FILE_SIZE) / FILE_SIZE) * FILE_SIZE + 100;
}

int get_y_pos(int index)
{
    return (index - 1) / ((WINH + FILE_SIZE) / FILE_SIZE) * FILE_SIZE + 100;
}

static file_t *setup_new_file(file_t *nwfile, char *name, int x, int y)
{
    nwfile->name = my_strdup(name);
    if (nwfile->name == NULL)
        return OMNIFREE(nwfile, 1);
    nwfile->sprite = make_sprite(name,
        get_file_sprite(name, nwfile->is_dir), x, y);
    nwfile->sprite->scale = VEC(0.5, 0.5);
    center_sprite_origin(nwfile->sprite, 0.5, 0.5);
    if (nwfile->sprite == NULL)
        return SDFREE("%1 %1", nwfile->name, nwfile);
    nwfile->text = make_text(name, name,
        x, y + nwfile->sprite->rect.height / 4);
    nwfile->text->color = (sfColor){180, 180, 180, 255};
    nwfile->text->scale = VEC(0.5, 0.5);
    center_text_origin(nwfile->text, 1, 1);
    if (nwfile->text == NULL) {
        DESTROY(nwfile->sprite, get_spritelist(), free_sprite);
        return SDFREE("%1 %1", nwfile->name, nwfile);
    }
    return nwfile;
}

file_t *make_file(char *name, bool is_dir)
{
    file_t *nwfile = malloc(sizeof(file_t) * 1);
    int len = LISTLEN(get_filelist());
    int x = get_x_pos(len);
    int y = get_y_pos(len);

    if (nwfile == NULL)
        return NULL;
    nwfile->is_dir = is_dir;
    if (setup_new_file(nwfile, name, x, y) == NULL)
        return NULL;
    nwfile->sprite->alpha = 0;
    nwfile->sprite->pos.y += FILE_RISE_AMOUNT;
    nwfile->text->alpha = 0;
    nwfile->text->pos.y += FILE_RISE_AMOUNT;
    run_timer(name, len / FILE_SPEED);
    nwfile->next = *get_filelist();
    *get_filelist() = nwfile;
    return nwfile;
}

file_t *get_file(char const *name)
{
    file_t *current = *get_filelist();

    if (name == NULL)
        return NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void free_file(file_t *file)
{
    if (file == NULL)
        return;
    OMNIFREE(file->name, 1);
    DESTROY(file->sprite, get_spritelist(), free_sprite);
    DESTROY(file->text, get_textlist(), free_text);
    OMNIFREE(file, 1);
}
