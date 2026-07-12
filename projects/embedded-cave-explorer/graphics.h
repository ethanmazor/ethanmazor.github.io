// ============================================
// The header file for Graphics class file.
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#ifndef GRAPHICS_H
#define GRAPHICS_H

/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const char* img);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
void draw_nothing(int u, int v);
void draw_wall(int u, int v);
void draw_explorer_init(int u, int v);
void draw_nightvision(int u, int v);
void draw_heal(int u, int v);
void draw_poison(int u, int v);
void draw_dark_outline(int u, int v);  // For torch light outline
void draw_teleporter(int u, int v);   
void draw_pickaxe(int u, int v); 
void draw_wildcard(int u, int v);
void draw_scrambler(int u, int v);

void draw_explorer(int u, int v, int direction, int type);
#endif // GRAPHICS_H