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
    #define WINNAME "name"
    #define WINW 800
    #define WINH 600
    #define RESX 800
    #define RESY 600
    #define MUSIC_VOLUME 60
    #define SFX_VOLUME 80
    #define FPS 60

    // math:
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
    #define ABS(nb) ((nb) < 0 ? -(nb) : (nb))
    #define CEILING_NEG(X) (int)(X)
    #define CEILING(X) (((X) > 0) ? CEILING_POS(X) : CEILING_NEG(X))
    #define RAD 0.01745329251
    #define DEG 57.2957795131


// --------- STRUCTURES ---------



// --------- FUNCTIONS ---------

// --- cdtor.c ---
int load_assets(void);
void destroy_assets(void);


#endif /* MAIN_H */
