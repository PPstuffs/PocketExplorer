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

static sprite_t *make_file_sprite(file_t *nwfile, char *name, char *directory,
    sfVector2i p)
{
    sprite_t *sprite = NULL;
    char *ext = get_file_format(name);
    char *temp = NULL;
    float scale = 0.0;

    if (my_strcmp(ext, ".mp3") == 0 || my_strcmp(ext, ".ogg") == 0 ||
            my_strcmp(ext, ".wav") == 0 || my_strcmp(ext, ".flac") == 0) {
        nwfile->type = SONG;
        return make_sprite(name, "music", TOXY(p));
    }
    if (my_strcmp(ext, ".png") != 0 && my_strcmp(ext, ".jpg") != 0 &&
            my_strcmp(ext, ".jpeg") != 0)
        return make_sprite(name, (nwfile->type == FOLDER) ? "folder" : "file",
            TOXY(p));
    nwfile->type = IMAGE;
    temp = MERGESTR(directory, "/", name);
    sprite = make_sprite(name, temp, TOXY(p));
    OMNIFREE(temp, 1);
    if (sprite == NULL)
        return NULL;
    scale = MIN((float)FILE_SIZE / (float)sprite->rect.width,
        (float)FILE_SIZE / (float)sprite->rect.height);
    sprite->scale = VEC(scale, scale);
    return sprite;
}

static file_t *setup_new_file(file_t *nwfile, char *name, char *directory,
    sfVector2i p)
{
    nwfile->name = my_strdup(name);
    if (nwfile->name == NULL)
        return OMNIFREE(nwfile, 1);
    nwfile->sprite = make_file_sprite(nwfile, name, directory, p);
    if (nwfile->sprite == NULL)
        return SDFREE("%1 %1", nwfile->name, nwfile);
    nwfile->sprite->scale.x /= 2.0;
    nwfile->sprite->scale.y /= 2.0;
    center_sprite_origin(nwfile->sprite, 0.5, 0.5);
    if (nwfile->sprite == NULL)
        return SDFREE("%1 %1", nwfile->name, nwfile);
    nwfile->text = make_text(name, name,
        p.x, p.y + FILE_SIZE / 4);
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

static int setup_pwd(char *directory)
{
    text_t *text = get_text("pwd");
    float x = 0.0;

    if (text == NULL) {
        text = make_text("pwd", directory, WINW / 2, 5);
        text->scale = VEC(0.5, 0.5);
        text->color = sfWhite;
        text->alpha = 0.7;
    } else {
        x = sfText_getGlobalBounds(text->text).width;
        set_text_string(text, directory);
        x = (WINW + sfText_getGlobalBounds(text->text).width - x) / 2.0;
        make_tween("pwdx", &text->pos.x, WINW / 2, 0.5)->start = x;
    }
    center_text_origin(text, 0.5, 0);
    make_tween("alpha", &text->alpha, 0.7, 1)->start = 1;
    return SUCCESS;
}

file_t *make_file(char *name, char *directory, bool is_dir)
{
    file_t *nwfile = malloc(sizeof(file_t) * 1);
    int len = LISTLEN(get_filelist());
    sfVector2i p = get_file_pos(len);

    if (nwfile == NULL)
        return NULL;
    nwfile->type = (is_dir == true) ? FOLDER : OTHERFILE;
    if (setup_new_file(nwfile, name, directory, p) == NULL)
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

static char *init_dir(const char *newdir, char *dir)
{
    char *temp = NULL;

    if (dir == NULL)
        return my_strdup(newdir);
    if (my_strcmp(newdir, "..") != 0) {
        temp = MERGESTR(dir, "/", newdir);
        if (temp == NULL)
            return NULL;
        OMNIFREE(dir, 1);
        return temp;
    }
    if (dir == NULL)
        return NULL;
    for (int i = my_strlen(dir); i > 0; i--) {
        if (dir[i] == '/') {
            dir[i] = '\0';
            break;
        }
    }
    return dir;
}

int setup_files(char *directory)
{
    static char *dir = NULL;
    char **content = NULL;
    char *temp = NULL;
    struct stat statbuf;

    dir = init_dir(directory, dir);
    content = open_directory(dir);
    my_sort_str_array(content);
    if (content == NULL)
        return ERROR;
    for (int i = my_strcmp(dir, ".") == 0 ? 1 : 0; i < ARRLEN(content); i++) {
        temp = MERGESTR(dir, "/", content[i]);
        if (temp == NULL)
            return ERROR;
        stat(temp, &statbuf);
        OMNIFREE(temp, 1);
        make_file(content[i], dir, S_ISDIR(statbuf.st_mode));
    }
    OMNIFREE(content, 2);
    return setup_pwd(dir);
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
