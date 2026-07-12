//=================================================================
// The main program file.
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// PROJECT INCLUDES
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "explorer.h"
#include "ThisThread.h"
#include <math.h>
#include <stdio.h>

// Important definitions


/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS
Explorer explorer;
bool teleporter_spawned = false;
bool restart_game = false;
bool paused = false;
int high_score = 0;

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES
void playSound(char* wav);
int get_action(GameInputs inputs);
int update_game(int action);
void draw_upper_status();
void draw_lower_status();
void draw_border();
void draw_game(int draw_option);
void init_main_map();

// helper functions
int  random_X();
int  random_Y();
void add_N_items(int type, int count);
void check_night_vision_timer();
void check_scrambler_timer();
void check_game_over(int result);
void check_spawn_teleporter(int result);

void print_instructions();
void print_start_page();
void draw_pause_overlay();
void display_stats();
void draw_wildcard_popup(const char* item_name);



/////////////////////////////////////////////////////////////////////////////////////////

/**
 * Program entry point! This is where it all begins.
 * This function is for all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    uLCD.baudrate(115200);
    wait_us(20000);

    GameInputs inputs;
    Timer t;
    t.start();

    // Outer restart loop
    while(1) {
        restart_game = false;
        paused = false;
        teleporter_spawned = false;

        // Start screen loop (toggle instructions with B1, start with CENTER only)
        bool show_instructions = false;
        bool screen_rst = true; // force initial draw
        while(1){
            if (screen_rst) {
                uLCD.cls();
                (show_instructions) ? print_instructions() : print_start_page();
                screen_rst = false;
            }

            inputs = read_inputs();
            if (inputs.b1) { 
                show_instructions = !show_instructions;
                screen_rst = true;
                wait_us(180000); // debounce ~180ms
                continue;
            }
            if (inputs.center) { // ONLY start on center press
                srand(t.elapsed_time().count());
                break;
            }
        }

        uLCD.cls();

        explorer_init(&explorer);
        
        map_init();
        init_main_map();

        // Initialize game state
        explorer.loc_x = explorer.loc_y = 5;
        explorer.loc_x_prev = explorer.loc_y_prev = 0;
        
        // Initial drawing
        draw_game(FULL_DRAW);
        
        // Main game loop
        while(!restart_game) {
        
            // Timer to measure game update speed
            t.reset();
            t.start();
            
            // 1. Read inputs 
            inputs = read_inputs();
            
            // 2. Determine action based off user input (move, act, menu, etc.)
            int action = get_action(inputs);

            if (action == MENU_BUTTON) {
                paused = true;
                draw_pause_overlay();
                wait_us(180000); // Debounce to wait for button release before entering pause loop

                while (paused) {
                    GameInputs pause_inputs = read_inputs();
                    if (pause_inputs.b4) {
                        paused = false;
                        wait_us(180000); // Debounce to prevent immediate re-pause
                        break;
                    }
                    wait_us(18000);
                }
                
            }
            
            // 3. Update and store game state based off action
            int state = update_game(action);

            // display explorer stats at top of LCD
            display_stats();

            check_night_vision_timer();     // Check if night vision duration has expired
            check_scrambler_timer();        // Check if scrambler duration has expired
            check_spawn_teleporter(state);  // Check if teleporter should be spawned
            
            // 4. Draw the game based on the new state
            draw_game(state);

            check_game_over(state);         // Check if players has won or lost
            
            // Compute update time
            t.stop();
            int dt = t.read_ms();

            // Display and wait
            // NOTE: Text is 8 pixels tall
            if (dt < 100) {
                wait_us((100 - dt) * 1000); // wait to enforce a 10 Hz update rate
            }
        }
    }
}

/**
 * Plays a .wav file
 */
void playSound(char* wav) {}

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
int get_action(GameInputs inputs) // get_action code block
{

    // Build the raw action first
    int action = NO_ACTION;
    
    // 1. Check your action and menu button inputs and return the corresponding action value
    if (inputs.center)  action = CENTER_PRESSED;
    else if (inputs.b1) action = ACTION_BUTTON;
    else if (inputs.b2) action = SWITCH_ITEM;
    else if (inputs.b3) action = SWITCH_SPRITE;
    else if (inputs.b4) action = MENU_BUTTON;
    // 2. Check for your navigation switch inputs and return the corresponding action value
    else if (inputs.left)  action = GO_LEFT;
    else if (inputs.right) action = GO_RIGHT;
    else if (inputs.up)    action = GO_UP;
    else if (inputs.down)  action = GO_DOWN;
    
    // If scrambler is active, remap only directional inputs
    if (explorer.scrambler_active) {
        int input_idx = -1;
        if (action == GO_UP) input_idx = 0;
        else if (action == GO_DOWN) input_idx = 1;
        else if (action == GO_LEFT) input_idx = 2;
        else if (action == GO_RIGHT) input_idx = 3;
        
        if (input_idx >= 0) {
            action = explorer.scrambler_map[input_idx];
        }
    }
    
    return action;
}

