#include <VL53L0X.h> // Source: https://github.com/pololu/vl53l0x-arduino

#define VL53L0X_LEFT_ADDRESS 0x30
#define VL53L0X_RIGHT_ADDRESS 0x31

#define VL53L0X_LEFT_PIN 4
#define VL53L0X_RIGHT_PIN 14

VL53L0X sensor_left;
VL53L0X sensor_right;

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
}

void loop_distance() {
  global_dynamic_distance_left = sensor_left.readRangeSingleMillimeters();
  global_dynamic_distance_right = sensor_right.readRangeSingleMillimeters();
}