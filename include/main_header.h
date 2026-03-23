/*
** EPITECH PROJECT, 2025
** main_header.h
** File description:
** Header file for a project.
*/

#ifndef MAIN_H
    #define MAIN_H


// --------- INCLUDES ---------

    #include "header_lib.h"
    #include "header_tools.h"
    #include "header_csfml.h"
    #include <stddef.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <stdbool.h>
    #include <SFML/System.h>
    #include <SFML/Graphics.h>
    #include <time.h>


// --------- DEFINES ---------

    // general values:
    #define SUCCESS 0
    #define ERROR 84
    #define TRUE 1
    #define FALSE 0
    #define MAX_INT 2147483647

    // Program values:
    #define WINNAME "PocketExplorer"
    #define WINW 800
    #define WINH 600
    #define RESX 800
    #define RESY 600
    #define MUSIC_VOLUME 60
    #define SFX_VOLUME 80
    #define FPS 60

    #define FILE_SIZE 150
    #define FILE_SPEED 15.0
    #define FILE_RISE_AMOUNT 25
    #define SCROLL_SPEED 10

    // math:
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
    #define ABS(nb) ((nb) < 0 ? -(nb) : (nb))
    #define CEILING_NEG(X) (int)(X)
    #define CEILING(X) (((X) > 0) ? CEILING_POS(X) : CEILING_NEG(X))
    #define RAD 0.01745329251
    #define DEG 57.2957795131


// --------- STRUCTURES ---------

typedef enum filetype_s {
    FOLDER,
    IMAGE,
    SONG,
    OTHERFILE
} filetype_t;

typedef struct file_s {
    struct file_s *next;
    char *name;
    sprite_t *sprite;
    text_t *text;
    filetype_t type;
} file_t;

// --------- FUNCTIONS ---------

// --- cdtor.c ---
char **get_current_dir(void);
int load_assets(void);
void destroy_assets(void);

// --- file_interaction.c ---
void scroll_files(sfMouseWheelScrollEvent mouse);
void resize_hovered_file(int x, int y);
void move_directory(int x, int y);

// --- file_management.c ---
file_t **get_filelist(void);
file_t *make_file(char *name, char *directory, bool is_dir);
int setup_files(char *directory);
void free_file(file_t *file);


#endif /* MAIN_H */
