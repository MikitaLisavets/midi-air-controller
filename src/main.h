#ifndef __MAIN_H
#define __MAIN_H

#include <arduino.h>
#include <stdio.h>
#include <Thread.h> // Source: https://github.com/ivanseidel/ArduinoThread
#include <StaticThreadController.h> // Source: https://github.com/ivanseidel/ArduinoThread
#include <Wire.h>

#include <display.h>
#include <distance.h>
#include <controls.h>
#include <midi.h>
#include <helpers.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

Thread thread_display;
Thread thread_distance;
Thread thread_midi_left;
Thread thread_midi_right;
Thread thread_controls;

// === Scales ===
struct scale_t {
  const char* name;
  const uint8_t size;
  const uint8_t steps[7];
};

const scale_t SCALES[] PROGMEM = {
  { "Major",         7, {2,2,1,2,2,2,1} },
  { "Natural Min",   7, {2,1,2,2,1,2,2} },
  { "Harmonic Maj",  7, {2,2,1,2,1,3,1} },
  { "Harmonic Min",  7, {2,1,2,2,1,2,2} },
  { "Melodic Min",   7, {2,1,2,2,2,2,1} },
  { "Dorian",        7, {2,1,2,2,2,1,2} },
  { "Phrygian",      7, {1,2,2,2,1,2,2} },
  { "Lydian",        7, {2,2,2,1,2,2,1} },
  { "Mixolydian",    7, {2,2,1,2,2,1,2} },
  { "Locrian",       7, {1,2,2,1,2,2,2} },
  { "Ahava Raba",    7, {1,3,1,2,1,2,2} },
  { "Pent Maj",      5, {2,2,3,2,3} },
  { "Pent Min",      5, {3,2,2,3,2} },
  { "Blues",         6, {3,2,1,1,3,2} },
  { "Whole tone",    1, {2} },
  { "Diminished",    2, {1,2} },
  { "Hungarian Min", 7, {2,1,3,1,1,3,1} },
  { "Japanese",      5, {1,4,2,3,2} },
  { "Hirajoshi",     5, {2,1,4,1,4} },
  { "Iwato",         5, {1,4,1,4,2} },
  { "Yo",            5, {2,3,2,2,3} },
  { "Chromatic",     1, {1} }
};
// ==============

// === Constants ===
const uint8_t MIDI_MIN = 0;
const uint8_t MIDI_MAX = 127;
const uint8_t DEFAULT_VELOCITY = 64;
const uint8_t MAX_MODES = 8;
const uint8_t MAX_MENU_ROWS = 8;
const uint8_t NUMBER_OF_SCALES = ARRAY_SIZE(SCALES);
// =================

// === Modes ===
enum mode_t : uint8_t {
  MODE_L_NOTE_R_CC = 0,                 // Left - Notes, Right - Control changes
  MODE_L_NOTE_R_CC_INVERTED = 1,        // Left - Notes, Right - Control changes (Inverted)
  MODE_L_NOTE_R_VELOCITY = 2,           // Left - Notes, Right - Velocity
  MODE_L_NOTE_R_VELOCITY_INVERTED = 3,  // Left - Notes, Right - Velocity (Inverted)
  MODE_L_CC_R_NOTE = 4,                 // Left - Control changes, Right - Notes
  MODE_L_CC_INVERTED_R_NOTE = 5,        // Left - Control changes (Inverted), Right - Notes
  MODE_L_VELOCITY_R_NOTE = 6,           // Left - Velocity, Right - Notes
  MODE_L_VELOCITY_INVERTED_R_NOTE = 7   // Left - Velocity (Inverted), Right - Notes
};
// ==============

// === Menu ===
enum menu_t : uint8_t {
  MENU_ROOT_NOTE = 0,
  MENU_SCALE = 1,
  MENU_MODE = 2,
  MENU_NOTES = 3,
  MENU_DISTANCE_STEP = 4,
  MENU_INTERVAL = 5,
  MENU_MIDI = 6,
  MENU_CC = 7,
};
// ============

// === Global Variables ===
uint16_t global_dynamic_distance_left = 0;
uint16_t global_dynamic_distance_right = 0;
uint16_t global_current_distance_left = 0;
uint16_t global_current_distance_right = 0;

uint8_t global_root_note = 36;
int8_t global_note_index = -1;
int8_t global_current_note = -1;
int8_t global_previous_note = -1;
uint8_t global_number_of_notes = 14;

uint8_t global_current_scale_index = 0;

uint8_t global_interval = 120;
uint8_t global_midi_channel = 0;
uint8_t global_control_change = 0;
uint8_t global_previous_control_value = 0;
uint8_t global_current_control_value = 0;

uint16_t global_distance_step = 15;
int16_t global_min_distance = 30;
int16_t global_max_distance = (global_min_distance + global_distance_step * global_number_of_notes);

uint8_t global_velocity = 64;
uint8_t global_previous_velocity = 64;

uint8_t global_mode = 0;
uint8_t global_menu_selected_row = 0;
// =========================

#endif