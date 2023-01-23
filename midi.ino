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

int getNoteOffset(byte note_index) {
  int offset = 0;

  for (byte i = 0; i < note_index; i++) {
    byte scale_step_index = i % scales_sizes[global_current_scale_index];
    byte scale_step = scales_steps[global_current_scale_index][scale_step_index];
    offset = offset + scale_step;
  }

  return offset;
} 

int get_note(int distance) {
  int note = global_previous_note;

  for (byte i = 0; i < global_number_of_notes; i++) {
    int range_start = global_min_distance + i * global_distance_step;
    int range_end = global_min_distance + (i + 1) * global_distance_step;

    if (inRange(distance, range_start, range_end)) {
      if (i == 0) {
        note = global_root_note;
      } else {
        int offset = getNoteOffset(i);
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

void play_midi(int note) {
  if (global_mode == MODE_NOTE) {
    if (global_previous_note != note) {
      if (note == -1) {
        note_off(global_midi_channel, global_previous_note, global_velocity);

        global_previous_note = note;

        MidiUSB.flush();
      } else if (global_previous_note != note) {
        note_off(global_midi_channel, global_previous_note, global_velocity);

        global_previous_note = note;

        note_on(global_midi_channel, note, global_velocity);
        MidiUSB.flush();
      }
    }
  } else if (global_mode == MODE_VALUE || global_mode == MODE_VALUE_INVERTED) {
    int value = global_current_distance < global_min_distance ? global_min_distance : global_current_distance;

    global_current_control_value = map(value, global_min_distance, global_max_distance, global_mode == MODE_VALUE ? 0 : 127,  global_mode == MODE_VALUE ? 127 : 0);

    if (global_current_control_value == 0 && global_mode == MODE_VALUE || global_current_control_value == 127 && global_mode == MODE_VALUE_INVERTED) {
      global_current_control_value = global_previous_control_value;
    }

    if (global_previous_control_value != global_current_control_value) {
      global_previous_control_value = global_current_control_value;
      control_change(global_midi_channel, global_control_channel, global_current_control_value);
      MidiUSB.flush();  
    }
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