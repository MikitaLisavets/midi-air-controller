#include <Thread.h> // Source: https://github.com/ivanseidel/ArduinoThread
#include <StaticThreadController.h> // Source: https://github.com/ivanseidel/ArduinoThread
#include <Wire.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

#define MIDI_MIN 0
#define MIDI_MAX 127
#define DEFAULT_VELOCITY 64

// === Modes ===
#define MODE_L_NOTE_R_CC 0                // Left - Notes, Right - Control changes
#define MODE_L_NOTE_R_CC_INVERTED 1       // Left - Notes, Right - Control changes (Inverted)
#define MODE_L_NOTE_R_VELOCITY 2          // Left - Notes, Right - Velocity
#define MODE_L_NOTE_R_VELOCITY_INVERTED 3 // Left - Notes, Right - Velocity (Inverted)
#define MODE_L_CC_R_NOTE 4                // Left - Control changes, Right - Notes
#define MODE_L_CC_INVERTED_R_NOTE 5       // Left - Control changes (Inverted), Right - Notes
#define MODE_L_VELOCITY_R_NOTE 6          // Left - Velocity, Right - Notes
#define MODE_L_VELOCITY_INVERTED_R_NOTE 7 // Left - Velocity (Inverted), Right - Notes
// ==============

Thread thread_display;
Thread thread_distance;
Thread thread_midi_left;
Thread thread_midi_right;
Thread thread_controls;

StaticThreadController<5> main_thread (&thread_display, &thread_distance, &thread_midi_left, &thread_midi_right, &thread_controls);

