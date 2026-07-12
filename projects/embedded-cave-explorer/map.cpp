// ============================================
// The Map class file
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "map.h"

#include "globals.h"
#include "graphics.h"
#define MHF_NBUCKETS 97

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define NUM_MAPS 1

static Map map;
static int active_map;

static const MapItem CLEAR_SENTINEL = {
    .type = CLEAR,
    .draw = draw_nothing
};

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    
    // TODO: Implement 
    // Hint: Think about how you accessed location in Project 1
    // 1. Return the 1-D xy key
    return Y*map.w + X;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    
    // TODO: Implement 
    // Hint: what math operator can be used as an easy way to hash a key?
    // 1. Use MHF_NBUCKETS as part of your hash function
    // 2. Return the hashed key
    return key % MHF_NBUCKETS;
}

/**
 * Initializes the map, using a hash_table, setting the width and height.
 */
void map_init()
{
    // Destroy existing hash table if it exists (for restart)
    if (map.items != NULL) {
        destroyHashTable(map.items);
    }
    
    map.items = createHashTable(map_hash, MHF_NBUCKETS);
    map.w = 40;
    map.h = 25;
}

Map* get_active_map()
{
    return &map;
}

void print_map()
{
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item && item->type < 8) printf("%c", lookup[item->type]);
            else printf(" ");
        }
        printf("\r\n");
    }
}

/**
 * Returns width of active map
 */
int map_width()
{
    Map* currMap = get_active_map();
    return currMap->w;
}

/**
 * Returns height of active map
 */
int map_height()
{
    Map* currMap = get_active_map();
    return currMap->h;
}

/**
 * Returns the area of the active map
 */
int map_area()
{
    Map* currMap = get_active_map();
    return currMap->w * currMap->h;    

}

/**
 * Returns MapItem at current coordinate location
 */
MapItem* get_current(int x, int y)
{
    HashTable* ht = map.items;
    unsigned int key = XY_KEY(x, y);

    MapItem* currItem = (MapItem*) getItem(ht, key);
    if (currItem != NULL && currItem->type == CLEAR) {
        return (MapItem*) removeItem(ht, key);
    }
    return (MapItem*) getItem(ht, key);
    // TODO: Implement  
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
}

/**
 * Returns the MapItem immediately above the given location.
 */
MapItem* get_north(int x, int y) // get north item
{
    HashTable* ht = map.items;
    unsigned int key = XY_KEY(x, y-1);

    MapItem* currItem = (MapItem*) getItem(ht, key);
    if (currItem != NULL && currItem->type == CLEAR) {
        return (MapItem*) removeItem(ht, key);
    }
    return (MapItem*) getItem(ht, key);
    // TODO: Implement
    // 1. Get map item 
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item 
}

/**
 * Returns the MapItem immediately below the given location.
 */
MapItem* get_south(int x, int y) // get south item
{
    HashTable* ht = map.items;
    unsigned int key = XY_KEY(x, y+1);

    MapItem* currItem = (MapItem*) getItem(ht, key);
    if (currItem != NULL && currItem->type == CLEAR) {
        return (MapItem*) removeItem(ht, key);
    }
    return (MapItem*) getItem(ht, key);
    // TODO: Implement 
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
}

/**
 * Returns the MapItem immediately right the given location.
 */
MapItem* get_east(int x, int y) // get east item
{
    HashTable* ht = map.items;
    unsigned int key = XY_KEY(x+1, y);

    MapItem* currItem = (MapItem*) getItem(ht, key);
    if (currItem != NULL && currItem->type == CLEAR) {
        return (MapItem*) removeItem(ht, key);
    }
    return (MapItem*) getItem(ht, key);
    // TODO: Implement 
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
}

/**
 * Returns the MapItem immediately left the given location.
 */
MapItem* get_west(int x, int y) // get west item
{
   HashTable* ht = map.items;
    unsigned int key = XY_KEY(x-1, y);

    MapItem* currItem = (MapItem*) getItem(ht, key);
    if (currItem != NULL && currItem->type == CLEAR) {
        return (MapItem*) removeItem(ht, key);
    }
    return (MapItem*) getItem(ht, key);
    // TODO: Implement 
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item 
}

/**
 * Returns the MapItem at current coordinate location
 */
 MapItem* get_here(int x, int y) // same as get_current
 {
    HashTable* ht = map.items;
    unsigned int key = XY_KEY(x, y);

    MapItem* currItem = (MapItem*) getItem(ht, key);
    if (currItem != NULL && currItem->type == CLEAR) {
        return (MapItem*) removeItem(ht, key);
    }
    return (MapItem*) getItem(ht, key);
     // TODO: Implement  
     // HINT: This is the same as get_current()
     // You may copy the same code from get_current for get_here
     // 1. Get map item
     // 2. Check and remove item if item exist and is clear type
     // 3. Return the item
 }

/**
 * Erase current location map item
 */
void map_erase(int x, int y)
{
    HashTable* ht = map.items;
    unsigned int key = XY_KEY(x, y);
    MapItem* clearItem = (MapItem*) malloc(sizeof(MapItem));
    clearItem->type = CLEAR;
    clearItem->draw = draw_nothing;
    clearItem->walkable = true;
    clearItem->data = NULL;
    void* val = insertItem(ht, key, clearItem); 
    if (val) free(val);

    // TODO: Implement  
    //1. Insert clear value to the map item at current location
    //2. Free current location map item if it exists
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_nightvision(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NIGHTVISION;
    w1->draw = draw_nightvision;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_explorer(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = EXPLORER;
    w1->draw = draw_explorer_init;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_heal(int x, int y) // code to add a heal item
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = HEAL;
    w1->draw = draw_heal;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_poison(int x, int y) // code to add a poison
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = POISON;
    w1->draw = draw_poison;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_teleporter(int x, int y, int dest_x, int dest_y) // code to add a simple teleporter
{
    MapItem* teleporter = (MapItem*) malloc(sizeof(MapItem));
    teleporter->type = TELEPORTER;
    teleporter->draw = draw_teleporter;
    teleporter->walkable = true;
    teleporter->data = NULL; //XY_KEY(dest_x, dest_y);
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), teleporter);
    if (val) free(val); // If something is already there, free it
}

void add_pickaxe(int x, int y)
{
    MapItem* pickaxe = (MapItem*) malloc(sizeof(MapItem));
    pickaxe->type = PICKAXE;
    pickaxe->draw = draw_pickaxe;
    pickaxe->walkable = true;
    pickaxe->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), pickaxe);
    if (val) free(val); // If something is already there, free it
}

void add_wildcard(int x, int y)
{
    MapItem* wildcard = (MapItem*) malloc(sizeof(MapItem));
    wildcard->type = WILDCARD;
    wildcard->draw = draw_wildcard;
    wildcard->walkable = true;
    wildcard->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), wildcard);
    if (val) free(val); // If something is already there, free it
}

void add_scrambler(int x, int y)
{
    MapItem* scrambler = (MapItem*) malloc(sizeof(MapItem));
    scrambler->type = SCRAMBLER;
    scrambler->draw = draw_scrambler;
    scrambler->walkable = true;
    scrambler->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), scrambler);
    if (val) free(val); // If something is already there, free it
}
