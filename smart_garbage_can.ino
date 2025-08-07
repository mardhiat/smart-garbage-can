#include <Servo.h>

const int metalSensorPin = A0;
const int greenLedPin = 3;
const int redLedPin = 4;
const int servo1Pin = 5;
const int servo2Pin = 6;

const int threshold = 500;  // Adjust after testing

Servo servo1;
Servo servo2;

void setup() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  int sensorValue = analogRead(metalSensorPin);

  if (sensorValue > threshold) {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    servo1.write(180);
    servo2.write(180);
  } else {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    servo1.write(0);
    servo2.write(0);
  }

  delay(200);
}
