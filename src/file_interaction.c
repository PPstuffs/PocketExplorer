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

    if (get_tween(temp) != NULL)
        pos = get_tween(temp)->dest;
    make_tween(temp, &current->sprite->pos.y,
        pos + mouse.delta * SCROLL_SPEED, 0.5);
    if (get_tween(current->name) != NULL)
        pos_text = get_tween(current->name)->dest;
    make_tween(current->name, &current->text->pos.y,
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

static sprite_t **get_current_hovered_file(void)
{
    static sprite_t *sprite = NULL;

    return &sprite;
}

static bool is_file_hovered(file_t *file, sfVector2i p)
{
    sprite_t *sprite = file->sprite;
    sfFloatRect bounds = sfSprite_getGlobalBounds(sprite->sprite);
    static float new_scale = 0.0;

    if (file->type != IMAGE && *get_current_hovered_file() != sprite &&
            sfFloatRect_contains(&bounds, p.x, p.y)) {
        new_scale = (file->type == FOLDER) ? 0.7 : 0.55;
        *get_current_hovered_file() = sprite;
        set_tween_method(
            make_tween(sprite->name, &sprite->scale.x, new_scale, 0.2),
            BACKOUT, 5);
        set_tween_method(
            make_tween(&sprite->name[1], &sprite->scale.y, new_scale, 0.2),
            BACKOUT, 5);
        return true;
    }
    return false;
}

void resize_hovered_file(int x, int y)
{
    file_t *current = *get_filelist();
    sprite_t *sprite = *get_current_hovered_file();
    sfFloatRect bounds;

    if (sprite != NULL && sprite->scale.x != 0.5) {
        bounds = sfSprite_getGlobalBounds(sprite->sprite);
        if (!sfFloatRect_contains(&bounds, x, y)) {
            *get_current_hovered_file() = NULL;
            set_tween_method(
                make_tween(sprite->name, &sprite->scale.x, 0.5, 0.5),
                BACKOUT, 5);
            set_tween_method(
                make_tween(&sprite->name[1], &sprite->scale.y, 0.5, 0.5),
                BACKOUT, 5);
        }
    }
    for (; current != NULL; current = current->next) {
        if (is_file_hovered(current, IVEC(x, y)) == true)
            break;
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

static bool is_file_clicked(file_t *file, int x, int y)
{
    sfFloatRect bounds = sfSprite_getGlobalBounds(file->sprite->sprite);
    char *temp = NULL;

    if (sfFloatRect_contains(&bounds, x, y)) {
        if (file->type != FOLDER) {
            make_tween("fileposx", &file->sprite->pos.y,
                file->sprite->pos.y - 5, 0.1)->method = FETCH;
            return true;
        }
        if (change_dir(file) == ERROR)
            return true;
        setup_files(*get_current_dir());
        return true;
    }
    return false;
}

void move_directory(int x, int y)
{
    file_t *current = *get_filelist();

    while (current != NULL) {
        if (is_file_clicked(current, x, y) == true)
            return;
        current = current->next;
    }
}
