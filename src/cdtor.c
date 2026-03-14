/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** main file.
*/

#include "../include/main_header.h"

int load_assets(void)
{
    create_window(WINW, WINH, WINNAME, sfDefaultStyle);
    init_cam();
    *get_clock() = sfClock_create();
    make_file("hello", true);
    return SUCCESS;
}

void destroy_assets(void)
{
    while (*get_texturelist())
        DESTROY(*get_texturelist(), get_texturelist(), free_texture);
    while (*get_spritelist())
        DESTROY(*get_spritelist(), get_spritelist(), free_sprite);
    while (*get_textlist())
        DESTROY(*get_textlist(), get_textlist(), free_text);
    while (*get_tweenlist())
        DESTROY(*get_tweenlist(), get_tweenlist(), free_tween);
    while (*get_timerlist())
        DESTROY(*get_timerlist(), get_timerlist(), free_timer);
    while (*get_soundbank())
        DESTROY(*get_soundbank(), get_soundbank(), free_sound);
    destroy_clock();
    destroy_window();
    destroy_cam();
}

