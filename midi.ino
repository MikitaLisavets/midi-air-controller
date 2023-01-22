#include "MIDIUSB.h"

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

int MAJOR_SCALE[] = {2,2,1,2,2,2,1};
int CHROMATIC_SCALE[] = {1,1,1,1,1,1,1,1,1,1,1,1};

String scales_names[] = {"Major", "Chromatic"};
int* scales_steps[] = {MAJOR_SCALE, CHROMATIC_SCALE};
int scales_sizes[] = {7, 12};

int start_distance = 20;
int distance_step = 50;

int previous_note = -1;
int velocity = 64;

int current_scale_index = 0;

int root_note = 48;
int number_of_notes = 8;

String note_names[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

void note_on(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void note_off(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void send_midi() {
  MidiUSB.flush();
}

bool inRange(int val, int minimum, int maximum) {
  return ((minimum <= val) && (val < maximum));
}

int getNoteOffset(int note_index) {
  int offset = 0;
  for (int i = 0; i < note_index; i++) {
    int scale_step_index = i % scales_sizes[current_scale_index];
    int scale_offset = scales_steps[current_scale_index][scale_step_index];
    offset = offset + scale_offset;
  }

  return offset;
} 

int get_note(int distance) {
  int note = previous_note;

  for (int i = 0; i < number_of_notes; i++) {
    int range_start = start_distance + i * distance_step;
    int range_end = start_distance + (i + 1) * distance_step;

    if (inRange(distance, range_start, range_end)) {
      if (i == 0) {
        note = root_note;
      } else {
        int offset = getNoteOffset(i);
        note = root_note + offset;
      }
      break;
    } else {
      note = -1;
    }
  }

  return note;
}

void play_note(int note) {
  if (note == -1) {
    note_off(0, previous_note, velocity);

    previous_note = note;

    send_midi();
  } else if (previous_note != note) {
    note_off(0, previous_note, velocity);

    previous_note = note;

    note_on(0, note, velocity);
    send_midi();
  }
}

String get_note_name(int note) {
  if (note == -1) {
    return "-";
  } else {
    return note_names[note % 12];
  }
}

String get_scale_name(int index) {
  return scales_names[index];
}

int get_number_of_scales() {
  return ARRAY_SIZE(scales_names);
}

int get_root_note() {
  return root_note;
}

void set_root_note(int note) {
  root_note = abs(note) % 127;
}

int get_current_scale_index() {
  return current_scale_index;
}

void set_current_scale_index(int index) {
  current_scale_index = abs(index) % get_number_of_scales();
}