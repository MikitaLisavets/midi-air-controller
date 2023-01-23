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

void render_display() {
  byte font_width = 6;
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.print(F("Dest: "));
  if (global_current_distance == -1) {
    display.print(F("-"));    
  } else {
    display.print(global_current_distance);
  }
  display.print(F(" | "));

  if (global_mode == MODE_NOTE) {
    display.print(F("Note: "));
    display.println(get_note_name(global_current_note));
  } else if (global_mode == MODE_VALUE || global_mode == MODE_VALUE_INVERTED) {
    display.print(F("Val: "));
    display.println(global_current_control_value);
  }

  display.println(F("---------------------"));

  if (global_selected_row == 0) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print(F("Root note: "));
  display.println(get_note_name(global_root_note));

  if (global_selected_row == 1) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print(F("Scale: "));
  display.println(get_scale_name(global_current_scale_index));

  if (global_selected_row == 2) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print(F("Notes: "));
  display.println(global_number_of_notes);

  if (global_selected_row == 3) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print(F("MIDI Channel: "));
  display.println(global_midi_channel);

  if (global_selected_row == 4) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print(F("Distance step: "));
  display.println(global_distance_step);

  if (global_selected_row == 5) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.print(F("Mode: "));
  display.println(get_mode_name(global_mode));

  display.display();
}