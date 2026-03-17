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

int get_x_pos(int index)
{
    return (index - 1) % ((WINH + FILE_SIZE) / FILE_SIZE) * FILE_SIZE + 100;
}

int get_y_pos(int index)
{
    return (index - 1) / ((WINH + FILE_SIZE) / FILE_SIZE) * FILE_SIZE + 100;
}

static sprite_t *make_file_sprite(file_t *nwfile, char *name, int x, int y)
{
    sprite_t *sprite = NULL;
    char *ext = get_file_format(name);
    char *pwd = NULL;
    float scale = 0.0;

    if (my_strcmp(ext, ".mp3") == 0 || my_strcmp(ext, ".ogg") == 0 ||
            my_strcmp(ext, ".wav") == 0 || my_strcmp(ext, ".flac") == 0)
        return make_sprite(name, "music", x, y);
    if (my_strcmp(ext, ".png") != 0 && my_strcmp(ext, ".jpg") != 0 &&
            my_strcmp(ext, ".jpeg") != 0)
        return make_sprite(name, (nwfile->is_dir) ? "folder" : "file", x, y);
    pwd = MERGESTR(*get_current_dir(), "/", name);
    if (pwd == NULL)
        return NULL;
    sprite = make_sprite(name, pwd, x, y);
    OMNIFREE(pwd, 1);
    if (sprite == NULL)
        return NULL;
    scale = MIN((float)FILE_SIZE / (float)sprite->rect.width,
        (float)FILE_SIZE / (float)sprite->rect.height);
    sprite->scale = VEC(scale, scale);
    return sprite;
}

static file_t *setup_new_file(file_t *nwfile, char *name, int x, int y)
{
    nwfile->name = my_strdup(name);
    if (nwfile->name == NULL)
        return OMNIFREE(nwfile, 1);
    nwfile->sprite = make_file_sprite(nwfile, name, x, y);
    nwfile->sprite->scale.x = nwfile->sprite->scale.x / 2.0;
    nwfile->sprite->scale.y = nwfile->sprite->scale.y / 2.0;
    center_sprite_origin(nwfile->sprite, 0.5, 0.5);
    if (nwfile->sprite == NULL)
        return SDFREE("%1 %1", nwfile->name, nwfile);
    nwfile->text = make_text(name, name,
        x, y + FILE_SIZE / 4);
    nwfile->text->color = (sfColor){180, 180, 180, 255};
    nwfile->text->scale = VEC(0.5, 0.5);
    center_text_origin(nwfile->text, 1, 1);
    if (nwfile->text == NULL) {
        DESTROY(nwfile->sprite, get_spritelist(), free_sprite);
        return SDFREE("%1 %1", nwfile->name, nwfile);
    }
    return nwfile;
}

file_t *make_file(char *name, char *directory, bool is_dir)
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
