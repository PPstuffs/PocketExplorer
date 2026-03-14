/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** main file.
*/

#include "../include/main_header.h"

static void events(void)
{
    sfEvent event;

    while (sfRenderWindow_pollEvent(WINDOW, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(WINDOW);
    }
}

int update_csfml_stuff(void)
{
    update_tweens();
    update_timers();
    update_cam();
    update_music();
    update_sounds();
    update_animsprites();
    return SUCCESS;
}

int update_run(void)
{
    events();
    update_csfml_stuff();
    if (!sfRenderWindow_hasFocus(WINDOW))
        return SUCCESS; // Pause the game
    return SUCCESS;
}

int launch(void)
{
    int retval = ERROR;

    load_assets();
    while (sfRenderWindow_isOpen(WINDOW)) {
        sfRenderWindow_clear(WINDOW, sfBlack);
        retval = update_run();
        if (retval == ERROR)
            break;
        sfRenderWindow_display(WINDOW);
    }
    destroy_assets();
    return retval;
}

// the CSFML itself leaks 226,396 bytes of memory on its own
// music and sounds can add to the memory leaks as well
//
// srand() sets the seed to 1 if it fails
// in which case the first rand() will always return 495274367
int main(int ac, char *av[])
{
    srand(time(NULL));
    if (rand() == 495274367)
        return debug_print("Failed to srand.");
    if (ac != 1) {
        return print_file("assets/usage.txt");
    }
    return launch();
}
