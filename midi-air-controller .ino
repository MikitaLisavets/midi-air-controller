#include <Thread.h> // Source: https://github.com/ivanseidel/ArduinoThread
#include <StaticThreadController.h> // Source: https://github.com/ivanseidel/ArduinoThread

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

// Thread thread_display;
Thread thread_distance;
Thread thread_midi;
Thread thread_controls;

StaticThreadController<3> main_thread (&thread_distance, &thread_midi, &thread_controls);

// === Scales ===
String note_names[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

int MAJOR_SCALE[] = {2,2,1,2,2,2,1};
// int NATURAL_MINOR_SCALE[] = {2,1,2,2,1,2,2};
// int HARMONIC_MAJOR_SCALE[] = {2,2,1,2,1,3,1};
// int HARMONIC_MINOR_SCALE[] = {2,1,2,2,1,3,1};
// int MELODIC_MINOR_SCALE[] = {2,1,2,2,2,2,1};
// int DORIAN_SCALE[] = {2,1,2,2,2,1,2};
// int PHRYGIAN_SCALE[] = {1,2,2,2,1,2,2};
// int LYDIAN_SCALE[] = {2,2,2,1,2,2,1};
// int MIXOLYDIAN_SCALE[] = {2,2,1,2,2,1,2};
// int LOCRIAN_SCALE[] = {1,2,2,1,2,2,2};
// int AHAVA_RABA_SCALE[] = {1,3,1,2,1,2,2};
int PENTATONIC_MAJOR_SCALE[] = {2,2,3,2,3};
int PENTATONIC_MINOR_SCALE[] = {3,2,2,3,2};
int BLUES_SCALE[] = {3,2,1,1,3,2};
// int WHOLE_TONE_SCALE[] = {2,2,2,2,2,2};
// int DIMINISHED_SCALE[] = {1,2,1,2,1,2,1,2};
// int WHOLE_HALF_DIMINISHED_SCALE[] = {2,1,2,1,2,1,2,1};
// int BEBOP_DOMINANT_SCALE[] = {2,2,1,2,2,1,1,1};
// int BEBOP_MAJOR_SCALE[] = {2,2,1,2,1,1,2,1};
// int HUNGARIAN_MINOR_SCALE[] = {2,1,3,1,1,3,1};
// int CHROMATIC_SCALE[] = {1,1,1,1,1,1,1,1,1,1,1,1};

char* scales_names[] = {
  "Major",
  // "Naturl Minor", 
  // "Harmonic Major",
  // "Harmonic Minor"
  // "Harmonic Major",
  // "Melodic Minor",
  // "Dorian", 
  // "Phrygian",
  // "Lydian",
  // "Mixolydian",
  // "Locrian",
  // "Ahava Raba",
  "Pentatonic Major",
  "Pentatonic Minor",
  "Blues",
  // "Whole tone",
  // "Deminished",
  // "Whole-half Deminished",
  // "Bebop domimant",
  // "Bebop Major",
  // "Hungarian Minor",
  // "Chromatic"
};

int* scales_steps[] = {
  MAJOR_SCALE,
  // NATURAL_MINOR_SCALE,
  // HARMONIC_MAJOR_SCALE,
  // HARMONIC_MINOR_SCALE,
  // MELODIC_MINOR_SCALE,
  // DORIAN_SCALE,
  // PHRYGIAN_SCALE,
  // LYDIAN_SCALE,
  // MIXOLYDIAN_SCALE,
  // LOCRIAN_SCALE,
  // AHAVA_RABA_SCALE,
  PENTATONIC_MAJOR_SCALE,
  PENTATONIC_MINOR_SCALE,
  BLUES_SCALE,
  // WHOLE_TONE_SCALE,
  // DIMINISHED_SCALE,
  // WHOLE_HALF_DIMINISHED_SCALE,
  // BEBOP_DOMINANT_SCALE,
  // BEBOP_MAJOR_SCALE,
  // HUNGARIAN_MINOR_SCALE,
  // CHROMATIC_SCALE
};

int scales_sizes[] = {
  ARRAY_SIZE(MAJOR_SCALE),
  // ARRAY_SIZE(NATURAL_MINOR_SCALE),
  // ARRAY_SIZE(HARMONIC_MAJOR_SCALE),
  // ARRAY_SIZE(HARMONIC_MINOR_SCALE),
  // ARRAY_SIZE(MELODIC_MINOR_SCALE),
  // ARRAY_SIZE(DORIAN_SCALE),
  // ARRAY_SIZE(PHRYGIAN_SCALE),
  // ARRAY_SIZE(LYDIAN_SCALE),
  // ARRAY_SIZE(MIXOLYDIAN_SCALE),
  // ARRAY_SIZE(LOCRIAN_SCALE),
  // ARRAY_SIZE(AHAVA_RABA_SCALE),
  ARRAY_SIZE(PENTATONIC_MAJOR_SCALE),
  ARRAY_SIZE(PENTATONIC_MINOR_SCALE),
  ARRAY_SIZE(BLUES_SCALE),
  // ARRAY_SIZE(WHOLE_TONE_SCALE),
  // ARRAY_SIZE(DIMINISHED_SCALE),
  // ARRAY_SIZE(WHOLE_HALF_DIMINISHED_SCALE),
  // ARRAY_SIZE(BEBOP_DOMINANT_SCALE),
  // ARRAY_SIZE(BEBOP_MAJOR_SCALE),
  // ARRAY_SIZE(HUNGARIAN_MINOR_SCALE),
  // ARRAY_SIZE(CHROMATIC_SCALE)
};
// ==============

// === Global Variables ===
int global_current_distance = 0;
int global_current_note = -1;

int global_minimal_distance = 20;
int global_distance_step = 20;

int global_previous_note = -1;

int global_velocity = 64;
int global_midi_channel = 0;
int global_max_midi_channel = 127;

int global_current_scale_index = 0;

int global_root_note = 48;
int global_max_note = 127;

int global_number_of_notes = 14;
int global_max_number_of_notes = 96;

int global_number_of_scales = ARRAY_SIZE(scales_names);

int global_selected_row = 0;
int global_max_rows = 5;

bool global_is_pitch = false;

// =========================

void setup() {
  Serial.begin(9600);

  setup_display();
  setup_controls();

  // thread_display.setInterval(500);
  // thread_display.onRun(loop_display);

  thread_distance.setInterval(10);
  thread_distance.onRun(loop_distance);

  thread_midi.setInterval(100);
  thread_midi.onRun(loop_midi);

  thread_controls.setInterval(50);
  thread_controls.onRun(loop_controls);
}

void loop() {
  main_thread.run();

  render_display(
    global_current_distance,
    global_current_note,
    global_root_note,
    global_current_scale_index,
    global_number_of_notes,
    global_midi_channel,
    global_is_pitch,
    global_selected_row
  );
}