/**
 * This function is called by update game to check when the explorer collects an object. 
 * We update the explorer and game conditions as needed based on the object type.
 */
int get_object(){
    
    //TODO: Implement
    
    MapItem* currItem = get_here(explorer.loc_x, explorer.loc_y);
    int itemType = NO_RESULT;
    if (currItem) {
        switch (currItem->type) {
            case NIGHTVISION: {
                explorer.nv_active = 1;
                explorer.nv_start = us_ticker_read();
                explorer.score += 10;
                map_erase(explorer.loc_x, explorer.loc_y);
                add_N_items(NIGHTVISION, 1);
                itemType = GOT_OBJ;
                break;
            }

            case HEAL: {
                explorer.lives++;
                map_erase(explorer.loc_x, explorer.loc_y);
                itemType = GOT_OBJ;
                break;
            }

            case POISON: {
                explorer.lives--;
                map_erase(explorer.loc_x, explorer.loc_y);
                itemType = GOT_OBJ;
                break;
            }

            case TELEPORTER: {
                // int dest_xy = (int) currItem->data; implement later
                map_erase(explorer.loc_x, explorer.loc_y);
                explorer.loc_x = 34, explorer.loc_y = 3; // teleport player to boss chamber
                break;
            }

            case PICKAXE: {
                explorer.pickaxe_active = 1;
                map_erase(explorer.loc_x, explorer.loc_y);
                add_N_items(PICKAXE, 1);
                itemType = GOT_OBJ;
                break;
            }

            case WILDCARD: {
                int r = random() % 3;
                map_erase(explorer.loc_x, explorer.loc_y);
                if (r == 0) {
                    // Night vision power-up
                    add_N_items(NIGHTVISION, 1);
                    explorer.nv_active = 1;
                    explorer.nv_start = us_ticker_read();
                    explorer.score += 10;
                    draw_wildcard_popup("Night Vision");
                    itemType = GOT_OBJ;
                } else if (r == 1) {
                    // Heal
                    explorer.lives++;
                    draw_wildcard_popup("Heal Potion");
                    itemType = GOT_OBJ;
                } else {
                    // Poison
                    explorer.lives--;
                    draw_wildcard_popup("Poison!");
                    itemType = GOT_OBJ;
                }
                break;
            }

            case SCRAMBLER: {
                explorer.scrambler_active = 1;
                explorer.scrambler_start = us_ticker_read();
                map_erase(explorer.loc_x, explorer.loc_y);
                // Generate random permutation for directional inputs only
                int available[4] = {GO_UP, GO_DOWN, GO_LEFT, GO_RIGHT};
                for (int i = 0; i < 4; i++) {
                    int j = i + random() % (4 - i);
                    int temp = available[i];
                    available[i] = available[j];
                    available[j] = temp;
                    explorer.scrambler_map[i] = available[i];
                }
                itemType = GOT_OBJ;
                break;
            }
        }
    }
    return itemType;
    
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the explorer position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the explorer has not moved.
 */
 
 
int update_game(int action) // updates the game with all changes
{
    explorer.loc_x_prev = explorer.loc_x;
    explorer.loc_y_prev = explorer.loc_y;

    int state = NO_RESULT;

    switch (action) {

        case GO_UP: {
            explorer.direction = UP_DIR;
            MapItem* northItem = get_north(explorer.loc_x, explorer.loc_y);
            if (!northItem || northItem->walkable) {
                explorer.loc_y--;
                explorer.score++;
                state = MOVED;
                get_object();
            }
            break;
        }  

        case GO_DOWN: {
            explorer.direction = DOWN_DIR;
            MapItem* southItem = get_south(explorer.loc_x, explorer.loc_y);
            if (!southItem || southItem->walkable) {
                explorer.loc_y++;
                explorer.score++;
                state = MOVED;
                get_object();
            }
            break;
        }

        case GO_LEFT: {
            explorer.direction = LEFT_DIR;
            MapItem* westItem = get_west(explorer.loc_x, explorer.loc_y);
            if (!westItem || westItem->walkable) {
                explorer.loc_x--;
                explorer.score++;
                state = MOVED;
                get_object();
            }
            break;
        }

        case GO_RIGHT: {
            explorer.direction = RIGHT_DIR;
            MapItem* eastItem = get_east(explorer.loc_x, explorer.loc_y);
            if (!eastItem || eastItem->walkable) {
                explorer.loc_x++;
                explorer.score++;
                state = MOVED;
                get_object();
            }
            break;
        }

        case CENTER_PRESSED: {
            if (explorer.loc_x > 30 && explorer.loc_y < 10) state = WIN;
            break;
        }

        case SWITCH_SPRITE: {
            explorer.sprite_type = (explorer.sprite_type + 1) % 3;
            break;
        }
        
        case MENU_BUTTON: {
            state = FULL_DRAW;
            break;
        }

        case ACTION_BUTTON: {
            if (!explorer.pickaxe_active) break; // do nothing if no pickaxe
            MapItem* item;
            switch (explorer.direction) {
                case UP_DIR: {
                    item = get_north(explorer.loc_x, explorer.loc_y);
                    if (item->type == WALL) {
                        explorer.score += 5;
                        map_erase(explorer.loc_x, explorer.loc_y - 1);
                    }
                    break;
                }
                case DOWN_DIR: {
                    item = get_south(explorer.loc_x, explorer.loc_y);
                    if (item->type == WALL) {
                        explorer.score += 5;
                        map_erase(explorer.loc_x, explorer.loc_y + 1);
                    }
                    break;
                }
                case LEFT_DIR: {
                    item = get_west(explorer.loc_x, explorer.loc_y);
                    if (item->type == WALL) {
                        explorer.score += 5;
                        map_erase(explorer.loc_x - 1, explorer.loc_y);
                    }
                    break;
                }
                case RIGHT_DIR: {
                    item = get_east(explorer.loc_x, explorer.loc_y);
                    if (item->type == WALL) {
                        explorer.score += 5;
                        map_erase(explorer.loc_x + 1, explorer.loc_y);
                    }
                    break;
                }
            }     
        }

        case NO_ACTION:
        default:
            state = NO_RESULT;
    }
    return state;
}




/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{
    uLCD.line(0, 9, 127, 9, GREEN);
}

/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    uLCD.line(0, 118, 127, 118, GREEN);
}

