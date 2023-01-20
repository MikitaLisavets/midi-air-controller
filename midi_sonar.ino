#include "MIDIUSB.h"
#include <RunningMedian.h>
#include <hcsr04.h>


#define TRIG_PIN 9
#define ECHO_PIN 6

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

int accuracy = 10;
RunningMedian distancies = RunningMedian(accuracy);

int previous_note = -1;
int velocity = 64;

int step = 30;
int scale[] = { 2, 1, 2, 2, 1, 3, 1 };
int root_note = 48;
int number_of_notes = 24;
int start_distance = 10;

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, start_distance, 4000);


void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

int getNoteOffset(int i) {
  int offset = 0;
  for (int j = 0; j < i; j++) {
    int index = j % ARRAY_SIZE(scale);
    offset = offset + scale[index];
  }
  return offset;
} 

bool inRange(int val, int minimum, int maximum) {
  return ((minimum <= val) && (val < maximum));
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int i = 0;
  while (i < accuracy) {
    distancies.add(hcsr04.distanceInMillimeters());
    delay(4);
    i++;
  }

  float distance = distancies.getMedian();

  Serial.println("Distance: " + String(distance));
  
  int note = previous_note;

  for (int i = 0; i < number_of_notes; i++) {
    int range_start = start_distance + i * step;
    int range_end = start_distance + (i + 1) * step;

    if (inRange(distance, range_start, range_end)) {
      Serial.println("Distance number: " + String(i));
      if (i == 0) {
        note = root_note;
      } else {
        int offset = getNoteOffset(i);
        Serial.println("Note offset: " + String(offset));
        note = root_note + offset;
      }
      Serial.println("Note: " + String(note));
      break;
    } else {
      note = -1;
    }
  }

  Serial.println("Note: " + String(note));

  if (note == -1) {
    noteOff(0, previous_note, velocity);
    MidiUSB.flush();
  } else if (previous_note != note) {
    noteOff(0, previous_note, velocity);
    previous_note = note;
    noteOn(0, note, velocity);
    MidiUSB.flush();
  }
}
