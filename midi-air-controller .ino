void setup() {
  Serial.begin(9600);

  setup_display();
  setup_controls();
}

void loop() {
  // xValue = analogRead(joyX);
  // yValue = analogRead(joyY);
  // buttonState = digitalRead(joySW);

  int distance = getDistance();
  // Serial.println("Distance: " + String(distance));

  int note = get_note(distance);
  // Serial.println("Note: " + String(note));

  String note_name = get_note_name(note);

  play_note(note);

  clear_display();
  set_display_status_bar("Dist: " + String(distance), "Note: " + String(note_name));

  int root_note = get_root_note();
  String root_note_name = get_note_name(root_note);

  int current_scale_index = get_current_scale_index();
  String scale_name = get_scale_name(current_scale_index);

  set_display_main(root_note_name, scale_name);

  render_display();

  handle_controls();
}
