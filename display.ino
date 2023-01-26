#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C
#define SCREEN_MENU_ROWS 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

byte font_width = 6;

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

String get_note_name(int note) {
  if (note == -1) {
    return "-";
  } else {
    return global_note_names[note % 12] + (char)round(note / 12);
  }
}

char* get_mode_name(int mode) {
  switch(mode) {
    case MODE_L_NOTE_R_CC: return "L:N | R:CC";
    case MODE_L_NOTE_R_CC_INVERTED: return "L:N | R:CC(I)";
    case MODE_L_NOTE_R_VELOCITY: return "L:N | R:V";
    case MODE_L_NOTE_R_VELOCITY_INVERTED: return "L:N | R:V(I)";
    case MODE_L_CC_R_NOTE: return "L:CC | R:N";
    case MODE_L_CC_INVERTED_R_NOTE: return "L:CC(I) | R:N";
    case MODE_L_VELOCITY_R_NOTE: return "L:V | R:N";
    case MODE_L_VELOCITY_INVERTED_R_NOTE: return "L:V(I) | R:N";
  }
}

char* get_scale_name(int index) {
  return global_scales_names[index];
}

void render_top_bar() {
  switch(global_mode) {
    case MODE_L_NOTE_R_CC:;
    case MODE_L_NOTE_R_CC_INVERTED:
      display.print(F("N: "));
      display.print(get_note_name(global_current_note));
      display.print(F(" | "));
      display.print(F("CC: "));
      display.print(global_current_control_value);
      break;
    case MODE_L_NOTE_R_VELOCITY:
    case MODE_L_NOTE_R_VELOCITY_INVERTED:
      display.print(F("N: "));
      display.print(get_note_name(global_current_note));
      display.print(F(" | "));
      display.print(F("V: "));
      display.print(global_velocity);
      break;
    case MODE_L_CC_R_NOTE:
    case MODE_L_CC_INVERTED_R_NOTE:
      display.print(F("CC: "));
      display.print(global_current_control_value);
      display.print(F(" | "));
      display.print(F("N: "));
      display.print(get_note_name(global_current_note));
      break;
    case MODE_L_VELOCITY_R_NOTE:
    case MODE_L_VELOCITY_INVERTED_R_NOTE:
      display.print(F("V: "));
      display.print(global_velocity);
      display.print(F(" | "));
      display.print(F("N: "));
      display.print(get_note_name(global_current_note));
      break;
  }

  display.println();
  display.println(F("---------------------"));
}

void render_menu() {
  for (byte i = 0; i < SCREEN_MENU_ROWS; i++) {
    if (global_selected_row < SCREEN_MENU_ROWS) {
      render_row(i);      
    } else {
      render_row(global_selected_row - SCREEN_MENU_ROWS + 1 + i);  
    }
  }
}
 
void render_row(byte row_index) {
  bool is_selected = row_index == global_selected_row;
  if (is_selected) {
    style_selected_row();
  } else {
    style_default_row();
  }

  switch(row_index) {
    case global_menu_root_note: return render_row_root_note();
    case global_menu_scale: return render_row_scale();
    case global_menu_mode: return render_row_mode();
    case global_menu_notes: return render_row_notes();
    case global_menu_distance_step: return render_row_distance_step();
    case global_menu_tempo: return render_row_tempo();
    case global_menu_midi: return render_row_midi();
    case global_menu_control_change: return render_row_control_change();
  }
}

void render_row_root_note() {
  display.print(F("Root note: "));
  display.println(get_note_name(global_root_note));
}

void render_row_scale() {
  display.print(F("Scale: "));
  display.println(get_scale_name(global_current_scale_index));
}

void render_row_mode() {
  display.print(F("Mode: "));
  display.println(get_mode_name(global_mode));
}

void render_row_notes() {
  display.print(F("Notes: "));
  display.println(global_number_of_notes);
}

void render_row_distance_step() {
  display.print(F("Distance step: "));
  display.println(global_distance_step);
}

void render_row_tempo() {
  display.print(F("Tempo: "));
  display.println(global_tempo);
}

void render_row_midi() {
  display.print(F("MIDI Channel: "));
  display.println(global_midi_channel);
}

void render_row_control_change() {
  display.print(F("Control change: "));
  display.println(global_control_change);
}

void loop_display() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  render_top_bar();

  render_menu();

  display.display();
}