/**
 * Draw the border for the map.
 */
void draw_border()
{
    uLCD.filled_rectangle(  0,   9, 127,  14, WHITE);   // Top
    uLCD.filled_rectangle(  0,  13,   2, 114, WHITE);   // Left
    uLCD.filled_rectangle(  0, 114, 127, 117, WHITE);   // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE);   // Right
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int draw_option)
{
    
    // Draw game border first
    if(draw_option == FULL_DRAW) 
    {
        draw_border();
        int u = 58;
        int v = 56;
        draw_explorer(u, v, explorer.direction, explorer.sprite_type);
    }

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Compute the current map (x,y) of this tile
            int x = i + explorer.loc_x;
            int y = j + explorer.loc_y;

            // Compute the previous map (px, py) of this tile
            int px = i + explorer.loc_x_prev;
            int py = j + explorer.loc_y_prev;

            DrawFunc draw = NULL;
            
            if (!explorer.nv_active) {
                if (abs(i) > 3 || abs(j) > 3) {
                    draw_nothing(u,v);
                    continue;
                } else if (abs(i) == 3 || abs(j) == 3) {
                    draw_dark_outline(u,v);
                    continue;
                }
            }
            if (i == 0 && j == 0) continue; // don't draw on explorer tile
            
            // Figure out what to draw
            // This code should be used for all tiles that are visible
            if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (draw_option || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it normally
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                } else if (curr_item && curr_item->type == CLEAR) {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            } else if (draw_option) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }
            

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    draw_explorer(58, 56, explorer.direction, explorer.sprite_type);
    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map with walls and other items.
 */
