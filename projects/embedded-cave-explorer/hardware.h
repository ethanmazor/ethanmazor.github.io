#ifndef HARDWARE_H
#define HARDWARE_H

// Structure that represents all the inputs to the game.
// If additional hardware is added, new elements should be added to this struct.
struct GameInputs {
    int up, down, left, right, center;     // Button presses
    int b1, b2, b3, b4;
};

int hardware_init();

GameInputs read_inputs();

void print(const char* c);
#endif // HARDWARE_H