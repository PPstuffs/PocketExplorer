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

static bool is_file_hovered(file_t *file, sfVector2i p)
{
    sprite_t *sprite = file->sprite;
    static float new_scale = 0.0;
    tween_t *tw = NULL;
  
    if (file->type != IMAGE && *get_hovered_file() != file &&
            sprite_rect_contains(sprite, TOXY(p))) {
        new_scale = (file->type == FOLDER) ? 0.7 : 0.55;
        *get_hovered_file() = file;
        set_tween_method(
            make_tween(sprite->name, &sprite->scale.x, new_scale, 0.2),
            BACKOUT, 5);
        set_tween_method(
            make_tween(&sprite->name[1], &sprite->scale.y, new_scale, 0.2),
            BACKOUT, 5);
        tw = get_tween(&sprite->name[2]);
        set_tween_method(
            make_tween(&sprite->name[2], &file->text->pos.y,
            (tw != NULL) ? tw->dest + 10 : file->text->pos.y + 10, 0.2),
            BACKOUT, 5);
        return true;
    }
    return false;
}

void resize_hovered_file(int x, int y)
{
    file_t *current = *get_filelist();
    file_t *f = *get_hovered_file();
    tween_t *tw = NULL;

    if (f->sprite != NULL && f->sprite->scale.x != 0.5 &&
            !sprite_rect_contains(file->sprite, x, y)) {
        *get_hovered_file() = NULL;
        set_tween_method(
            make_tween(f->sprite->name, &f->sprite->scale.x, 0.5, 0.35),
            BACKOUT, 5);
        set_tween_method(
            make_tween(&f->sprite->name[1], &f->sprite->scale.y, 0.5, 0.35),
            BACKOUT, 5);
        tw = get_tween(&f->sprite->name[2]);
        set_tween_method(
            make_tween(&f->name[2], &f->text->pos.y,
            (tw != NULL) ? tw->dest - 10 : f->text->pos.y - 10, 0.35),
            BACKOUT, 5);
    }
    for (; current != NULL; current = current->next) {
        if (is_file_hovered(current, IVEC(x, y)) == true)
            break;
    }
}
