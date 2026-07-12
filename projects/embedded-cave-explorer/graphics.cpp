// ============================================
// The Graphics class file
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "graphics.h"
#include "globals.h"
#include "explorer.h"

// Character facing forward (Down)
const char* EXPLORER_DOWN = 
    "bbbbbbbbbbb"
    "bbbbYYYbbbb"
    "bbbbDWDbbbb" 
    "bbbbDDDbbbb" 
    "bbbGGGGGbbb" 
    "bbbGGGGGbbb" 
    "bbbGGGGGbbb"
    "bbbb333bbbb" 
    "bbbb3b3bbbb"
    "bbbbDbDbbbb" 
    "bbbbbbbbbbb";

// Character facing away (Up)
const char* EXPLORER_UP = 
    "bbbbbbbbbbb"
    "bbbbYYYbbbb"
    "bbbbYYYbbbb" 
    "bbbbYYYbbbb"
    "bbbGGGGGbbb" 
    "bbbGGGGGbbb"
    "bbbGGGGGbbb"
    "bbbb333bbbb"
    "bbbb3b3bbbb"
    "bbbbDbDbbbb"
    "bbbbbbbbbbb";

// Character facing right (Side profile)
const char* EXPLORER_RIGHT = 
    "bbbbbbbbbbb"
    "bbbYYYbbbbb" 
    "bbbYYDbbbbb"
    "bbbYYWbbbbb" 
    "bbbYYDbbbbb"
    "bbGGGGbbbbb" 
    "bbGGGGbbbbb"
    "bbGGGGbbbbb"
    "bbb33bbbbbb" 
    "bbb33bbbbbb"
    "bbbDDbbbbbb" 
    "bbbbbbbbbbb";

const char* NIGHTVISION_SPRITE = 
    "bbbbbbbbbbb" 
    "bbDDDDDDDbb" 
    "bD3333333Db" 
    "b355555553b" 
    "b35GG3GG53b" 
    "b3GGG3GGG3b" 
    "b35GG3GG53b" 
    "b335555533b" 
    "bb333b333bb" 
    "bbbbbbbbbbb" 
    "bbbbbbbbbbb";

// Character facing left (Side profile mirror)
const char* EXPLORER_LEFT = 
    "bbbbbbbbbbb"
    "bbbbbYYYbbb"
    "bbbbbDYYbbb"
    "bbbbbWYYbbb"
    "bbbbbDYYbbb"
    "bbbbbGGGGbb"
    "bbbbbGGGGbb"
    "bbbbbGGGGbb"
    "bbbbbb33bbb"
    "bbbbbb33bbb"
    "bbbbbbDDbbb"
    "bbbbbbbbbbb";

// A flask with a white cork, glass neck, and red liquid
const char* HEAL_POTION_SPRITE = 
    "bbbbWbbbbbb"
    "bbbb3bbbbbb"
    "bbb5R5bbbbb"
    "bbb5RR5bbbb"
    "bb5RRRR5bbb"
    "bb5RRRR5bbb"
    "bb5RRRR5bbb"
    "bbb5RR5bbbb"
    "bbbb55bbbbb"
    "bbbbbbbbbbb"
    "bbbbbbbbbbb";

// Same flask shape, filled with green liquid
const char* POISON_POTION_SPRITE = 
    "bbbbWbbbbbb"
    "bbbb3bbbbbb"
    "bbb5G5bbbbb"
    "bbb5GG5bbbb"
    "bb5GGGG5bbb"
    "bb5GGGG5bbb"
    "bb5GGGG5bbb"
    "bbb5GG5bbbb"
    "bbbb55bbbbb"
    "bbbbbbbbbbb"
    "bbbbbbbbbbb";

// A metallic pad with active, glowing yellow and white energy rising from it.
const char* TELEPORTER_SPRITE = 
    "bbbbYbbYbbb" 
    "bbbYbbWbbYb" 
    "bbbbWmWbbbb" 
    "bbb5YYY5bbb" 
    "bb35YWY53bb" 
    "b35YYWYY53b" 
    "bb35YWY53bb" 
    "bbb5YYY5bbb" 
    "bb3355533bb" 
    "b333333333b"  
    "bbbbbbbbbbb";

// A curved grey head with a brown handle
const char* PICKAXE_SPRITE = 
    "bbbbbbbbbbb"
    "bbb33333bbb"
    "bb3555553bb"
    "bb333D333bb"
    "bbbbbDbbbbb"
    "bbbbbDbbbbb"
    "bbbbbDbbbbb"
    "bbbbbDbbbbb"
    "bbbbbDbbbbb"
    "bbbbbDbbbbb"
    "bbbbbbbbbbb";

// A spade-shaped grey head with a brown handle
const char* SHOVEL_SPRITE = 
    "bbbbbbbbbbb"
    "bbbb333bbbb"
    "bbb35553bbb"
    "bbb35553bbb"
    "bbb35553bbb"
    "bbbb353bbbb"
    "bbbbbDbbbbb"
    "bbbbbDbbbbb"
    "bbbbbDbbbbb"
    "bbbbbDbbbbb"
    "bbbbbbbbbbb";

