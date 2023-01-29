#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <main.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;
const uint8_t OLED_RESET = 4;
const uint8_t SCREEN_ADDRESS = 0x3C;
const uint8_t SCREEN_MENU_ROWS = 6;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup_display();
void loop_display();

#endif