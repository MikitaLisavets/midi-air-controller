#include <main.h>

StaticThreadController<5> main_thread (&thread_display, &thread_distance, &thread_midi_left, &thread_midi_right, &thread_controls);

void setup() {
  setup_display();
  setup_distance(); // NOTE: Setup VL53L0X only after display
  setup_controls();

  thread_display.setInterval(60);
  thread_display.onRun(loop_display);

  thread_distance.setInterval(20);
  thread_distance.onRun(loop_distance);

  thread_midi_left.setInterval(global_interval);
  thread_midi_left.onRun(loop_midi_left);

  thread_midi_right.setInterval(global_interval);
  thread_midi_right.onRun(loop_midi_right);

  thread_controls.setInterval(100);
  thread_controls.onRun(loop_controls);
}

void loop() {
  main_thread.run();
}
