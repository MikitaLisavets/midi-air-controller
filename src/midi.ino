#include <midi.h>

void note_on(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  midiEventPacket_t noteOn = {0x09, (uint8_t)(0x90 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void note_off(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  midiEventPacket_t noteOff = {0x08, (uint8_t)(0x80 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void control_change(uint8_t channel, uint8_t control, uint8_t value) {
  midiEventPacket_t event = {0x0B, (uint8_t)(0xB0 | channel), control, value};
  MidiUSB.sendMIDI(event);
}

uint8_t getNoteOffset(uint8_t side, uint8_t note_index) {
  uint8_t offset = 0;
  int8_t scale_index = settings.scale_index[side];

  for (uint8_t i = 0; i < note_index; i++) {
    int8_t scale_step_index = i % pgm_read_byte(&SCALES[scale_index].size);
    int8_t scale_step = pgm_read_byte(&SCALES[scale_index].steps[scale_step_index]);
    offset = offset + scale_step;
  }

  return offset;
} 

uint8_t get_note(uint8_t side, bool is_inverted) {
  int8_t note = global_previous_note[side];
  uint8_t offset;

  for (uint8_t i = 0; i < settings.number_of_notes[side]; i++) {
    int range_start = global_min_distance[side] + i * settings.distance_step[side];
    int range_end = global_min_distance[side] + (i + 1) * settings.distance_step[side];

    if (inRange(global_current_distance[side], range_start, range_end)) {
      offset = getNoteOffset(side, is_inverted ? (settings.number_of_notes[side] - 1) - i: i);
      note = settings.root_note[side] + offset;
      global_note_index[side] = i;
      break;
    } else {
      note = -1;
    }
  }
  return note;
}

void midi_note(uint8_t side, bool is_inverted) {
  global_current_note[side] = get_note(side, is_inverted);

  if (global_previous_note[side] == global_current_note[side]) {
    return;
  }

  if (global_current_note[side] == -1) {
    note_off(settings.midi_channel[side], global_previous_note[side], global_velocity);

    global_previous_note[side] = global_current_note[side];

    MidiUSB.flush();
  } else {
    note_off(settings.midi_channel[side], global_previous_note[side], global_velocity);

    global_previous_note[side] = global_current_note[side];

    note_on(settings.midi_channel[side], global_current_note[side], global_velocity);
    MidiUSB.flush();
  }
}

uint8_t get_midi_value(uint8_t side, bool is_inverted) {
  int value;
  if (global_current_distance[side] < global_min_distance[side]) {
    value = global_min_distance[side];
  } else if (global_current_distance[side] > global_max_distance[side]) {
    value = global_max_distance[side];
  } else {
    value = global_current_distance[side];
  }

  return map(value, global_min_distance[side], global_max_distance[side], is_inverted ? 127 : 0,  is_inverted ? 0 : 127);
}

void midi_cc(uint8_t side, bool is_inverted) {
  global_velocity = DEFAULT_VELOCITY;
  global_previous_velocity = DEFAULT_VELOCITY;

  global_current_control_value[side] = get_midi_value(side, is_inverted);

  if ((global_current_control_value[side] == 0 && is_inverted) || (global_current_control_value[side] == 127 && !is_inverted)) {
    global_current_control_value[side] = global_previous_control_value[side];
  }

  if (global_previous_control_value[side] != global_current_control_value[side]) {
    global_previous_control_value[side] = global_current_control_value[side];
    control_change(settings.midi_channel[side], settings.control_change[side], global_current_control_value[side]);
    MidiUSB.flush();  
  }
}

void midi_velocity(uint8_t side, bool is_inverted) {
  global_velocity = get_midi_value(side, is_inverted);

  if ((global_velocity == 0 && is_inverted) || (global_velocity == 127 && !is_inverted)) {
    global_velocity = global_previous_velocity;
  }

  global_previous_velocity = global_velocity;
}

void process_midi(uint8_t side) {
  if ( (millis() - midi_timer[side]) < settings.midi_interval[side] ) {
    return;
  }

  midi_timer[side] = millis();
  global_current_distance[side] = readSensor(side);

  switch(settings.mode[side]) {
    case MODE_NOTE:
    case MODE_NOTE_INVERTED:
      midi_note(side, settings.mode[side] == MODE_NOTE_INVERTED);
      break;
    case MODE_CC:
    case MODE_CC_INVERTED:
      midi_cc(side, settings.mode[side] == MODE_CC_INVERTED);
      break;
    case MODE_VELOCITY:
    case MODE_VELOCITY_INVERTED:
      midi_velocity(side, settings.mode[side] == MODE_VELOCITY_INVERTED);
      break;
  }
}

void loop_midi_left() {
  process_midi(LEFT_SIDE);
}

void loop_midi_right() {
  process_midi(RIGHT_SIDE);
}