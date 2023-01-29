const uint8_t BUTTON_UP = 6;
const uint8_t BUTTON_DOWN = 9;
const uint8_t BUTTON_LEFT = 7;
const uint8_t BUTTON_RIGHT = 8;

const uint8_t UP = 1;
const uint8_t DOWN = 2;
const uint8_t LEFT = 3;
const uint8_t RIGHT = 4;
const uint8_t PRESS = 5;
const uint8_t NONE = 0;

const uint16_t TIMER_TIMEOUT = 200;

const uint16_t CENTER_VALUE = 500;
const uint16_t THRESHOLD = 250;

uint8_t control_status = NONE;
uint32_t timer;

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
    case MENU_INTERVAL:
        set_interval(global_interval + 5);
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
    case MENU_INTERVAL:
        set_interval(global_interval - 5);
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
  if (millis() < timer + TIMER_TIMEOUT && control_status != NONE) {
    return;
  }

  timer = millis();
  control_status = NONE;

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