void init_main_map()
{
    
    // the game is run according to the pdf specs.
    
    printf("Adding walls!\r\n");
    add_wall(            0,              0, HORIZONTAL, map_width());
    add_wall(            0, map_height()-1, HORIZONTAL, map_width());
    add_wall(            0,              0,   VERTICAL, map_height());
    add_wall(map_width()-1,              0,   VERTICAL, map_height());
    printf("Walls done!\r\n");
    
    add_explorer(explorer.loc_x, explorer.loc_y);
    
    printf("Adding boss chamber\r\n");
    add_wall(30,  0,   VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39,  0,   VERTICAL, 10);
    printf("Added!\r\n");
    
    // Create maze layout
    printf("Creating maze...\r\n");
    
    // Large starting room around spawn point 
    add_wall(2, 2, HORIZONTAL, 7);  
    add_wall(2, 9, HORIZONTAL, 7);     
    add_wall(2, 2, VERTICAL, 7);     
    add_wall(9, 2, VERTICAL, 7);      
    printf("Starting room created!\r\n");
    
    // Create openings in starting room
    map_erase(5, 2); 
    map_erase(9, 5);   
    map_erase(5, 9);   
    
    // Medium room
    add_wall(12, 3, HORIZONTAL, 6);  
    add_wall(12, 8, HORIZONTAL, 6);  
    add_wall(12, 3, VERTICAL, 5);    
    add_wall(18, 3, VERTICAL, 5);    
    map_erase(15, 3);  
    map_erase(12, 5); 
    printf("Medium room 1 created!\r\n");
    
    // Connecting corridor 
    add_wall(10, 4, HORIZONTAL, 2);   
    add_wall(10, 6, HORIZONTAL, 2);   
    printf("Corridor 1 created!\r\n");
    
    // Small room 
    add_wall(20, 5, HORIZONTAL, 4);
    add_wall(20, 9, HORIZONTAL, 4);
    add_wall(20, 5, VERTICAL, 4);
    add_wall(24, 5, VERTICAL, 4);
    map_erase(22, 5);  
    map_erase(20, 7);  
    printf("Small room created!\r\n");
    
    // Connecting corridor 
    add_wall(18, 6, HORIZONTAL, 2);
    map_erase(19, 7);  // opening
    printf("Corridor to small room created!\r\n");
    
    // Lower area
    add_wall(4, 12, HORIZONTAL, 6);  
    add_wall(4, 17, HORIZONTAL, 6);   
    add_wall(4, 12, VERTICAL, 5);     
    add_wall(10, 12, VERTICAL, 5);    
    map_erase(7, 12);  
    map_erase(10, 14); 
    printf("Lower area created!\r\n");
    
    // Simple vertical connecting corridor from starting room
    add_wall(5, 10, VERTICAL, 2);
    printf("Vertical corridor created!\r\n");
    
    // Medium room
    add_wall(13, 12, HORIZONTAL, 5);
    add_wall(13, 16, HORIZONTAL, 5);
    add_wall(13, 12, VERTICAL, 4);
    add_wall(18, 12, VERTICAL, 4);
    map_erase(15, 12); 
    map_erase(13, 14); 
    printf("Medium room 2 created!\r\n");
    
    // Connecting corridor between medium rooms
    add_wall(11, 13, HORIZONTAL, 2);
    add_wall(11, 15, HORIZONTAL, 2);
    printf("Inter-room corridor created!\r\n");
    
    // Large exploration chamber
    add_wall(21, 10, HORIZONTAL, 6);
    add_wall(21, 16, HORIZONTAL, 6);
    add_wall(21, 10, VERTICAL, 6);
    add_wall(27, 10, VERTICAL, 6);
    map_erase(24, 10); 
    map_erase(21, 13); 
    printf("Large exploration chamber created!\r\n");
    
    // Connecting corridor to large chamber
    add_wall(19, 12, HORIZONTAL, 2);
    add_wall(19, 14, HORIZONTAL, 2);
    printf("Chamber connector created!\r\n");
    
    // Right side room
    add_wall(32, 14, HORIZONTAL, 3);
    add_wall(32, 18, HORIZONTAL, 3);
    add_wall(32, 14, VERTICAL, 4);
    add_wall(35, 14, VERTICAL, 4);
    map_erase(33, 14); 
    map_erase(32, 16); 
    printf("Right side room created!\r\n");
    
    // Simple connecting corridor to right area
    add_wall(28, 15, HORIZONTAL, 4);
    printf("Right area connector created!\r\n");
    add_wall(6, 19, HORIZONTAL, 6);
    add_wall(6, 19, VERTICAL, 4);
    add_wall(12, 19, VERTICAL, 4);
    printf("Bottom medium room created!\r\n");
    add_wall(8, 17, VERTICAL, 2);
    printf("Bottom connector created!\r\n");
    add_wall(29, 17, HORIZONTAL, 2);
    printf("Bottom right connector created!\r\n");
    add_wall(36, 15, VERTICAL, 3);
    add_wall(36, 18, HORIZONTAL, 2);
    printf("Bottom right dead end created!\r\n");
    map_erase(5, 10);   
    map_erase(5, 11);   
   
    printf("Cave maze completed successfully!\r\n");

    // TODO: Place night vision, heal, poison, and other items at random each time
    add_N_items(NIGHTVISION, 10);
    add_N_items(HEAL, 5);
    add_N_items(POISON, 5);
    add_N_items(PICKAXE, 7);
    add_N_items(WILDCARD, 7);
    add_N_items(SCRAMBLER, 7);
    
    print_map();
}

