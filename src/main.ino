#include <main.h>

StaticThreadController<4> main_thread (&thread_display, &thread_midi_left, &thread_midi_right, &thread_controls);
Settings settings;

void setup() {
  setup_display();
  setup_distance(); // NOTE: Setup VL53L0X after display only
  setup_controls();

  toggle_leds();
  render_init_screen();

  load_settings();
  if (!settings.autoLoadSettings) {
    reset_settings();
  }

  global_max_distance[LEFT_SIDE] = global_min_distance[LEFT_SIDE] + (settings.distance_step[LEFT_SIDE] * settings.number_of_notes[LEFT_SIDE]);
  global_max_distance[RIGHT_SIDE] = global_min_distance[RIGHT_SIDE] + (settings.distance_step[RIGHT_SIDE] * settings.number_of_notes[RIGHT_SIDE]);


  thread_display.onRun(loop_display);
  thread_midi_left.onRun(loop_midi_left);
  thread_midi_right.onRun(loop_midi_right);
  thread_controls.onRun(loop_controls);
}

void loop() {
  main_thread.run();
}
