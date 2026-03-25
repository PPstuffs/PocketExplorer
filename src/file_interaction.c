/*
** EPITECH PROJECT, 2025
** file_interaction.c
** File description:
** interact with file.
*/

#include "../include/main_header.h"

file_t **get_hovered_file(void)
{
    static file_t *file = NULL;

    return &file;
}

static void launch_hoverfile_tween(file_t *file, float imgscale, float time,
    float op(float, float))
{
    sprite_t *sprite = file->sprite;
    tween_t *tw = get_tween(&sprite->name[2]);
    char *tween_name = MERGESTR("tween", sprite->name);
    sfVector2i p = get_file_pos(LISTLEN(get_filelist()) - GETNODEORD(file,
        get_filelist()) - 1);

    if (tween_name == NULL)
        return;
    set_tween_method(
        make_tween(tween_name, &sprite->scale.x, imgscale, 0.2),
        BACKOUT, 5);
    set_tween_method(
        make_tween(&tween_name[1], &sprite->scale.y, imgscale, 0.2),
        BACKOUT, 5);
    set_tween_method(
        make_tween(&tween_name[2], &file->text->pos.y,
        op(p.y + FILE_SIZE / 4 + 7, 7), time),
        BACKOUT, 5);
    OMNIFREE(tween_name, 1);
}

static bool is_file_hovered(file_t *file, sfVector2i p)
{
    sprite_t *sprite = file->sprite;
  
    if (file->type != IMAGE && *get_hovered_file() != file &&
            sprite_rect_contains(sprite, TOXY(p))) {
        *get_hovered_file() = file;
        launch_hoverfile_tween(file,
            (file->type == FOLDER) ? 0.7 : 0.55, 0.2, ADD);
        return true;
    }
    return false;
}

void resize_hovered_file(int x, int y)
{
    file_t *current = *get_filelist();
    file_t *f = *get_hovered_file();

    if (f != NULL && f->sprite != NULL && f->sprite->scale.x != 0.5 &&
            !sprite_rect_contains(f->sprite, x, y)) {
        *get_hovered_file() = NULL;
        launch_hoverfile_tween(f, 0.5, 0.35, SUB);
    }
    for (; current != NULL; current = current->next) {
        if (is_file_hovered(current, IVEC(x, y)) == true)
            break;
    }
}
