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
  int8_t scale_index = global_current_scale_index[side];

  for (uint8_t i = 0; i < note_index; i++) {
    int8_t scale_step_index = i % pgm_read_byte(&SCALES[scale_index].size);
    int8_t scale_step = pgm_read_byte(&SCALES[scale_index].steps[scale_step_index]);
    offset = offset + scale_step;
  }

  return offset;
} 

uint8_t get_note(uint8_t side) {
  int8_t note = global_previous_note[side];

  for (uint8_t i = 0; i < global_number_of_notes[side]; i++) {
    int range_start = global_min_distance[side] + i * global_distance_step[side];
    int range_end = global_min_distance[side] + (i + 1) * global_distance_step[side];

    if (inRange(global_current_distance[side], range_start, range_end)) {
      if (i == 0) {
        note = global_root_note[side];
      } else {
        uint8_t offset = getNoteOffset(side, i);
        note = global_root_note[side] + offset;
      }
      global_note_index[side] = i;
      break;
    } else {
      note = -1;
    }
  }
  return note;
}

void midi_note(uint8_t side) {
  global_current_note[side] = get_note(side);

  if (global_previous_note[side] == global_current_note[side]) {
    return;
  }

  if (global_current_note[side] == -1) {
    note_off(global_midi_channel[side], global_previous_note[side], global_velocity[side]);

    global_previous_note[side] = global_current_note[side];

    MidiUSB.flush();
  } else {
    note_off(global_midi_channel[side], global_previous_note[side], global_velocity[side]);

    global_previous_note[side] = global_current_note[side];

    note_on(global_midi_channel[side], global_current_note[side], global_velocity[side]);
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
  global_velocity[side] = DEFAULT_VELOCITY;
  global_previous_velocity[side] = DEFAULT_VELOCITY;

  global_current_control_value[side] = get_midi_value(side, is_inverted);

  if ((global_current_control_value[side] == 0 && is_inverted) || (global_current_control_value[side] == 127 && !is_inverted)) {
    global_current_control_value[side] = global_previous_control_value[side];
  }

  if (global_previous_control_value[side] != global_current_control_value[side]) {
    global_previous_control_value[side] = global_current_control_value[side];
    control_change(global_midi_channel[side], global_control_change[side], global_current_control_value[side]);
    MidiUSB.flush();  
  }
}

void midi_velocity(uint8_t side, bool is_inverted) {
  global_velocity[side] = get_midi_value(side, is_inverted);

  if ((global_velocity[side] == 0 && is_inverted) || (global_velocity[side] == 127 && !is_inverted)) {
    global_velocity[side] = global_previous_velocity[side];
  }

  global_previous_velocity[side] = global_velocity[side];
}

void loop_midi_left() {
  if (millis() - midi_timer[LEFT_SIDE] <= (60000 / global_bpm[LEFT_SIDE]) / global_note_duration[LEFT_SIDE]) {
    return;
  } else {
    midi_timer[LEFT_SIDE] = millis();
  }
  global_current_distance[LEFT_SIDE] = global_dynamic_distance[LEFT_SIDE];

  switch(global_mode[LEFT_SIDE]) {
    case MODE_NOTE:
      midi_note(LEFT_SIDE);
      break;
    case MODE_CC:
      midi_cc(LEFT_SIDE, false);
      break;
    case MODE_CC_INVERTED:
      midi_cc(LEFT_SIDE, true);
      break;
    case MODE_VELOCITY:
      midi_velocity(LEFT_SIDE, false);
      break;
    case MODE_VELOCITY_INVERTED:
      midi_velocity(LEFT_SIDE, true);
      break;
  }
}

void loop_midi_right() {
  if (millis() - midi_timer[RIGHT_SIDE] <= (60000 / global_bpm[RIGHT_SIDE]) / global_note_duration[RIGHT_SIDE]) {
    return;
  } else {
    midi_timer[RIGHT_SIDE] = millis();
  }
  global_current_distance[RIGHT_SIDE] = global_dynamic_distance[RIGHT_SIDE];

  switch(global_mode[RIGHT_SIDE]) {
    case MODE_NOTE:
      midi_note(RIGHT_SIDE);
      break;
    case MODE_CC:
      midi_cc(RIGHT_SIDE, false);
      break;
    case MODE_CC_INVERTED:
      midi_cc(RIGHT_SIDE, true);
      break;
    case MODE_VELOCITY:
      midi_velocity(RIGHT_SIDE, false);
      break;
    case MODE_VELOCITY_INVERTED:
      midi_velocity(RIGHT_SIDE, true);
      break;
  }
}