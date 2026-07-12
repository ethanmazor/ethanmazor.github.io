// ============================================
// The header file for explorer class file.
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#ifndef EXPLORER_H
#define EXPLORER_H
#include "map.h"
// #include <vector>

#define UP_DIR 16
#define DOWN_DIR 17
#define LEFT_DIR 18
#define RIGHT_DIR 19

#define SPRITE_EXPLORER 0
#define SPRITE_ROBOT 1
#define SPRITE_SKULL 2

// Explorer Structure
typedef struct {
    int loc_x, loc_y, loc_x_prev, loc_y_prev; // Location of the explorer
    int direction; // current orientation of the explorer
    int score; //Current score of the explorer
    int lives; // Current lives of the explorer

    bool nv_active; // boolean flag for when night vision is activated
    int nv_start; // starting time when night vision was activated

    bool pickaxe_active; // boolean flag for when pickaxe is active

    bool scrambler_active; // boolean flag for when input scrambler is active
    int scrambler_start; // starting time when scrambler was activated
    int scrambler_map[4]; // randomized mapping for directions: 0-3 for up,down,left,right

     int sprite_type; // current sprite (0=explorer, 1=robot, 2=skull)

    // std::vector<MapItem> inventory; // resizeable array for holding tools

    // add sword or other tool statuses as desired (or can be placed elsewhere in code)
} Explorer;

// Initialize an Explorer structure
// You want to assign initial values to each of the variables defined above
// in the Explorer structure.
void explorer_init (Explorer* explorer);

#endif // EXPLORER_H