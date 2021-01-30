#include <Arduino.h>
#include <ESP32Servo.h>

// Functions
bool is_positive(int value);

// Servo settings
int servo_pin = 4;
int servo_min_us = 750;
int servo_max_us = 2400;
Servo feeder_servo;
int servo_hz = 50;

// Commands
String toggle = "toggle";

int current_angle = -90;
int angle_offset = 90;
bool is_toggled = false;
int dir_multiplier = 1; // sweep direction, either 1 or -1



void setup() {
  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  // Set up debug serial
	Serial.begin(115200);
  Serial.println("Setting up HamsterFeeder V1.0");

  // Set up servo output
  feeder_servo.setPeriodHertz(servo_hz);
  feeder_servo.attach(servo_pin, servo_min_us, servo_max_us);

  // Center servo
  feeder_servo.write(current_angle + angle_offset);
}

void loop() {
  // Read command from serial TODO func
  String input = Serial.readString();

  if(input.indexOf(toggle) >= 0){
    is_toggled = true;
    Serial.println("Toggling!");
  }

  if(is_toggled){
    int target_angle = 90*dir_multiplier;
    Serial.print("moving from ");
    Serial.print(current_angle);
    Serial.print(" to ");
    Serial.println(target_angle);

    while(abs(current_angle) < abs(target_angle) || is_positive(current_angle) != is_positive(target_angle)){
      current_angle += dir_multiplier;

      feeder_servo.write(current_angle + angle_offset);
      delay(10); // waits 10ms for the servo to reach the position //TODO use millis
    }

    is_toggled = false;
    dir_multiplier = -dir_multiplier;
    delay(10);
    Serial.print("Dir multi now:");
    Serial.println(dir_multiplier);
  }
}

// TODO
void getCommandFromSerial() {

}

// TODO
bool is_positive(int value) {
 return value>=0;
}
