#include <display.h>

void setup_display() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
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
    case MODE_L_NOTE_R_CC: return "L:N | R:CC";
    case MODE_L_NOTE_R_CC_INVERTED: return "L:N | R:CC(I)";
    case MODE_L_NOTE_R_VELOCITY: return "L:N | R:V";
    case MODE_L_NOTE_R_VELOCITY_INVERTED: return "L:N | R:V(I)";
    case MODE_L_CC_R_NOTE: return "L:CC | R:N";
    case MODE_L_CC_INVERTED_R_NOTE: return "L:CC(I) | R:N";
    case MODE_L_VELOCITY_R_NOTE: return "L:V | R:N";
    case MODE_L_VELOCITY_INVERTED_R_NOTE: return "L:V(I) | R:N";
    default: return "";
  }
}

const char* get_scale_name(int8_t index) {
  return (const char*)pgm_read_word(&SCALES[index].name);
}

void render_top_bar() {
  display.setTextColor(WHITE);

  switch(global_mode) {
    case MODE_L_NOTE_R_CC:;
    case MODE_L_NOTE_R_CC_INVERTED:
      display.print(F("N: "));
      display.print(get_note_name(global_current_note));
      display.print(get_note_octave(global_current_note));
      display.print(F(" | "));
      display.print(F("CC: "));
      display.print(global_current_control_value);
      break;
    case MODE_L_NOTE_R_VELOCITY:
    case MODE_L_NOTE_R_VELOCITY_INVERTED:
      display.print(F("N: "));
      display.print(get_note_name(global_current_note));
      display.print(get_note_octave(global_current_note));
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
      display.print(get_note_octave(global_current_note));
      break;
    case MODE_L_VELOCITY_R_NOTE:
    case MODE_L_VELOCITY_INVERTED_R_NOTE:
      display.print(F("V: "));
      display.print(global_velocity);
      display.print(F(" | "));
      display.print(F("N: "));
      display.print(get_note_name(global_current_note));
      display.print(get_note_octave(global_current_note));
      break;
  }

  display.println();
  display.println(F("---------------------"));
}

void render_menu() {
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
    case MENU_ROOT_NOTE: return render_row_root_note();
    case MENU_SCALE: return render_row_scale();
    case MENU_MODE: return render_row_mode();
    case MENU_NOTES: return render_row_notes();
    case MENU_DISTANCE_STEP: return render_row_distance_step();
    case MENU_BPM: return render_row_bpm();
    case MENU_NOTE_DURATION: return render_row_note_duration();
    case MENU_MIDI: return render_row_midi();
    case MENU_CC: return render_row_control_change();
  }
}

void render_row_root_note() {
  display.print(F("Root note: "));
  display.print(get_note_name(global_root_note));
  display.println(get_note_octave(global_root_note));
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

void render_row_bpm() {
  display.print(F("BPM: "));
  display.println(global_bpm);
}

void render_row_note_duration() {
  display.print(F("Note duration: 1/"));
  display.println(global_note_duration);
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
  render_top_bar();
  render_menu();
  display.display();
}