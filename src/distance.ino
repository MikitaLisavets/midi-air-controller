#include <distance.h>

void setup_distance(){
  Wire.begin();

  pinMode(VL53L0X_LEFT_PIN, OUTPUT);
  pinMode(VL53L0X_RIGHT_PIN, OUTPUT);

  digitalWrite(VL53L0X_LEFT_PIN, LOW);
  digitalWrite(VL53L0X_RIGHT_PIN, LOW);

  pinMode(VL53L0X_LEFT_PIN, INPUT);
  sensor_left.init(true);
  sensor_left.setAddress(VL53L0X_LEFT_ADDRESS);

  pinMode(VL53L0X_RIGHT_PIN, INPUT);
  sensor_right.init(true);
  sensor_right.setAddress(VL53L0X_RIGHT_ADDRESS);

  sensor_left.setMeasurementTimingBudget(20000);
  sensor_right.setMeasurementTimingBudget(20000);
}

void loop_distance_left() {
  global_dynamic_distance[LEFT_SIDE] = sensor_left.readRangeSingleMillimeters();
}

void loop_distance_right() {
  global_dynamic_distance[RIGHT_SIDE] = sensor_right.readRangeSingleMillimeters();
}