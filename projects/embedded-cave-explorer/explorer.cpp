// ============================================
// The explorer class file
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "explorer.h"

void explorer_init (Explorer * e)
{
    e->loc_x = 0, e->loc_y = 0;
    e->direction = RIGHT_DIR;
    e->score = 0;
    e->lives = 1;
    e->sprite_type = SPRITE_EXPLORER;
    
    e->nv_active = false;
    e->nv_start = 0;

    e->pickaxe_active = false;
    
    e->scrambler_active = false;
    e->scrambler_start = 0;
    for (int i = 0; i < 4; i++) e->scrambler_map[i] = i; // identity mapping initially
    
}
