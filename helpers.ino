void set_selected_row(byte row) {
  if (row < 0) {
    row = global_max_rows + row;
  }

  global_selected_row = row % global_max_rows;
}

void set_current_scale_index(byte index) {
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
}

void set_midi_channel(byte channel) {
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
}

void set_is_pitch(bool is_pitch) {
  global_is_pitch = is_pitch;
}