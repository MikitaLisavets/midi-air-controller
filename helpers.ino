bool inRange(int val, int minimum, int maximum) {
  return ((minimum <= val) && (val < maximum));
}

void set_selected_row(int row) {
  if (row < 0) {
    row = global_max_rows + row;
  }

  global_selected_row = row % global_max_rows;
}

void set_current_scale_index(int index) {
  if (index < 0) {
    index = global_number_of_scales - 1;
  }
  global_current_scale_index = index % global_number_of_scales;
}

void set_number_of_notes(int number) {
  if (number < MIDI_MIN) {
    number = MIDI_MAX - 1;
  }

  global_number_of_notes = number % MIDI_MAX;
  global_max_distance = (global_min_distance + global_distance_step * global_number_of_notes);
}

void set_midi_channel(int channel) {
  if (channel < MIDI_MIN) {
    channel = MIDI_MAX - 1;
  }

  global_midi_channel = channel % MIDI_MAX;
}

void set_distance_step(int step) {
  if (step < 0) {
    step = 0;
  }

  global_distance_step = step;
  global_max_distance = (global_min_distance + global_distance_step * global_number_of_notes);
}

void set_mode(int mode) {
  if (mode < 0) {
    mode = global_max_modes - 1;
  }

  global_mode = mode % global_max_modes;
}

void set_interval(int interval) {
  if (interval < 0) {
    interval = 0;
  }

  global_interval = interval;
  thread_midi_left.setInterval(global_interval);
  thread_midi_right.setInterval(global_interval);
}

void set_root_note(int note) {
  if (note < MIDI_MIN) {
    note = MIDI_MAX - 1;
  }

  global_root_note = note % MIDI_MAX;
}

void set_control_change(int cc) {
  if (cc < MIDI_MIN) {
    cc = MIDI_MAX - 1;
  }

  global_control_change = cc % MIDI_MAX;
}