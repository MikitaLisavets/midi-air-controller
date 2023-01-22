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

void set_selected_row(int row) {
  if (row < 0) {
    row = global_max_rows + row;
  }

  global_selected_row = row % global_max_rows;
}

void render_display(int current_distance, int current_note, int root_note, int scale_index, int number_of_notes, int midi_channel, bool is_pitch, int selected_row) {
  int font_width = 6;
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.print("D: " + String(current_distance));
  String current_note_name = get_note_name(current_note);
  String right_text = "N: " + String(current_note_name);
  display.setCursor(SCREEN_WIDTH - (right_text.length() * font_width), 0);
  display.print(right_text);
  display.println("---------------------");

  if (selected_row == 0) {
    style_selected_row();
  } else {
    style_default_row();
  }
  String root_note_name = get_note_name(root_note);
  display.println("Root note: " + root_note_name);

  if (selected_row == 1) {
    style_selected_row();
  } else {
    style_default_row();
  }
  String scale_name = get_scale_name(scale_index);
  display.println("Scale: " + scale_name);

  if (selected_row == 2) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.println("Notes: " + String(number_of_notes));

  if (selected_row == 3) {
    style_selected_row();
  } else {
    style_default_row();
  }
  display.println("MIDI Channel: " + String(midi_channel));

  if (selected_row == 4) {
    style_selected_row();
  } else {
    style_default_row();
  }
  String type = is_pitch ? "Pitch" : "Step";
  display.println("Type: " + type);

  display.display();
}

void loop_display() {
  // render_display(
  //   global_current_distance,
  //   global_current_note,
  //   global_root_note,
  //   global_current_scale_index,
  //   global_number_of_notes,
  //   global_midi_channel,
  //   global_is_pitch,
  //   global_selected_row
  // );
}