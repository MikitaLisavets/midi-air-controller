#include <RunningMedian.h>
#include <hcsr04.h>

#define TRIG_PIN 10
#define ECHO_PIN 9

#define DISTANCE_ACCURACY 3

RunningMedian distancies = RunningMedian(DISTANCE_ACCURACY);

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, global_min_distance, 9999);

void loop_distance() {
  distancies.add(hcsr04.distanceInMillimeters());
  global_current_distance = round(distancies.getMedian());
}