// Feel free to add any helper functions below or elsewhere in the document

/**
 * Helper function to calcualte random X coordinate.
 */
int random_X() {
    return random() % map_width();
}
/**
 * Helper function to calculate random Y coordinate.
 */
int random_Y() {
    return random() % map_height();
}

/**
 * Helper function to add N type items to the Map without overwriting existing MapItems.
 */
void add_N_items(int type, int count) {
    // Define function pointer type for item adders
    typedef void (*AddFunc)(int, int);
    
    // Map item type to corresponding add function
    AddFunc add_func = NULL;
    switch (type) {
        case NIGHTVISION:
            add_func = add_nightvision;
            break;
        case HEAL:
            add_func = add_heal;
            break;
        case POISON:
            add_func = add_poison;
            break;
        case PICKAXE:
            add_func = add_pickaxe;
            break;
        case WILDCARD:
            add_func = add_wildcard;
            break;
        case SCRAMBLER:
            add_func = add_scrambler;
            break;
        default:
            return;
    }
    
    int added = 0;
    while (added < count) {
        int X = random_X();
        int Y = random_Y();
        MapItem* randItem = get_current(X, Y);
        if (!randItem) { // do not overwrite existing item
            add_func(X, Y); // Call the function pointer
            added++;
        }
    }
}

/**
 * Helper function to check if night vision duration has expired.
 * Disables night vision after 10 seconds.
 * Returns FULL_DRAW if night vision expired this frame, NO_RESULT otherwise.
 */
void check_night_vision_timer()
{
    if (explorer.nv_active) {
        uint64_t current_time = us_ticker_read();
        uint64_t elapsed_us = current_time - explorer.nv_start;
        // Disable after 5 seconds (5000000 us)
        if (elapsed_us > 5000000) {
            explorer.nv_active = 0;
        }
    }
}

/**
 * Check if scrambler duration has expired and disable it.
 */
void check_scrambler_timer()
{
    if (explorer.scrambler_active) {
        uint64_t current_time = us_ticker_read();
        uint64_t elapsed_us = current_time - explorer.scrambler_start;
        // Disable after 5 seconds (7000000 us)
        if (elapsed_us > 5000000) {
            explorer.scrambler_active = 0;
        }
    }
}

/**
 * Check for game over or win conditions and handle accordingly.
 */
void check_game_over(int result) {
    // Check if explorer has run out of lives
    if (explorer.lives <= 0) {
        // Update high score if current score is higher
        if (explorer.score > high_score) {
            high_score = explorer.score;
        }
        
        uLCD.cls();
        uLCD.locate(3, 5);
        uLCD.color(RED);
        uLCD.printf("GAME OVER");
        uLCD.locate(2, 7);
        uLCD.printf("Final Score: %d", explorer.score);
        uLCD.locate(1, 9);
        uLCD.color(WHITE);
        uLCD.printf("Press any button");
        uLCD.locate(2, 10);
        uLCD.printf("to restart...");
        
        // Wait for button press
        while(1) {
            GameInputs inputs = read_inputs();
            if (inputs.up || inputs.down || inputs.right || inputs.left || 
                inputs.center || inputs.b1 || inputs.b2 || inputs.b3) {
                restart_game = true; // Signal restart
                return;
            }
            wait_us(100000); // Small delay to prevent busy-waiting
        }
    }
    
    // Check if explorer won (result == WIN)
    if (result == WIN) {
        // Update high score if current score is higher
        if (explorer.score > high_score) {
            high_score = explorer.score;
        }
        
        uLCD.cls();
        uLCD.locate(3, 5);
        uLCD.color(GREEN);
        uLCD.printf("YOU WIN!");
        uLCD.locate(2, 7);
        uLCD.printf("Final Score: %d", explorer.score);
        uLCD.locate(1, 9);
        uLCD.color(WHITE);
        uLCD.printf("Press any button");
        uLCD.locate(2, 10);
        uLCD.printf("to restart...");
        
        // Wait for button press
        while(1) {
            GameInputs inputs = read_inputs();
            if (inputs.up || inputs.down || inputs.right || inputs.left || 
                inputs.center || inputs.b1 || inputs.b2 || inputs.b3) {
                restart_game = true; // Signal restart
                return;
            }
            wait_us(100000); // Small delay to prevent busy-waiting
        }
    } 
}

