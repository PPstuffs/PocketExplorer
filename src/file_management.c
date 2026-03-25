/*
** EPITECH PROJECT, 2025
** file_management.c
** File description:
** Manage all the files.
*/

#include "../include/main_header.h"
#include <sys/stat.h>

file_t **get_filelist(void)
{
    static file_t *file_list = NULL;

    return &file_list;
}

sfVector2i get_file_pos(int i)
{
    return IVEC((i - 1) % ((WINH + FILE_SIZE) / FILE_SIZE) * FILE_SIZE + 100,
        (i - 1) / ((WINH + FILE_SIZE) / FILE_SIZE) * FILE_SIZE + 100);
}

static sprite_t *make_file_sprite(file_t *nwfile, char *name, int x, int y)
{
    sprite_t *sprite = NULL;
    char *ext = get_file_format(name);
    char *pwd = NULL;
    float scale = 0.0;

    if (my_strcmp(ext, ".mp3") == 0 || my_strcmp(ext, ".ogg") == 0 ||
            my_strcmp(ext, ".wav") == 0 || my_strcmp(ext, ".flac") == 0) {
        nwfile->type = SONG;
        return make_sprite(name, "music", x, y);
    }
    if (my_strcmp(ext, ".png") != 0 && my_strcmp(ext, ".jpg") != 0 &&
            my_strcmp(ext, ".jpeg") != 0)
        return make_sprite(name, (nwfile->type == FOLDER) ? "folder" : "file",
            x, y);
    nwfile->type = IMAGE;
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
    if (nwfile->sprite == NULL)
        return SDFREE("%1 %1", nwfile->name, nwfile);
    nwfile->sprite->scale.x /= 2.0;
    nwfile->sprite->scale.y /= 2.0;
    center_sprite_origin(nwfile->sprite, 0.5, 0.5);
    if (nwfile->sprite == NULL)
        return SDFREE("%1 %1", nwfile->name, nwfile);
    nwfile->text = make_text(name, name,
        x, y + FILE_SIZE / 4);
    if (nwfile->text == NULL) {
        DESTROY(nwfile->sprite, get_spritelist(), free_sprite);
        return SDFREE("%1 %1", nwfile->name, nwfile);
    }
    nwfile->text->color = (sfColor){180, 180, 180, 255};
    nwfile->text->scale = VEC(MIN((FILE_SIZE - 10) /
        (float)sfText_getGlobalBounds(nwfile->text->text).width, 0.5), 0.5);
    center_text_origin(nwfile->text, 0.5, 0);
    return nwfile;
}

static void setup_nwfile_tweens(file_t *nwfile, float speed)
{
    set_tween_delay(
        make_tween(NULL, &nwfile->text->pos.y,
            nwfile->text->pos.y - FILE_RISE_AMOUNT, 1),speed);
    set_tween_delay(make_tween(NULL, &nwfile->text->alpha, 1, 1), speed);
    set_tween_delay(make_tween(NULL, &nwfile->sprite->pos.y,
        nwfile->sprite->pos.y - FILE_RISE_AMOUNT, 1), speed);
    set_tween_delay(make_tween(NULL, &nwfile->sprite->alpha, 1, 1), speed);
}

file_t *make_file(char *name, char *directory, bool is_dir)
{
    file_t *nwfile = malloc(sizeof(file_t) * 1);
    int len = LISTLEN(get_filelist());
    sfVector2i p = get_file_pos(len);

    if (nwfile == NULL)
        return NULL;
    nwfile->type = (is_dir == true) ? FOLDER : OTHERFILE;
    if (setup_new_file(nwfile, name, TOXY(p)) == NULL)
        return NULL;
    nwfile->sprite->alpha = 0;
    nwfile->sprite->pos.y += FILE_RISE_AMOUNT;
    nwfile->text->alpha = 0;
    nwfile->text->pos.y += FILE_RISE_AMOUNT;
    setup_nwfile_tweens(nwfile, len / FILE_SPEED);
    nwfile->next = *get_filelist();
    *get_filelist() = nwfile;
    return nwfile;
}

static void init_dir(const char *newdir, char *dir)
{
    char *temp = NULL;

    if (my_strcmp(newdir, "..") != 0) {
        temp = MERGESTR((dir == NULL) ? "." : dir, "/", newdir);
        if (temp == NULL)
            return;
        OMNIFREE(dir, 1);
        dir = temp;
        return;
    }
    if (dir == NULL)
        return;
    for (int i = my_strlen(dir); i > 0; i--) {
        if (dir[i] == '/') {
            dir[i] = '\0';
            break;
        }
    }
}

int setup_files(char *directory)
{
    char **content = open_directory(directory);
    static char *dir = NULL;
    char *temp = NULL;
    struct stat statbuf;

    my_sort_str_array(content);
    if (content == NULL)
        return ERROR;
    init_dir(directory, dir);
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

void free_file(file_t *file)
{
    if (file == NULL)
        return;
    OMNIFREE(file->name, 1);
    DESTROY(file->sprite, get_spritelist(), free_sprite);
    DESTROY(file->text, get_textlist(), free_text);
    OMNIFREE(file, 1);
}
