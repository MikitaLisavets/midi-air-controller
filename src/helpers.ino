#include <helpers.h>

bool inRange(int16_t val, int16_t minimum, int16_t maximum) {
  return ((minimum <= val) && (val < maximum));
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

void set_side(int side) {
  if (side < 0) {
    side = 1;
  }
  settings.side = side % 2;
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
  settings.scale_index[settings.side] = index % NUMBER_OF_SCALES;
}

void set_number_of_notes(int8_t number) {
  if (number < MIDI_MIN) {
    number = MIDI_MAX - 1;
  }

  settings.number_of_notes[settings.side] = number % MIDI_MAX;
  global_max_distance[settings.side] = (global_min_distance[settings.side] + settings.distance_step[settings.side] * settings.number_of_notes[settings.side]);
}

void set_midi_channel(int8_t channel) {
  if (channel < MIDI_MIN) {
    channel = MIDI_MAX - 1;
  }

  settings.midi_channel[settings.side] = channel % MIDI_MAX;
}

void set_distance_step(int8_t step) {
  if (step < 0) {
    step = 0;
  }

  settings.distance_step[settings.side] = step;
  global_max_distance[settings.side] = (global_min_distance[settings.side] + settings.distance_step[settings.side] * settings.number_of_notes[settings.side]);
}

void set_mode(int8_t mode) {
  if (mode < 0) {
    mode = MAX_MODES - 1;
  }

  settings.mode[settings.side] = mode % MAX_MODES;
}

void set_midi_interval(uint16_t midi_interval){
  if (midi_interval <= 0) {
    midi_interval = 0;
  }

  settings.midi_interval[settings.side] = midi_interval;
}

void set_root_note(int8_t note) {
  if (note < MIDI_MIN) {
    note = MIDI_MAX - 1;
  }

  settings.root_note[settings.side] = note % MIDI_MAX;
}

void set_control_change(int8_t cc) {
  if (cc < MIDI_MIN) {
    cc = MIDI_MAX - 1;
  }

  settings.control_change[settings.side] = cc % MIDI_MAX;
}

void toggle_auto_load_settings() {
  settings.autoLoadSettings = !settings.autoLoadSettings;
}

void load_settings() {
  EEPROM.get(STR_ADDR, settings);
}

void save_settings() {
  EEPROM.put(STR_ADDR, settings);
}

void reset_settings() {
  settings = {
    .side = LEFT_SIDE,
    .mode = {0, 4},
    .root_note = {36, 24},
    .number_of_notes = {15, 8},
    .scale_index = {0, 0},
    .midi_interval = {140, 140},
    .midi_channel = {0, 0},
    .control_change = {0, 1},
    .distance_step = {15, 30},
    .autoLoadSettings = false,
  };
}
