#ifndef __DISTANCE_H
#define __DISTANCE_H

#include <main.h>
#include <VL53L0X.h> // Source: https://github.com/pololu/vl53l0x-arduino

const uint8_t VL53L0X_LEFT_ADDRESS = 0x30;
const uint8_t VL53L0X_RIGHT_ADDRESS = 0x31;

const uint8_t VL53L0X_LEFT_PIN = 4;
const uint8_t VL53L0X_RIGHT_PIN = 14;

VL53L0X sensor_left;
VL53L0X sensor_right;

void setup_distance();
void loop_distance();

#endif