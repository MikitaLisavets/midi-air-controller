#ifndef __CONTROLS_H
#define __CONTROLS_H

#include <main.h>

const uint8_t BUTTON_UP = 6;
const uint8_t BUTTON_DOWN = 9;
const uint8_t BUTTON_LEFT = 7;
const uint8_t BUTTON_RIGHT = 8;

const uint8_t UP = 1;
const uint8_t DOWN = 2;
const uint8_t LEFT = 3;
const uint8_t RIGHT = 4;
const uint8_t PRESS = 5;
const uint8_t NONE = 0;

const uint16_t TIMER_TIMEOUT = 200;

const uint16_t CENTER_VALUE = 500;
const uint16_t THRESHOLD = 250;

uint8_t control_status = NONE;
uint32_t timer;

void setup_controls();
void loop_controls();

#endif