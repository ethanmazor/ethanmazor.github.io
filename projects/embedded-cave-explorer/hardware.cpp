// This header has all the (extern) declarations of the globals.
// "extern" means "this is instantiated somewhere, but here's what the name
// means.
#include "globals.h"
#include "hardware.h"

// We need to actually instantiate all of the globals (i.e. declare them once
// without the extern keyword). That's what this file does!

// Hardware initialization: Instantiate all the things!
uLCD_4DGL uLCD(p9,p10,p11);             // LCD Screen (tx, rx, reset)
Nav_Switch myNav(p12, p15, p14, p16, p13); // initialize nav switch
BufferedSerial pc(USBTX, USBRX, 115200);                  // USB Console (tx, rx)

DigitalIn button1(p24);
DigitalIn button2(p23);
DigitalIn button3(p22);
DigitalIn button4(p21);                 // Pushbuttons (pin)



AnalogOut DACout(p18);                  // Speaker (pin)
PwmOut speaker(p25); 

int hardware_init()
{    
    button1.mode(PullUp); 
    button2.mode(PullUp);
    button3.mode(PullUp);
    button4.mode(PullUp);
    return ERROR_NONE;
}

GameInputs read_inputs() 
{
    GameInputs in;
    in = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    // 1. read and invert the buttons (the buttons are active-low)
    in.b1 = !button1.read();
    in.b2 = !button2.read();
    in.b3 = !button3.read();
    in.b4 = !button4.read();

    // 2. read the navswitch
    in.up = myNav.up();
    in.down = myNav.down();
    in.left= myNav.left();
    in.right = myNav.right();
    in.center = myNav.center();
    return in;
}


// A shortcut to serial port print
void print(const char* c)
{
    pc.write(c, strlen(c));
}