// a question mark representing a wildcard item
const char* WILDCARD_SPRITE = 
    "ZZZZZZZZZZZ"
    "ZZZ555ZZZZZ"
    "ZZ55555ZZZZ"
    "ZZ5ZZZ5ZZZZ"
    "ZZZZZZ5ZZZZ"
    "ZZZZZ5ZZZZZ"
    "ZZZZ5ZZZZZZ"
    "ZZZZ5ZZZZZZ"
    "ZZZZZZZZZZZ"
    "ZZZZ5ZZZZZZ"
    "ZZZZZZZZZZZ";

// four arrows representing the input scrambler item
const char* SCRAMBLER_SPRITE = 
    "ZZZZZ5ZZZZZ"
    "ZZZZ555ZZZZ"
    "ZZZ55555ZZZ"
    "Z5ZZ555ZZ5Z"
    "Z55Z555Z55Z"
    "Z5ZZ555ZZ5Z"
    "ZZZ55555ZZZ"
    "ZZZZ555ZZZZ"
    "ZZZZZ5ZZZZZ"
    "ZZZZZZZZZZZ"
    "ZZZZZZZZZZZ";

const char* ROBOT_SPRITE = 
    "ZZZZZZZZZZZ"
    "ZZZ55555ZZZ"
    "ZZ5555555ZZ"
    "ZZ5Z5Z5Z5ZZ"
    "ZZ5555555ZZ"
    "ZZ5555555ZZ"
    "ZZ5Z555Z5ZZ"
    "ZZZZ555ZZZZ"
    "ZZ555555555"
    "ZZ5ZZ5ZZ5ZZ"
    "ZZ5ZZ5ZZ5ZZ";

const char* SKULL_SPRITE = 
    "ZZZZZZZZZZZ"
    "ZZZ55555ZZZ"
    "ZZ5555555ZZ"
    "Z555555555Z"
    "Z5Z55Z55Z5Z"
    "Z555555555Z"
    "Z555555555Z"
    "ZZ55ZZZ55ZZ"
    "ZZ5555555ZZ"
    "ZZ5Z5Z5Z5ZZ"
    "ZZZZZZZZZZZ";

void draw_nothing(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = 0xFFFF00;   // Yellow
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = 0xD2691E;   // "Dirt"
        else if (img[i] == '5') colors[i] = LGREY;      // 50% grey
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'W') colors[i] = 0xFFFFFF; // white
        else if (img[i] == 'Z') colors[i] = BLACK;
        else if (img[i] == 'P') colors[i] = 0x800080; // purple
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

// For each sprite, there is an array of 121 characters describing the color of each pixel of the tile where the sprite is drawn
// Characters describe colors according to draw_img
// Feel free to add more!
void draw_wall(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, 0x8e8e8e8e);
}

void draw_nightvision(int u, int v)
{
    draw_img(u, v, NIGHTVISION_SPRITE);
}

void draw_explorer_init(int u, int v)
{
    draw_explorer(u, v, RIGHT_DIR, 0);
}

void draw_explorer(int u, int v, int direction, int type)
{
    if (type == 0) { // SPRITE_EXPLORER
        switch (direction) {
            case (UP_DIR)   : draw_img(u, v, EXPLORER_UP); return;
            case (DOWN_DIR) : draw_img(u, v, EXPLORER_DOWN); return;
            case (LEFT_DIR) : draw_img(u, v, EXPLORER_LEFT); return;
            case (RIGHT_DIR): draw_img(u, v, EXPLORER_RIGHT); return;
        }
    }
    else if (type == 1) { // SPRITE_ROBOT
        draw_img(u, v, ROBOT_SPRITE);
    }
    else if (type == 2) { // SPRITE_SKULL
        draw_img(u, v, SKULL_SPRITE);
    }
}

void draw_heal(int u, int v)
{
    draw_img(u, v, HEAL_POTION_SPRITE);
}

void draw_poison(int u, int v)
{
    draw_img(u, v, POISON_POTION_SPRITE);
}

void draw_teleporter(int u, int v)
{
   draw_img(u, v, TELEPORTER_SPRITE);
}

void draw_pickaxe(int u, int v)
{
    draw_img(u, v, PICKAXE_SPRITE);
}

void draw_wildcard(int u, int v)
{
    draw_img(u, v, WILDCARD_SPRITE);
}

void draw_scrambler(int u, int v)
{
    draw_img(u, v, SCRAMBLER_SPRITE);
}

// Provided: calculate which sprite to draw based on the screen position
// Use this in draw_game()
void draw_dark_outline(int u, int v)
{
    // Convert screen coordinates back to relative tile position
    int i = (u - 3) / 11 - 5;  // Calculate i from u
    int j = (v - 15) / 11 - 4; // Calculate j from v
    
    // Fill the entire tile with dark color first
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
    
    // Determine which side is closest to the center and draw orange line
    if (abs(i) > abs(j)) {
        // Horizontal edge is closest to center
        if (i < 0) {
            // Right edge (orange line on right)
            uLCD.filled_rectangle(u+10, v, u+10, v+10, DORANGE);  // orange line on right edge
        } else {
            // Left edge (orange line on left)
            uLCD.filled_rectangle(u, v, u, v+10, DORANGE);  // orange line on left edge
        }
    } else if (abs(i) != abs(j)) {
        // Vertical edge is closest to center
        if (j < 0) {
            // Bottom edge (orange line on bottom)
            uLCD.filled_rectangle(u, v+10, u+10, v+10, DORANGE);  // orange line on bottom edge
        } else {
            // Top edge (orange line on top)
            uLCD.filled_rectangle(u, v, u+10, v, DORANGE);  // orange line on top edge
        }
    }
}