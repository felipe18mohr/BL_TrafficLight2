#include "mbed.h"

DigitalOut leds[] = {(p6), (p8), (p7)};
InterruptIn but(p5);
Timeout time_led;
Timeout time_pressed;

enum states{RED, GREEN, YEL};
float times[] = {3.0, 4.0, 2.0};
int state = RED;
int last_state = state;
bool pressed;

void but_rise();
void but_fall();
void next_led();
void turn_off();
void turn_on();
void allert();
void blink();

int main(){
    leds[state] = 1;
    but.rise(&but_rise);
    but.fall(&but_fall);
    time_led.attach(&next_led, times[state]);
}

void but_rise(){
    pressed = true;
    if(state != 4) time_pressed.attach(&allert, 2.0);
    else time_pressed.attach(&turn_on, 4.0);
}

void but_fall(){
    pressed = false;
    if(state == GREEN) next_led();
}

void next_led(){
    leds[state] = 0;
    if(state<2) state++;
    else state = 0;
    leds[state] = 1;
    time_led.attach(&next_led, times[state]);
    last_state = state;
}

void allert(){
    if(pressed){
        if(state !=3){
            state = 3;
            for(int i=0; i<3; i++) leds[i]=0;
            time_led.attach(&blink, 0.5);
            time_pressed.attach(&turn_off, 2.0);
        }
        else{
            for(int i=0; i<3; i++) leds[i]=0;
            state = last_state - 1;
            next_led();
            time_pressed.attach(&turn_off, 2.0);
        }
    }    
}

void blink(){
    leds[YEL] = !leds[YEL];
    time_led.attach(&blink, 0.5);
}

void turn_off(){
    if(pressed){
        for(int i=0; i<3; i++) leds[i]=0;
        state = 4;
        time_led.detach();
        time_pressed.attach(&turn_on, 4.0);
    }
}

void turn_on(){
    if(pressed){
        next_led();
        time_pressed.attach(&turn_on, 4.0);
    }
}
