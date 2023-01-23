#include <RunningMedian.h>
#include <hcsr04.h>

#define TRIG_PIN 10
#define ECHO_PIN 9

#define DISTANCE_ACCURACY 5
#define DISTANCE_CHECK_TIMEOUT 4

RunningMedian distancies = RunningMedian(DISTANCE_ACCURACY);

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, global_min_distance, global_max_distance);

int getDistance() {
  byte i = 0;

  while (i < DISTANCE_ACCURACY) {
    distancies.add(hcsr04.distanceInMillimeters());
    delay(DISTANCE_CHECK_TIMEOUT);
    i++;
  }

  // int distance = hcsr04.distanceInMillimeters();
  int distance = round(distancies.getMedian());

  return distance;
}

void loop_distance() {
  global_current_distance = getDistance();
}