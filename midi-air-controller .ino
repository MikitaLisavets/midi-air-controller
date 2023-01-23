#include <Thread.h> // Source: https://github.com/ivanseidel/ArduinoThread
#include <StaticThreadController.h> // Source: https://github.com/ivanseidel/ArduinoThread

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

#define MODE_NOTE 0
#define MODE_VALUE 1
#define MODE_VALUE_INVERTED 2

Thread thread_distance;
Thread thread_midi;
Thread thread_controls;

StaticThreadController<3> main_thread (&thread_distance, &thread_midi, &thread_controls);

// === Scales ===
char* note_names[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

byte MAJOR_SCALE[] = {2,2,1,2,2,2,1};
byte NATURAL_MINOR_SCALE[] = {2,1,2,2,1,2,2};
byte HARMONIC_MAJOR_SCALE[] = {2,2,1,2,1,3,1};
byte HARMONIC_MINOR_SCALE[] = {2,1,2,2,1,3,1};
byte MELODIC_MINOR_SCALE[] = {2,1,2,2,2,2,1};
byte DORIAN_SCALE[] = {2,1,2,2,2,1,2};
byte PHRYGIAN_SCALE[] = {1,2,2,2,1,2,2};
byte LYDIAN_SCALE[] = {2,2,2,1,2,2,1};
byte MIXOLYDIAN_SCALE[] = {2,2,1,2,2,1,2};
byte LOCRIAN_SCALE[] = {1,2,2,1,2,2,2};
byte AHAVA_RABA_SCALE[] = {1,3,1,2,1,2,2};
byte PENTATONIC_MAJOR_SCALE[] = {2,2,3,2,3};
byte PENTATONIC_MINOR_SCALE[] = {3,2,2,3,2};
byte BLUES_SCALE[] = {3,2,1,1,3,2};
byte WHOLE_TONE_SCALE[] = {2,2,2,2,2,2};
byte DIMINISHED_SCALE[] = {1,2,1,2,1,2,1,2};
byte WHOLE_HALF_DIMINISHED_SCALE[] = {2,1,2,1,2,1,2,1};
byte BEBOP_DOMINANT_SCALE[] = {2,2,1,2,2,1,1,1};
byte BEBOP_MAJOR_SCALE[] = {2,2,1,2,1,1,2,1};
byte HUNGARIAN_MINOR_SCALE[] = {2,1,3,1,1,3,1};
byte CHROMATIC_SCALE[] = {1,1,1,1,1,1,1,1,1,1,1,1};

char* scales_names[] = {
  "Major",
  "Natural Min", 
  "Harmonic Maj",
  "Harmonic Min",
  "Harmonic Maj",
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

byte* scales_steps[] = {
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

const uint8_t scales_sizes[] PROGMEM = {
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
byte global_current_distance = 0;
byte global_current_note = 0;
byte global_note_index = -1;

int global_min_distance = 20;
int global_max_distance = 4000;
int global_distance_step = 20;

byte global_previous_note = 0;

byte global_velocity = 64;
int global_midi_channel = 0;
byte global_max_midi_channel = 127;

int global_current_scale_index = 0;

byte global_root_note = 48;
byte global_max_note = 127;

int global_number_of_notes = 14;
byte global_max_number_of_notes = 96;

byte global_number_of_scales = ARRAY_SIZE(scales_names);

int global_selected_row = 0;
byte global_max_rows = 6;

int global_mode = 0;
byte global_max_modes = 3;

int global_control_channel = 0;

// =========================

void setup() {
  setup_display();
  setup_controls();

  thread_distance.setInterval(10);
  thread_distance.onRun(loop_distance);

  thread_midi.setInterval(100);
  thread_midi.onRun(loop_midi);

  thread_controls.setInterval(100);
  thread_controls.onRun(loop_controls);
}

void loop() {
  main_thread.run();

  render_display();
}
