#define JOY_X A0
#define JOY_Y A1
#define JOY_SW 7

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define PRESS 5
#define NONE 0

#define TIMER_TIMEOUT 200

#define CENTER_VALUE 500
#define THRESHOLD 250

int xValue;
int yValue;
bool buttonState;

byte control_status = NONE;

unsigned long timer;

void setup_controls() {
  pinMode(JOY_SW, INPUT_PULLUP);
}

void handle_button_press() {
  control_status = PRESS;
}

void handle_press_right() {
  control_status = RIGHT;

  switch (global_selected_row) {
    case 0:
        set_root_note(global_root_note + 1);
        break;
    case 1:
        set_current_scale_index(global_current_scale_index + 1);
        break;
    case 2:
        set_number_of_notes(global_number_of_notes + 1);
        break;
    case 3:
        set_midi_channel(global_midi_channel + 1);
        break;
    case 4:
        set_distance_step(global_distance_step + 1);
        break;
    case 5:
        set_mode(global_mode + 1);
        break;
  }
}

void handle_press_left() {
  control_status = LEFT;

  switch (global_selected_row) {
    case 0:
        set_root_note(global_root_note - 1);
        break;
    case 1:
        set_current_scale_index(global_current_scale_index - 1);
        break;
    case 2:
        set_number_of_notes(global_number_of_notes - 1);
        break;
    case 3:
        set_midi_channel(global_midi_channel - 1);
        break;
    case 4:
        set_distance_step(global_distance_step - 1);
        break;
    case 5:
        set_mode(global_mode - 1);
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
  if (millis() < timer && control_status != NONE) {
    return;
  }

  timer = millis();
  control_status = NONE;

  xValue = analogRead(JOY_X);
  yValue = analogRead(JOY_Y);
  buttonState = digitalRead(JOY_SW);

  if (buttonState == LOW) {
    handle_button_press();
  } else if (xValue >= CENTER_VALUE + THRESHOLD) {
    handle_press_left();
  } else if (xValue <= CENTER_VALUE - THRESHOLD) {
    handle_press_right();
  } else if (yValue >= CENTER_VALUE + THRESHOLD) {
    handle_press_up();
  } else if (yValue <= CENTER_VALUE - THRESHOLD) {
    handle_press_down();
  }
}