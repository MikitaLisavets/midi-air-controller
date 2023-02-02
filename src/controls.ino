#include <controls.h>

void setup_controls() {
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
}

void handle_press_right() {
  control_status = RIGHT;

  switch (global_menu_selected_row) {
   case MENU_ROOT_NOTE:
        set_root_note(global_root_note + 1);
        break;
    case MENU_SCALE:
        set_current_scale_index(global_current_scale_index + 1);
        break;
    case MENU_MODE:
        set_mode(global_mode + 1);
        break;
    case MENU_NOTES:
        set_number_of_notes(global_number_of_notes + 1);
        break;
    case MENU_DISTANCE_STEP:
        set_distance_step(global_distance_step + 5);
        break;
    case MENU_BPM:
        set_bpm(global_bpm + 1);
        break;
    case MENU_NOTE_DURATION:
        set_note_duration(global_note_duration + 1);
        break;
    case MENU_MIDI:
        set_midi_channel(global_midi_channel + 1);
        break;
    case MENU_CC:
        set_control_change(global_control_change + 1);
        break;
  }
}

void handle_press_left() {
  control_status = LEFT;

  switch (global_menu_selected_row) {
    case MENU_ROOT_NOTE:
        set_root_note(global_root_note - 1);
        break;
    case MENU_SCALE:
        set_current_scale_index(global_current_scale_index - 1);
        break;
    case MENU_MODE:
        set_mode(global_mode - 1);
        break;
    case MENU_NOTES:
        set_number_of_notes(global_number_of_notes - 1);
        break;
    case MENU_DISTANCE_STEP:
        set_distance_step(global_distance_step - 5);
        break;
    case MENU_BPM:
        set_bpm(global_bpm - 1);
        break;
    case MENU_NOTE_DURATION:
        set_note_duration(global_note_duration - 1);
        break;
    case MENU_MIDI:
        set_midi_channel(global_midi_channel - 1);
        break;
    case MENU_CC:
        set_control_change(global_control_change - 1);
        break;
  }

}

void handle_press_up() {
  control_status = UP;
  set_selected_row(global_menu_selected_row - 1);
}

void handle_press_down() {
  control_status = DOWN;
  set_selected_row(global_menu_selected_row + 1);
}

void loop_controls() {
  if (millis() < control_timer + TIMER_TIMEOUT && control_status != NONE) {
    return;
  }

  control_timer = millis();
  control_status = NONE;

  if (digitalRead(BUTTON_UP) == LOW && digitalRead(BUTTON_DOWN) == LOW) {
    global_is_display_enabled = !global_is_display_enabled;
    clear_display();
  }

  if (digitalRead(BUTTON_LEFT) == LOW) {
    handle_press_left();
  } else if (digitalRead(BUTTON_RIGHT) == LOW) {
    handle_press_right();
  } else if (digitalRead(BUTTON_UP) == LOW) {
    handle_press_up();
  } else if (digitalRead(BUTTON_DOWN) == LOW) {
    handle_press_down();
  }
}