#ifndef __MAIN_H
#define __MAIN_H

#include <arduino.h>
#include <stdio.h>
#include <Thread.h> // Source: https://github.com/ivanseidel/ArduinoThread
#include <StaticThreadController.h> // Source: https://github.com/ivanseidel/ArduinoThread
#include <Wire.h>
#include <EEPROM.h>

#include <display.h>
#include <distance.h>
#include <controls.h>
#include <midi.h>
#include <helpers.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

#define VERSION 1.04

#define STR_ADDR 0

Thread thread_display;
Thread thread_distance_left;
Thread thread_distance_right;
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
  { "Arabic",        7, {2,2,1,1,2,2,2} },
  { "Pent Maj",      5, {2,2,3,2,3} },
  { "Pent Min",      5, {3,2,2,3,2} },
  { "Blues",         6, {3,2,1,1,3,2} },
  { "Whole tone",    1, {2} },
  { "Diminished",    2, {1,2} },
  { "Hungarian Min", 7, {2,1,3,1,1,3,1} },
  { "Romanian Min",  7, {2,1,3,1,2,1,2} },
  { "Japanese",      5, {1,4,2,3,2} },
  { "Hirajoshi",     5, {2,1,4,1,4} },
  { "Iwato",         5, {1,4,1,4,2} },
  { "Yo",            5, {2,3,2,2,3} },
  { "Chromatic",     1, {1} }
};
// ==============

// === Constants ===
const uint8_t MIDI_MIN = 0;
const uint8_t MIDI_MAX = 128;
const uint8_t DEFAULT_VELOCITY = 64;
const uint8_t MAX_MODES = 5;
const uint8_t MAX_MENU_ROWS = 13;
const uint8_t NUMBER_OF_SCALES = ARRAY_SIZE(SCALES);
const uint8_t LEFT_SIDE = 0;
const uint8_t RIGHT_SIDE = 1;
// =================

// === Modes ===
enum mode_t : uint8_t {
  MODE_NOTE = 0,
  MODE_CC,
  MODE_CC_INVERTED,
  MODE_VELOCITY,
  MODE_VELOCITY_INVERTED
};
// ==============

// === Menu ===
enum menu_t : uint8_t {
  MENU_SIDE = 0,
  MENU_MODE,
  MENU_ROOT_NOTE,
  MENU_SCALE,
  MENU_NOTES,
  MENU_DISTANCE_STEP,
  MENU_NOTE_TIMEOUT,
  MENU_MIDI,
  MENU_CC,
  MENU_LOAD,
  MENU_SAVE,
  MENU_RESET,
  MENU_AUTO_LOAD_SETTINGS,
};
// ============


// === Settings ===
struct Settings {
  uint8_t side;
  uint8_t mode[2];
  uint8_t root_note[2];
  uint8_t number_of_notes[2];
  uint8_t scale_index[2];
  uint16_t note_timeout[2];
  uint8_t midi_channel[2];
  uint8_t control_change[2];
  uint16_t distance_step[2];

  bool autoLoadSettings;
};

extern Settings settings;
// ================

// === Global Variables ===
uint8_t global_menu_selected_row = 0;

uint16_t global_dynamic_distance[2] = {0, 0};
uint16_t global_current_distance[2] = {0, 0};

int8_t global_note_index[2] = {-1, -1};
int8_t global_current_note[2] = {-1, -1};
int8_t global_previous_note[2] = {-1, -1};

uint8_t global_previous_control_value[2] = {0, 0};
uint8_t global_current_control_value[2] = {0, 0};

uint16_t global_min_distance[2] = {30, 30};
uint16_t global_max_distance[2] = {
  (global_min_distance[LEFT_SIDE] + settings.distance_step[LEFT_SIDE] * settings.number_of_notes[LEFT_SIDE]),
  (global_min_distance[RIGHT_SIDE] + settings.distance_step[RIGHT_SIDE] * settings.number_of_notes[RIGHT_SIDE])
};

uint8_t global_velocity = DEFAULT_VELOCITY;
uint8_t global_previous_velocity = DEFAULT_VELOCITY;

bool global_is_display_enabled = true;
bool global_is_led_enabled = false;
// =========================

#endif