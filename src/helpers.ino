#include <helpers.h>

bool inRange(int16_t val, int16_t minimum, int16_t maximum) {
  return ((minimum <= val) && (val < maximum));
}

void set_selected_row(int8_t row) {
  if (row < 0) {
    row = MAX_MENU_ROWS + row;
  }

  global_menu_selected_row = row % MAX_MENU_ROWS;
}

void set_current_scale_index(int8_t index) {
  if (index < 0) {
    index = NUMBER_OF_SCALES - 1;
  }
  global_current_scale_index = index % NUMBER_OF_SCALES;
}

void set_number_of_notes(int8_t number) {
  if (number < MIDI_MIN) {
    number = MIDI_MAX - 1;
  }

  global_number_of_notes = number % MIDI_MAX;
  global_max_distance = (global_min_distance + global_distance_step * global_number_of_notes);
}

void set_midi_channel(int8_t channel) {
  if (channel < MIDI_MIN) {
    channel = MIDI_MAX - 1;
  }

  global_midi_channel = channel % MIDI_MAX;
}

void set_distance_step(int8_t step) {
  if (step < 0) {
    step = 0;
  }

  global_distance_step = step;
  global_max_distance = (global_min_distance + global_distance_step * global_number_of_notes);
}

void set_mode(int8_t mode) {
  if (mode < 0) {
    mode = MAX_MODES - 1;
  }

  global_mode = mode % MAX_MODES;
}

void set_bpm(int16_t bpm) {
  if (bpm < 0) {
    bpm = 0;
  }
  global_bpm = bpm;
}

void set_note_duration(int8_t note_duration){
  if (note_duration <= 0) {
    note_duration = 1;
  }

  global_note_duration = note_duration;
}

void set_root_note(int8_t note) {
  if (note < MIDI_MIN) {
    note = MIDI_MAX - 1;
  }

  global_root_note = note % MIDI_MAX;
}

void set_control_change(int8_t cc) {
  if (cc < MIDI_MIN) {
    cc = MIDI_MAX - 1;
  }

  global_control_change = cc % MIDI_MAX;
}

void toggle_leds() {
  if (global_is_led_enabled) {
    pinMode(LED_BUILTIN_TX, OUTPUT);
    pinMode(LED_BUILTIN_RX, OUTPUT);
  } else {
    pinMode(LED_BUILTIN_TX, INPUT);
    pinMode(LED_BUILTIN_RX, INPUT);
  }
}