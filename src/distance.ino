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
  sensor_left.setMeasurementTimingBudget(20000);

  pinMode(VL53L0X_RIGHT_PIN, INPUT);
  sensor_right.init(true);
  sensor_right.setAddress(VL53L0X_RIGHT_ADDRESS);
  sensor_right.setMeasurementTimingBudget(20000);
}

uint16_t readSensor(uint8_t side) {
  if (side == LEFT_SIDE) {
    return sensor_left.readRangeSingleMillimeters();
  } else {
    return sensor_right.readRangeSingleMillimeters();
  }
}