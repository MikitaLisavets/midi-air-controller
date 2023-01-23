#include "MIDIUSB.h"

void pitch_bend(byte channel, byte value) {
  byte lowValue = value & 0x7F;
  byte highValue = value >> 7;
  midiEventPacket_t pitchBend = {0x0E, 0xE0 | channel, lowValue, highValue};
  MidiUSB.sendMIDI(pitchBend);
}

void note_on(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void note_off(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void control_change(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

bool inRange(int val, int minimum, int maximum) {
  return ((minimum <= val) && (val < maximum));
}

byte getNoteOffset(byte note_index) {
  byte offset = 0;

  for (byte i = 0; i < note_index; i++) {
    byte scale_step_index = i % pgm_read_byte(&scales_sizes[global_current_scale_index]);
    byte scale_step = scales_steps[global_current_scale_index][scale_step_index];
    offset = offset + scale_step;
  }

  return offset;
} 

byte get_note(int distance) {
  byte note = global_previous_note;

  for (byte i = 0; i < global_number_of_notes; i++) {
    int range_start = global_min_distance + i * global_distance_step;
    int range_end = global_min_distance + (i + 1) * global_distance_step;

    if (inRange(distance, range_start, range_end)) {
      if (i == 0) {
        note = global_root_note;
      } else {
        byte offset = getNoteOffset(i);
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

void play_midi(byte note) {
  if (global_mode == 0) {
    if (note == 0) {
      note_off(global_midi_channel, global_previous_note, global_velocity);

      global_previous_note = note;

      MidiUSB.flush();
    } else if (global_previous_note != note) {
      note_off(global_midi_channel, global_previous_note, global_velocity);

      global_previous_note = note;

      note_on(global_midi_channel, note, global_velocity);
      MidiUSB.flush();
    }
  } else if (global_mode == 1) {
    byte value = map(global_current_distance, global_min_distance, (global_min_distance + global_distance_step * global_number_of_notes), 0, 127);
    control_change(global_midi_channel, global_control_channel, value);    
  }
  // if (global_is_pitch) {
    // TO-DO
    // int pitchBendVal = map(global_current_distance, (global_note_index * global_distance_step) - global_distance_step, (global_note_index * global_distance_step) + global_distance_step, 0, 16383);
    // Serial.println("global_current_distance " + String(global_current_distance));
    // Serial.println("from " + String( (global_note_index * global_distance_step) - global_distance_step));
    // Serial.println("to " + String( (global_note_index * global_distance_step) + global_distance_step));

    // Serial.println("pitchBendVal " + String(pitchBendVal));
    // pitch_bend(global_midi_channel, pitchBendVal);
    // MidiUSB.flush();
  // }
}

String get_note_name(int note) {
  if (note == 0) {
    return "-";
  } else {
    return note_names[note % 12] + String(round(note / 12));
  }
}

char* get_scale_name(int index) {
  return scales_names[index];
}

void set_root_note(int note) {
  if (note < 0) {
    note = global_max_note;
  }

  global_root_note = note % global_max_note;
}

void loop_midi() {
  global_current_note = get_note(global_current_distance);
  play_midi(global_current_note);
}