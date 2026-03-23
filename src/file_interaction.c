/*
** EPITECH PROJECT, 2025
** file_interaction.c
** File description:
** interact with file.
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
    char *text_temp = MERGESTR("t", current->name);

    if (text_temp == NULL)
        return;
    if (get_tween(temp) != NULL)
        pos = get_tween(temp)->dest;
    make_tween(temp, &current->sprite->pos.y,
        pos + mouse.delta * SCROLL_SPEED, 0.5);
    if (get_tween(text_temp) != NULL)
        pos_text = get_tween(text_temp)->dest;
    make_tween(text_temp, &current->text->pos.y,
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

sprite_t **get_current_hovered_file(void)
{
    static sprite_t *sprite = NULL;

    return &sprite;
}

void reset_hovered_file(int x, int y)
{
    sprite_t *sprite = *get_current_hovered_file();
    sfFloatRect bounds;

    if (sprite == NULL || sprite->scale.x == 0.5)
        return;
    bounds = sfSprite_getGlobalBounds(sprite->sprite);
    if (!sfFloatRect_contains(&bounds, x, y)) {
        *get_current_hovered_file() = NULL;
        make_tween(NULL, &sprite->scale.x, 0.5, 0.1)->method = EASEIN;
        make_tween(NULL, &sprite->scale.y, 0.5, 0.1)->method = EASEIN;
    }
}

static void detect_file_mouse_hovered(file_t *file, sfVector2i p)
{
    sprite_t *sprite = file->sprite;
    sfFloatRect bounds = sfSprite_getGlobalBounds(sprite->sprite);

    if (file->is_dir && sfFloatRect_contains(&bounds, p.x, p.y)) {
        *get_current_hovered_file() = sprite;
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

    if (*get_timerlist() != NULL)
        return;
    reset_hovered_file(x, y);
    while (current != NULL) {
        detect_file_mouse_hovered(current, IVEC(x, y));
        current = current->next;
    }
}

static int change_dir(file_t *file)
{
    char *temp = MERGESTR(*get_current_dir(), "/", file->name);

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
