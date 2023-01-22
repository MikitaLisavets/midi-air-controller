#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup_display() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
}

void clear_display() {
  display.clearDisplay();
}

void set_display_status_bar(String left, String right) {
  int font_width = 6;

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(left);
  display.setCursor(SCREEN_WIDTH - (right.length() * font_width) , 0);
  display.print(right);
  display.println();
  display.println();
}

void set_display_main (String root_note_name, String scale_name) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("(U/D) Root Note: " + root_note_name);
  display.println("--------------------");
  display.println("(L/R) Scale: " + scale_name);
  display.println("--------------------");
}

void render_display() {
  display.display();
}