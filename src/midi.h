#ifndef __MIDI_H
#define __MIDI_H

#include <main.h>
#include "MIDIUSB.h" // Source: https://github.com/arduino-libraries/MIDIUSB

uint32_t midi_left_timer;
uint32_t midi_right_timer;

void setup_midi();
void loop_midi_left();
void loop_midi_right();

#endif