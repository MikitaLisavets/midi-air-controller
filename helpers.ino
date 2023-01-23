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
  if (number < 0) {
    number = global_max_number_of_notes - 1;
  }

  global_number_of_notes = number % global_max_number_of_notes;
  global_max_distance = (global_min_distance + global_distance_step * global_number_of_notes);
}

void set_midi_channel(int channel) {
  if (channel < 0) {
    channel = global_max_midi_channel - 1;
  }

  global_midi_channel = channel % global_max_midi_channel;
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

void set_root_note(int note) {
  if (note < 0) {
    note = global_max_note;
  }

  global_root_note = note % global_max_note;
}

char* get_mode_name(int mode) {
  switch(mode) {
    case MODE_NOTE: return "Note";
    case MODE_VALUE: return "Value";
    case MODE_VALUE_INVERTED: return "Value (inv)";
  }
}

String get_note_name(int note) {
  if (note == -1) {
    return "-";
  } else {
    return note_names[note % 12] + String(round(note / 12));
  }
}

char* get_scale_name(int index) {
  return scales_names[index];
}

bool inRange(int val, int minimum, int maximum) {
  return ((minimum <= val) && (val < maximum));
}