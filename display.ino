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

void style_selected_row() {
  display.setTextColor(BLACK, WHITE);
}

void style_default_row() {
  display.setTextColor(WHITE);
}

void render_display(byte current_distance, byte current_note, byte root_note, byte scale_index, byte number_of_notes, byte midi_channel, bool is_pitch, byte selected_row) {
  byte font_width = 6;
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.print("D: ");
  display.print(current_distance);
  display.print(" | ");
  display.print("N: ");
  display.println(get_note_name(current_note));
  display.println("---------------------");

  if (selected_row == 0) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print("Root note: ");
  display.println(get_note_name(root_note));

  if (selected_row == 1) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print("Scale: ");
  display.println(get_scale_name(scale_index));

  if (selected_row == 2) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print("Notes: ");
  display.println(number_of_notes);

  if (selected_row == 3) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print("MIDI Channel: ");
  display.println(midi_channel);

  if (selected_row == 4) {
    style_selected_row();
  } else {
    style_default_row();
  }
  char* type = is_pitch ? "Pitch" : "Step";
  display.print("Type: ");
  display.println(type);

  if (selected_row == 5) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print("Distance step: ");
  display.println(global_distance_step);

  display.display();
}