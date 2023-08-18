#include <display.h>

void setup_display() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
}

const char* get_note_name(int8_t note) {
  if (note == -1) {
    return "";
  } else {
    switch(note % 12) {
      case 0: return "C";
      case 1: return "C#";
      case 2: return "D";
      case 3: return "D#";
      case 4: return "E";
      case 5: return "F";
      case 6: return "F#";
      case 7: return "G";
      case 8: return "G#";
      case 9: return "A";
      case 10: return "A#";
      case 11: return "B";
      default: return "";
    }
  }
}

int8_t get_note_octave(int8_t note) {
  if (note == -1) {
    return 0;
  } else {
    return round(note / 12);
  }
}

const char* get_mode_name(int8_t mode) {
  switch(mode) {
    case MODE_NOTE: return "Note";
    case MODE_CC: return "CC";
    case MODE_CC_INVERTED: return "CC (Inv)";
    case MODE_VELOCITY: return "Velocity";
    case MODE_VELOCITY_INVERTED: return "Velocity (Inv)";
    default: return "";
  }
}

const char* get_scale_name(int8_t index) {
  return (const char*)pgm_read_word(&SCALES[index].name);
}

void render_top_bar() {
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 2; i++) {
    if (i == RIGHT_SIDE) {
      display.setCursor(SCREEN_WIDTH - 6 * SCREEN_FONT_WIDTH, 0);
    }

    switch(settings.mode[i]) {
      case MODE_NOTE:
        display.print(F("N: "));
        display.print(get_note_name(global_current_note[i]));
        display.print(get_note_octave(global_current_note[i]));
        break;
      case MODE_CC:
      case MODE_CC_INVERTED:
        display.print(F("C: "));
        display.print(global_current_control_value[i]);
        break;
      case MODE_VELOCITY:
      case MODE_VELOCITY_INVERTED:
        display.print(F("V: "));
        display.print(global_velocity);
        break;
    }
  }

  display.setCursor((SCREEN_WIDTH / 2) - 15, 0);
  display.print(settings.side == LEFT_SIDE ? F("LEFT") : F("RIGHT"));

  display.println();
  display.println(F("---------------------"));
}

void render_menu() {
  display.setCursor(0, SCREEN_FONT_HEIGHT * 2);

  for (byte i = 0; i < SCREEN_MENU_ROWS; i++) {
    if (global_menu_selected_row < SCREEN_MENU_ROWS) {
      render_row(i);
    } else {
      render_row(global_menu_selected_row - SCREEN_MENU_ROWS + 1 + i);  
    }
  }
}
 
void render_row(int8_t row_index) {
  bool is_selected = row_index == global_menu_selected_row;
  if (is_selected) {
    display.setTextColor(BLACK, WHITE);
  } else {
    display.setTextColor(WHITE);
  }

  switch(row_index) {
    case MENU_SIDE: return render_row_side();
    case MENU_ROOT_NOTE: return render_row_root_note();
    case MENU_SCALE: return render_row_scale();
    case MENU_MODE: return render_row_mode();
    case MENU_NOTES: return render_row_notes();
    case MENU_DISTANCE_STEP: return render_row_distance_step();
    case MENU_NOTE_TIMEOUT: return render_row_note_timeout();
    case MENU_MIDI: return render_row_midi();
    case MENU_CC: return render_row_control_change();
    case MENU_LOAD: return render_row_load();
    case MENU_SAVE: return render_row_save();
    case MENU_RESET: return render_row_reset();
    case MENU_AUTO_LOAD_SETTINGS: return render_row_auto_load_settings();
  }
}

void render_row_side() {
  display.print(F("Side: "));
  display.println(settings.side == LEFT_SIDE ? F("Left") : F("Right"));
}

void render_row_root_note() {
  display.print(F("Root note: "));
  display.print(get_note_name(settings.root_note[settings.side]));
  display.println(get_note_octave(settings.root_note[settings.side]));
}

void render_row_scale() {
  display.print(F("Scale: "));
  display.println(get_scale_name(settings.scale_index[settings.side]));
}

void render_row_mode() {
  display.print(F("Mode: "));
  display.println(get_mode_name(settings.mode[settings.side]));
}

void render_row_notes() {
  display.print(F("Notes: "));
  display.println(settings.number_of_notes[settings.side]);
}

void render_row_distance_step() {
  display.print(F("Distance step: "));
  display.println(settings.distance_step[settings.side]);
}

void render_row_note_timeout() {
  display.print(F("Note timeout: "));
  display.println(settings.note_timeout[settings.side]);
}

void render_row_midi() {
  display.print(F("MIDI Channel: "));
  display.println(settings.midi_channel[settings.side]);
}

void render_row_control_change() {
  display.print(F("Control change: "));
  display.println(settings.control_change[settings.side]);
}

void render_row_load() {
  display.println(F("Load Settings"));
}

void render_row_save() {
  display.println(F("Save Settings"));
}

void render_row_reset() {
  display.println(F("Reset Settings"));
}

void render_row_auto_load_settings() {
  display.print(F("Auto-Load: "));
  display.println(settings.autoLoadSettings ? F("Yes") : F("No"));
}

void render_save_screen(){
  clear_display();
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println(F("Saving..."));
  display.display();
  display.setTextSize(1);
  delay(1000);
}

void render_load_screen(){
  clear_display();
  display.setTextSize(2);
  display.setCursor(5, 20);
  display.println(F("Loading..."));
  display.display();
  display.setTextSize(1);
  delay(1000);
}

void render_reset_screen(){
  clear_display();
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.println(F("Reset..."));
  display.display();
  display.setTextSize(1);
  delay(1000);
}

void render_init_screen() {
  clear_display();
  display.print(F("Version: "));
  display.print(VERSION);
  display.display();

  display.setTextSize(2);
  display.setCursor(30, 20);
  display.print(F("X"));
  display.display();
  delay(100);
  display.print(F("-"));
  display.display();
  delay(100);
  display.print(F("A"));
  display.display();
  delay(100);
  display.print(F("i"));
  display.display();
  delay(100);
  display.print(F("r"));
  display.display();
  delay(1000);

  clear_display();
}

void clear_display() {
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
}

void loop_display() {
  if (global_is_display_enabled) {
    display.clearDisplay();
    render_top_bar();
    render_menu();
    display.display();
  }
}