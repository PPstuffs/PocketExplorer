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
    sfVector2i p = {CAM->center.x + (GETMOUSEPOS.x + CAM->offset.x - 400),
        CAM->center.y + (GETMOUSEPOS.y + CAM->offset.y - 300)};

    resize_hovered_file(TOXY(p));
    while (sfRenderWindow_pollEvent(WINDOW, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(WINDOW);
        if (event.type == sfEvtMouseButtonPressed &&
                event.mouseButton.button == sfMouseLeft)
            move_directory(TOXY(p));
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

int launch(int ac, char *av[])
{
    int retval = ERROR;

    load_assets();
    setup_files((ac == 2) ? av[1] : ".");
    while (sfRenderWindow_isOpen(WINDOW)) {
        sfRenderWindow_clear(WINDOW, (sfColor){36, 36, 36, 255});
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
    return launch(ac, av);
}
