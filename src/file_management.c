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

file_t *make_file(char *name, bool is_dir)
{
    file_t *nwfile = malloc(sizeof(file_t) * 1);
    int x = 100;
    int y = 100;

    if (nwfile == NULL)
        return NULL;
    nwfile->name = my_strdup(name);
    if (nwfile->name == NULL)
        return OMNIFREE(nwfile, 1);
    nwfile->sprite = make_sprite(name, get_file_sprite(name, is_dir), x, y);
    nwfile->sprite->scale = VEC(0.5, 0.5);
    center_sprite_origin(nwfile->sprite, 0.5, 0.5);
    if (nwfile->sprite == NULL)
        return SDFREE("%1 %1", nwfile->name, nwfile);
    nwfile->is_dir = is_dir;
    nwfile->text = make_text(name, name,
        x, y + nwfile->sprite->rect.height / 4);
    nwfile->text->color = (sfColor){180, 180, 180, 255};
    nwfile->text->scale = VEC(0.5, 0.5);
    center_text_origin(nwfile->text, 1, 1);
    if (nwfile->text == NULL) {
        DESTROY(nwfile->sprite, get_spritelist(), free_sprite);
        return SDFREE("%1 %1", nwfile->name, nwfile);
    }
    nwfile->next = *get_filelist();
    *get_filelist() = nwfile;
}

void free_file(file_t *file)
{
    if (file == NULL)
        return;
    OMNIFREE(file->name, 1);
    DESTROY(file->sprite, get_spritelist(), free_sprite);
    DESTROY(file->text, get_textlist(), free_sprite);
    OMNIFREE(file, 1);
}
