#ifndef __HELPERS_H
#define __HELPERS_H

#include <main.h>

bool inRange(int16_t val, int16_t minimum, int16_t maximum);
void set_selected_row(int8_t row);
void set_current_scale_index(int8_t index);
void set_number_of_notes(int8_t number);
void set_midi_channel(int8_t channel);
void set_distance_step(int8_t step);
void set_mode(int8_t mode);
void set_bpm(int16_t bpm);
void set_note_duration(int8_t note_duration);
void set_root_note(int8_t note);
void set_control_change(int8_t cc);
void toggle_leds();

#endif