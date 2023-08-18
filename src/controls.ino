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
   case MENU_SIDE:
        set_side(settings.side + 1);
        break;
   case MENU_ROOT_NOTE:
        set_root_note(settings.root_note[settings.side] + 1);
        break;
    case MENU_SCALE:
        set_current_scale_index(settings.scale_index[settings.side] + 1);
        break;
    case MENU_MODE:
        set_mode(settings.mode[settings.side] + 1);
        break;
    case MENU_NOTES:
        set_number_of_notes(settings.number_of_notes[settings.side] + 1);
        break;
    case MENU_DISTANCE_STEP:
        set_distance_step(settings.distance_step[settings.side] + 5);
        break;
    case MENU_NOTE_LENGTH:
        set_note_length(settings.note_length[settings.side] + 1);
        break;
    case MENU_MIDI:
        set_midi_channel(settings.midi_channel[settings.side] + 1);
        break;
    case MENU_CC:
        set_control_change(settings.control_change[settings.side] + 1);
        break;
    case MENU_SAVE:
        save_settings();
        render_save_screen();
        break;
    case MENU_LOAD:
        load_settings();
        render_load_screen();
        break;
    case MENU_RESET:
        reset_settings();
        render_reset_screen();
        break;
    case MENU_AUTO_LOAD_SETTINGS:
        toggle_auto_load_settings();
        break;
  }
}

void handle_press_left() {
  control_status = LEFT;

  switch (global_menu_selected_row) {
   case MENU_SIDE:
        set_side(settings.side - 1);
        break;
    case MENU_ROOT_NOTE:
        set_root_note(settings.root_note[settings.side] - 1);
        break;
    case MENU_SCALE:
        set_current_scale_index(settings.scale_index[settings.side] - 1);
        break;
    case MENU_MODE:
        set_mode(settings.mode[settings.side] - 1);
        break;
    case MENU_NOTES:
        set_number_of_notes(settings.number_of_notes[settings.side] - 1);
        break;
    case MENU_DISTANCE_STEP:
        set_distance_step(settings.distance_step[settings.side] - 5);
        break;
    case MENU_NOTE_LENGTH:
        set_note_length(settings.note_length[settings.side] - 1);
        break;
    case MENU_MIDI:
        set_midi_channel(settings.midi_channel[settings.side] - 1);
        break;
    case MENU_CC:
        set_control_change(settings.control_change[settings.side] - 1);
        break;
    case MENU_SAVE:
        save_settings();
        render_save_screen();
        break;
    case MENU_LOAD:
        load_settings();
        render_load_screen();
        break;
    case MENU_RESET:
        reset_settings();
        render_reset_screen();
        break;
    case MENU_AUTO_LOAD_SETTINGS:
        toggle_auto_load_settings();
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
    control_status = UP_DOWN;
    global_is_display_enabled = !global_is_display_enabled;
    clear_display();
    return;
  }

  if (digitalRead(BUTTON_LEFT) == LOW && digitalRead(BUTTON_RIGHT) == LOW) {
    control_status = LEFT_RIGHT;
    global_is_led_enabled = !global_is_led_enabled;
    toggle_leds();
    return;
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