/*
** EPITECH PROJECT, 2025
** file_interaction.c
** File description:
** interact with file.
*/

#include "../include/main_header.h"

sprite_t **get_current_hovered_pin(void)
{
    static sprite_t *sprite = NULL;

    return &sprite;
}

void reset_hovered_file(int x, int y)
{
    sprite_t *sprite = *get_current_hovered_pin();
    sfFloatRect bounds;

    if (sprite == NULL || sprite->scale.x == 0.5)
        return;
    bounds = sfSprite_getGlobalBounds(sprite->sprite);
    if (!sfFloatRect_contains(&bounds, x, y)) {
        *get_current_hovered_pin() = NULL;
        make_tween(NULL, &sprite->scale.x, 0.5, 0.1)->method = EASEIN;
        make_tween(NULL, &sprite->scale.y, 0.5, 0.1)->method = EASEIN;
    }
}

static void detect_file_mouse_hovered(file_t *file, sfVector2i p)
{
    sprite_t *sprite = file->sprite;
    sfFloatRect bounds = sfSprite_getGlobalBounds(sprite->sprite);

    if (file->is_dir && sfFloatRect_contains(&bounds, p.x, p.y)) {
        *get_current_hovered_pin() = sprite;
        if (sprite->scale.x < 0.7)
            sprite->scale.x += 0.05;
        if (sprite->scale.y < 0.7)
            sprite->scale.y += 0.05;
        return;
    }
}

void resize_hovered_file(int x, int y)
{
    file_t *current = *get_filelist();

    reset_hovered_file(x, y);
    while (current != NULL) {
        detect_file_mouse_hovered(current, IVEC(x, y));
        current = current->next;
    }
}