/**
 * Helper function for spawning the teleporter
 */
void check_spawn_teleporter(int result) {
    if (teleporter_spawned) return;
    if (explorer.score < 100) return;
    int teleporter_locations[8][2] = { {5,4}, {6,4}, {6,5}, {6,6}, {5,6}, {4,6}, {4,5}, {4,4} };
    for (int i = 0; i < 8; i++) {
        int x = teleporter_locations[i][0];
        int y = teleporter_locations[i][1];
        if (!get_current(x, y)) {
            add_teleporter(x, y, 34, 3);
            teleporter_spawned = true;
            return;
        }
    }
}

void print_instructions() {
    uLCD.color(RED);
    uLCD.locate(0,0);  uLCD.printf("== Instructions ==");
    uLCD.color(WHITE);
    uLCD.locate(0,2);  uLCD.printf("Explore the cave");
    uLCD.locate(0,3);  uLCD.printf("& survive hazards.");
    uLCD.locate(0,5);  uLCD.printf("Move: Nav switch");
    uLCD.locate(0,7);  uLCD.printf("Center: Start game");
    uLCD.color(GREEN);
    uLCD.locate(0,9);  uLCD.printf("NV: Temp vision");
    uLCD.locate(0,10); uLCD.printf("Heal: +1 life");
    uLCD.locate(0,11); uLCD.printf("Poison: -1 life");
}

void print_start_page() {
    uLCD.color(RED);
    uLCD.locate(0,0);  uLCD.printf("== CaveExplorer ==");
    uLCD.color(WHITE);
    uLCD.locate(0,2);  uLCD.printf("Center: Start Game");
    uLCD.locate(0,4);  uLCD.printf("B1: Instructions");
    uLCD.color(0xFFFF00); // Yellow
    uLCD.locate(0,6);  uLCD.printf("High Score: %d", high_score);
    uLCD.color(GREEN);
    uLCD.locate(0,8);  uLCD.printf("Tips:");
    uLCD.locate(0,9);  uLCD.printf("Collect NV items");
    uLCD.locate(0,10); uLCD.printf("Avoid poison!");
}

void draw_pause_overlay() {
    // Draw semi-transparent overlay box in center
    uLCD.filled_rectangle(20, 40, 107, 80, BLACK);
    uLCD.rectangle(20, 40, 107, 80, WHITE);
    
    uLCD.color(RED);
    uLCD.locate(6,7);
    uLCD.printf("PAUSED");
    
}

void display_stats()
{
    uLCD.filled_rectangle(0, 0, 160, 7, BLACK);
    char str[1024];
    snprintf(str, sizeof(str), "(%d,%d) S:%d L:%d", explorer.loc_x, explorer.loc_y, explorer.score, explorer.lives); // need position, score, and lives, UNCOMMENT WHEN DONE
    uLCD.text_string(str, 0, 0, FONT_7X8, RED);
}

void draw_wildcard_popup(const char* item_name)
{
    // Draw centered popup box
    uLCD.filled_rectangle(15, 35, 112, 85, BLACK);
    uLCD.rectangle(15, 35, 112, 85, WHITE);
    
    uLCD.color(0xFFFF00); // Yellow
    uLCD.locate(2, 5);
    uLCD.printf("WILDCARD!");
    
    uLCD.color(GREEN);
    uLCD.locate(2, 7);
    uLCD.printf("You got:");
    
    uLCD.color(WHITE);
    uLCD.locate(2, 9);
    uLCD.printf("%s", item_name);
    
    // Persist until any user input is detected
    wait_us(120000);
    while (1) {
        GameInputs in = read_inputs();
        if (in.up || in.down || in.left || in.right || in.center || in.b1 || in.b2 || in.b3 || in.b4) {
            // Debounce and exit
            wait_us(120000);
            break;
        }
        wait_us(20000);
    }
}