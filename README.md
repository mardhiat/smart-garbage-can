# Smart Garbage Can (Arduino Project)

This is an Arduino-powered smart garbage can that detects **metal**, **plastic**, and **general waste**, then uses servo motors to sort them into separate bins.

## Objective
To automate waste sorting using sensors and servos, helping with eco-friendly recycling and efficient disposal.

## Components
- Metal sensor (analog)
- Plastic detection sensor (e.g., IR sensor or ML mock)
- 2x Servo motors (for lid movement or bin rotation)
- 3x LED lights (green for metal, blue for plastic, red for trash)
- Arduino Uno
- Breadboard + jumper wires

## How It Works
- The garbage can uses sensors to detect what kind of material is inserted.
- Based on detection:
  - **Metal** ➝ Green LED on ➝ Servo rotates to metal bin
  - **Plastic** ➝ Blue LED on ➝ Servo rotates to plastic bin
  - **Other trash** ➝ Red LED on ➝ Servo keeps default position
- After sorting, the lid closes and resets for the next item.

## Features
- Automatic waste classification
- Real-time feedback with colored LEDs
- Motorized sorting with servo control
- Expandable for smart home or school projects

## Media

All project photos and videos are available here:  
📁 [View on Google Drive](https://drive.google.com/drive/folders/1WMvN9db4KP4SwLWDAoNWmgD6A0NWS14E?usp=sharing)

## Code

## 💻 Source Code

```cpp
int ledGreen = 5;
int ledRed = 6;
int servoPin = 9;
int metalSensor = A0;  // Analog metal sensor
int sensorThreshold = 100;

#include <Servo.h>
Servo myServo;

void setup() {
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  myServo.attach(servoPin);
  myServo.write(0);  // Initial position
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(metalSensor);
  Serial.println(sensorValue);

  if (sensorValue > sensorThreshold) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
    myServo.write(90);  // Rotate to push metal item
    delay(1000);
    myServo.write(0);   // Reset
  } else {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);
    myServo.write(0);  // Stay still
  }

  delay(500);
}

