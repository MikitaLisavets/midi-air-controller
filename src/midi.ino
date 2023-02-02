#include <midi.h>

uint32_t timer;

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

uint8_t getNoteOffset(uint8_t note_index) {
  int offset = 0;

  for (uint8_t i = 0; i < note_index; i++) {
    int8_t scale_step_index = i % pgm_read_byte(&SCALES[global_current_scale_index].size);
    int8_t scale_step = pgm_read_byte(&SCALES[global_current_scale_index].steps[scale_step_index]);
    offset = offset + scale_step;
  }

  return offset;
} 

uint8_t get_note(int16_t distance) {
  int8_t note = global_previous_note;

  for (uint8_t i = 0; i < global_number_of_notes; i++) {
    int range_start = global_min_distance + i * global_distance_step;
    int range_end = global_min_distance + (i + 1) * global_distance_step;

    if (inRange(distance, range_start, range_end)) {
      if (i == 0) {
        note = global_root_note;
      } else {
        uint8_t offset = getNoteOffset(i);
        note = global_root_note + offset;
      }
      global_note_index = i;
      break;
    } else {
      note = -1;
    }
  }
  return note;
}

void midi_note(int16_t current_distance) {
  global_current_note = get_note(current_distance);

  if (global_previous_note == global_current_note) {
    return;
  }

  if (global_current_note == -1) {
    note_off(global_midi_channel, global_previous_note, global_velocity);

    global_previous_note = global_current_note;

    MidiUSB.flush();
  } else {
    note_off(global_midi_channel, global_previous_note, global_velocity);

    global_previous_note = global_current_note;

    note_on(global_midi_channel, global_current_note, global_velocity);
    MidiUSB.flush();
  }
}

uint8_t get_midi_value(int16_t current_distance, bool is_inverted) {
  int value;
  if (current_distance < global_min_distance) {
    value = global_min_distance;
  } else if (current_distance > global_max_distance) {
    value = global_max_distance;
  } else {
    value = current_distance;
  }

  return map(value, global_min_distance, global_max_distance, is_inverted ? 127 : 0,  is_inverted ? 0 : 127);
}

void midi_cc(int16_t current_distance, bool is_inverted) {
  global_velocity = DEFAULT_VELOCITY;
  global_previous_velocity = DEFAULT_VELOCITY;

  global_current_control_value = get_midi_value(current_distance, is_inverted);

  if ((global_current_control_value == 0 && is_inverted) || (global_current_control_value == 127 && !is_inverted)) {
    global_current_control_value = global_previous_control_value;
  }

  if (global_previous_control_value != global_current_control_value) {
    global_previous_control_value = global_current_control_value;
    control_change(global_midi_channel, global_control_change, global_current_control_value);
    MidiUSB.flush();  
  }
}

void midi_velocity(int16_t current_distance, bool is_inverted) {
  global_velocity = get_midi_value(current_distance, is_inverted);

  if ((global_velocity == 0 && is_inverted) || (global_velocity == 127 && !is_inverted)) {
    global_velocity = global_previous_velocity;
  }

  global_previous_velocity = global_velocity;
}

void loop_midi_left() {
  if (millis() - midi_left_timer <= (60000 / global_bpm) / global_note_duration) {
    return;
  } else {
    midi_left_timer = millis();
  }
  global_current_distance_left = global_dynamic_distance_left;

  switch(global_mode) {
    case MODE_L_NOTE_R_CC:
      midi_note(global_current_distance_left);
      break;
    case MODE_L_NOTE_R_CC_INVERTED:
      midi_note(global_current_distance_left);
      break;
    case MODE_L_NOTE_R_VELOCITY:
      midi_note(global_current_distance_left);
      break;
    case MODE_L_NOTE_R_VELOCITY_INVERTED:
      midi_note(global_current_distance_left);
      break;
    case MODE_L_CC_R_NOTE:
      midi_cc(global_current_distance_left, false);
      break;
    case MODE_L_CC_INVERTED_R_NOTE:
      midi_cc(global_current_distance_left, true);
      break;
    case MODE_L_VELOCITY_R_NOTE:
      midi_velocity(global_current_distance_left, false);  
      break;
    case MODE_L_VELOCITY_INVERTED_R_NOTE:
      midi_velocity(global_current_distance_left, true);
      break;
  }
}

void loop_midi_right() {
  if (millis() - midi_right_timer <= (60000 / global_bpm) / global_note_duration) {
    return;
  } else {
    midi_right_timer = millis();
  }

  global_current_distance_right = global_dynamic_distance_right;

  switch(global_mode) {
    case MODE_L_NOTE_R_CC:
      midi_cc(global_current_distance_right, false);
      break;
    case MODE_L_NOTE_R_CC_INVERTED:
      midi_cc(global_current_distance_right, true);
      break;
    case MODE_L_NOTE_R_VELOCITY:
      midi_velocity(global_current_distance_right, false);
      break;
    case MODE_L_NOTE_R_VELOCITY_INVERTED:
      midi_velocity(global_current_distance_right, true);
      break;
    case MODE_L_CC_R_NOTE:
      midi_note(global_current_distance_right);
      break;
    case MODE_L_CC_INVERTED_R_NOTE:
      midi_note(global_current_distance_right);
      break;
    case MODE_L_VELOCITY_R_NOTE:
      midi_note(global_current_distance_right);
      break;
    case MODE_L_VELOCITY_INVERTED_R_NOTE:
      midi_note(global_current_distance_right);
      break;
  }
}