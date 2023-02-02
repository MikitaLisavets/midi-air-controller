#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <main.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;
const uint8_t SCREEN_ADDRESS = 0x3C;
const uint8_t SCREEN_MENU_ROWS = 6;
const uint8_t SCREEN_FONT_HEIGHT = 8;
const uint8_t SCREEN_FONT_WIDTH = 6;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup_display();
void loop_display();
void clear_display();

#endif