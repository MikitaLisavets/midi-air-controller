#include <RunningMedian.h>
#include <hcsr04.h>

#define TRIG_PIN 10
#define ECHO_PIN 9

#define DISTANCE_ACCURACY 10
#define DISTANCE_CHECK_TIMEOUT 5

RunningMedian distancies = RunningMedian(DISTANCE_ACCURACY);

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 0, 4000);

int getDistance() {
  int i = 0;

  while (i < DISTANCE_ACCURACY) {
    distancies.add(hcsr04.distanceInMillimeters());
    delay(DISTANCE_CHECK_TIMEOUT);
    i++;
  }

  int distance = round(distancies.getMedian());

  return distance;
}