// === Scales ===
constexpr char*  global_note_names[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

constexpr byte MAJOR_SCALE[] = {2,2,1,2,2,2,1};
constexpr byte NATURAL_MINOR_SCALE[] = {2,1,2,2,1,2,2};
constexpr byte HARMONIC_MAJOR_SCALE[] = {2,2,1,2,1,3,1};
constexpr byte HARMONIC_MINOR_SCALE[] = {2,1,2,2,1,3,1};
constexpr byte MELODIC_MINOR_SCALE[] = {2,1,2,2,2,2,1};
constexpr byte DORIAN_SCALE[] = {2,1,2,2,2,1,2};
constexpr byte PHRYGIAN_SCALE[] = {1,2,2,2,1,2,2};
constexpr byte LYDIAN_SCALE[] = {2,2,2,1,2,2,1};
constexpr byte MIXOLYDIAN_SCALE[] = {2,2,1,2,2,1,2};
constexpr byte LOCRIAN_SCALE[] = {1,2,2,1,2,2,2};
constexpr byte AHAVA_RABA_SCALE[] = {1,3,1,2,1,2,2};
constexpr byte PENTATONIC_MAJOR_SCALE[] = {2,2,3,2,3};
constexpr byte PENTATONIC_MINOR_SCALE[] = {3,2,2,3,2};
constexpr byte BLUES_SCALE[] = {3,2,1,1,3,2};
constexpr byte WHOLE_TONE_SCALE[] = {2};
constexpr byte DIMINISHED_SCALE[] = {1,2};
constexpr byte WHOLE_HALF_DIMINISHED_SCALE[] = {2,1};
constexpr byte BEBOP_DOMINANT_SCALE[] = {2,2,1,2,2,1,1,1};
constexpr byte BEBOP_MAJOR_SCALE[] = {2,2,1,2,1,1,2,1};
constexpr byte HUNGARIAN_MINOR_SCALE[] = {2,1,3,1,1,3,1};
constexpr byte CHROMATIC_SCALE[] = {1};

constexpr char* global_scales_names[] = {
  "Major",
  "Natural Min", 
  "Harmonic Maj",
  "Harmonic Min",
  "Melodic Min",
  "Dorian", 
  "Phrygian",
  "Lydian",
  "Mixolydian",
  "Locrian",
  "Ahava Raba",
  "Pent Maj",
  "Pent Min",
  "Blues",
  "Whole tone",
  "Deminished",
  "Wholehalf Dem",
  "Bebop Dom",
  "Bebop Maj",
  "Hungarian Min",
  "Chromatic"
};

constexpr byte* scales_steps[] = {
  MAJOR_SCALE,
  NATURAL_MINOR_SCALE,
  HARMONIC_MAJOR_SCALE,
  HARMONIC_MINOR_SCALE,
  MELODIC_MINOR_SCALE,
  DORIAN_SCALE,
  PHRYGIAN_SCALE,
  LYDIAN_SCALE,
  MIXOLYDIAN_SCALE,
  LOCRIAN_SCALE,
  AHAVA_RABA_SCALE,
  PENTATONIC_MAJOR_SCALE,
  PENTATONIC_MINOR_SCALE,
  BLUES_SCALE,
  WHOLE_TONE_SCALE,
  DIMINISHED_SCALE,
  WHOLE_HALF_DIMINISHED_SCALE,
  BEBOP_DOMINANT_SCALE,
  BEBOP_MAJOR_SCALE,
  HUNGARIAN_MINOR_SCALE,
  CHROMATIC_SCALE
};

byte constexpr scales_sizes[] = {
  ARRAY_SIZE(MAJOR_SCALE),
  ARRAY_SIZE(NATURAL_MINOR_SCALE),
  ARRAY_SIZE(HARMONIC_MAJOR_SCALE),
  ARRAY_SIZE(HARMONIC_MINOR_SCALE),
  ARRAY_SIZE(MELODIC_MINOR_SCALE),
  ARRAY_SIZE(DORIAN_SCALE),
  ARRAY_SIZE(PHRYGIAN_SCALE),
  ARRAY_SIZE(LYDIAN_SCALE),
  ARRAY_SIZE(MIXOLYDIAN_SCALE),
  ARRAY_SIZE(LOCRIAN_SCALE),
  ARRAY_SIZE(AHAVA_RABA_SCALE),
  ARRAY_SIZE(PENTATONIC_MAJOR_SCALE),
  ARRAY_SIZE(PENTATONIC_MINOR_SCALE),
  ARRAY_SIZE(BLUES_SCALE),
  ARRAY_SIZE(WHOLE_TONE_SCALE),
  ARRAY_SIZE(DIMINISHED_SCALE),
  ARRAY_SIZE(WHOLE_HALF_DIMINISHED_SCALE),
  ARRAY_SIZE(BEBOP_DOMINANT_SCALE),
  ARRAY_SIZE(BEBOP_MAJOR_SCALE),
  ARRAY_SIZE(HUNGARIAN_MINOR_SCALE),
  ARRAY_SIZE(CHROMATIC_SCALE)
};
// ==============

// === Global Variables ===
int global_dynamic_distance_left = 0;
int global_dynamic_distance_right = 0;

int global_current_distance_left = 0;
int global_current_distance_right = 0;

int global_current_note = -1;
int global_note_index = -1;
int global_previous_note = -1;

int global_previous_control_value = 0;
int global_current_control_value = 0;

int global_number_of_notes = 14;
int global_max_number_of_notes = 96;

int global_distance_step = 15;
int global_min_distance = 40;
int global_max_distance = (global_min_distance + global_distance_step * global_number_of_notes);

int global_velocity = 64;
int global_previous_velocity = 64;
int global_midi_channel = 0;

int global_current_scale_index = 0;

int global_root_note = 36;

int global_number_of_scales = ARRAY_SIZE(global_scales_names);

int global_selected_row = 0;
int global_max_rows = 8;

int global_mode = 0;
int global_max_modes = 8;

int global_control_change = 0;

int global_tempo = 120;

constexpr byte global_menu_root_note = 0;
constexpr byte global_menu_scale = 1;
constexpr byte global_menu_mode = 2;
constexpr byte global_menu_notes = 3;
constexpr byte global_menu_distance_step = 4;
constexpr byte global_menu_tempo = 5;
constexpr byte global_menu_midi = 6;
constexpr byte global_menu_control_change = 7;
// =========================

void setup() {
  setup_display();
  setup_distance(); // NOTE: Setup VL53L0X only after display
  setup_controls();

  thread_display.setInterval(60);
  thread_display.onRun(loop_display);

  thread_distance.setInterval(20);
  thread_distance.onRun(loop_distance);

  thread_midi_left.setInterval(global_tempo);
  thread_midi_left.onRun(loop_midi_left);

  thread_midi_right.setInterval(global_tempo);
  thread_midi_right.onRun(loop_midi_right);

  thread_controls.setInterval(100);
  thread_controls.onRun(loop_controls);
}

void loop() {
  main_thread.run();
}
