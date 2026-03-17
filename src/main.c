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

    resize_hovered_file(TOXY(GETMOUSEPOS));
    while (sfRenderWindow_pollEvent(WINDOW, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(WINDOW);
        if (event.type == sfEvtMouseButtonPressed &&
                event.mouseButton.button == sfMouseLeft)
            move_directory(event.mouseButton.x, event.mouseButton.y);
        if (event.type == sfEvtMouseWheelScrolled)
            scroll_files(event.mouseWheelScroll);
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
    draw_allsprites(NONE);
    draw_alltexts(NONE);
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
    setup_files(*get_current_dir());
    while (sfRenderWindow_isOpen(WINDOW)) {
        sfRenderWindow_clear(WINDOW, (sfColor){46, 46, 46, 255});
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
    if (ac > 2)
        return print_file("assets/usage.txt");
    *get_current_dir() = my_strdup((ac == 2) ? av[1] : ".");
    if (*get_current_dir() == NULL)
        return ERROR;
    return launch();
}
