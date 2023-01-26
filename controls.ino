#define BUTTON_UP 6
#define BUTTON_DOWN 9
#define BUTTON_LEFT 7
#define BUTTON_RIGHT 8

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define PRESS 5
#define NONE 0

#define TIMER_TIMEOUT 200

#define CENTER_VALUE 500
#define THRESHOLD 250

byte control_status = NONE;

unsigned long timer;

void setup_controls() {
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
}

void handle_press_right() {
  control_status = RIGHT;

  switch (global_selected_row) {
   case global_menu_root_note:
        set_root_note(global_root_note + 1);
        break;
    case global_menu_scale:
        set_current_scale_index(global_current_scale_index + 1);
        break;
    case global_menu_mode:
        set_mode(global_mode + 1);
        break;
    case global_menu_notes:
        set_number_of_notes(global_number_of_notes + 1);
        break;
    case global_menu_distance_step:
        set_distance_step(global_distance_step + 5);
        break;
    case global_menu_tempo:
        set_tempo(global_tempo + 1);
        break;
    case global_menu_midi:
        set_midi_channel(global_midi_channel + 1);
        break;
    case global_menu_control_change:
        set_control_change(global_control_change + 1);
        break;
  }
}

void handle_press_left() {
  control_status = LEFT;

  switch (global_selected_row) {
    case global_menu_root_note:
        set_root_note(global_root_note - 1);
        break;
    case global_menu_scale:
        set_current_scale_index(global_current_scale_index - 1);
        break;
    case global_menu_mode:
        set_mode(global_mode - 1);
        break;
    case global_menu_notes:
        set_number_of_notes(global_number_of_notes - 1);
        break;
    case global_menu_distance_step:
        set_distance_step(global_distance_step - 5);
        break;
    case global_menu_tempo:
        set_tempo(global_tempo - 1);
        break;
    case global_menu_midi:
        set_midi_channel(global_midi_channel - 1);
        break;
    case global_menu_control_change:
        set_control_change(global_control_change - 1);
        break;
  }

}

void handle_press_up() {
  control_status = UP;
  set_selected_row(global_selected_row - 1);
}

void handle_press_down() {
  control_status = DOWN;
  set_selected_row(global_selected_row + 1);
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