#define JOY_X A0
#define JOY_Y A1
#define JOY_SW 7

#define CENTER_VALUE 500
#define THRESHOLD 250

#define PRESS "PRESS"
#define UP "UP"
#define DOWN "DOWN"
#define LEFT "LEFT"
#define RIGHT "RIGHT"

int xValue;
int yValue;

int middle_value = 500;

bool buttonState;

String control_status = "";

void setup_controls() {
  pinMode(JOY_SW, INPUT_PULLUP);
}

void handle_button_press() {
  // control_status = PRESS;
  Serial.println("handle_button_press");
}

void handle_press_right() {
  // control_status = RIGHT;
  Serial.println("handle_press_right");
  set_current_scale_index(get_current_scale_index() + 1);
}

void handle_press_left() {
  // control_status = LEFT;
  Serial.println("handle_press_left");
  set_current_scale_index(get_current_scale_index() - 1);
}

void handle_press_up() {
  // control_status = UP;
  Serial.println("handle_press_up");
  set_root_note(get_root_note() + 1);
}

void handle_press_down() {
  // control_status = DOWN;
  Serial.println("handle_press_down");
  set_root_note(get_root_note() - 1);
}

void handle_controls() {
  xValue = analogRead(JOY_X);
  yValue = analogRead(JOY_Y);
  buttonState = digitalRead(JOY_SW);
  // Serial.println("xValue " + String(xValue));
  // Serial.println("yValue " + String(yValue));

  if (buttonState == LOW && control_status != PRESS) {
    handle_button_press();
  }
  if (xValue >= CENTER_VALUE + THRESHOLD && control_status != LEFT) {
    handle_press_left();
  }
  if (xValue <= CENTER_VALUE - THRESHOLD && control_status != RIGHT) {
    handle_press_right();
  }
  if (yValue >= CENTER_VALUE + THRESHOLD && control_status != UP) {
    handle_press_up();
  }
  if (yValue <= CENTER_VALUE - THRESHOLD && control_status != DOWN) {
    handle_press_down();
  }
}