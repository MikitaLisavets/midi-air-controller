#define JOY_X A0
#define JOY_Y A1
#define JOY_SW 7

#define PRESS "PRESS"
#define UP "UP"
#define DOWN "DOWN"
#define LEFT "LEFT"
#define RIGHT "RIGHT"

int center_value = 500;
int threshold = 250;

int xValue;
int yValue;
bool buttonState;

String control_status = "NONE";

void setup_controls() {
  pinMode(JOY_SW, INPUT_PULLUP);
}

void handle_button_press() {
  control_status = PRESS;
  Serial.println("Event: PRESS");
}

void handle_press_right() {
  control_status = RIGHT;
  Serial.println("Event: RIGHT");

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
        set_is_pitch(!global_is_pitch);
        break;
  }
}

void handle_press_left() {
  control_status = LEFT;
  Serial.println("Event: LEFT");

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
        set_is_pitch(!global_is_pitch);
        break;
  }

}

void handle_press_up() {
  control_status = UP;
  Serial.println("Event: UP");
  set_selected_row(global_selected_row - 1);
}

void handle_press_down() {
  control_status = DOWN;
  Serial.println("Event: DOWN");
  set_selected_row(global_selected_row + 1);
}

void loop_controls() {
  xValue = analogRead(JOY_X);
  yValue = analogRead(JOY_Y);
  buttonState = digitalRead(JOY_SW);

  if (buttonState == LOW) {
    handle_button_press();
  }
  if (xValue >= center_value + threshold) {
    handle_press_left();
  }
  if (xValue <= center_value - threshold) {
    handle_press_right();
  }
  if (yValue >= center_value + threshold) {
    handle_press_up();
  }
  if (yValue <= center_value - threshold) {
    handle_press_down();
  }

  control_status = "NONE";
}