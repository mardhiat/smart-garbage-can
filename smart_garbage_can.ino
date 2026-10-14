#include <Servo.h>

// Arduino Uno R3. One inductive proximity sensor on A0.
//
// Metal latches when the reading rises above threshold + hysteresis.
// It releases when the reading falls below threshold - hysteresis.
// The gap keeps a noisy sensor from throwing the servos back and forth.
//
// Pin 6 is the lid. Pin 5 is the diverter. Each one is written once per
// edge: the lid opens first and closes last. Both use the original stops,
// 0 and 180 degrees. Bring-up photos show the servos loose, before a bin
// was built, so those names are the roles in this sequence.
//
// Green means metal is latched. Red means it is not. A low reading is
// "no metal" (empty sensor, or an object this sensor ignores). It is not
// a plastic or trash classification.
//
// Serial is 9600 baud. 500 counts is about 2.44 V on the Uno's 5 V ADC.
// Keep A0 between 0 V and 5 V.

const int metalSensorPin = A0;
const int greenLedPin = 3;
const int redLedPin = 4;
const int diverterPin = 5;
const int lidPin = 6;

const int sensorThreshold = 500;
const int hysteresis = 40;
const int homeAngle = 0;
const int activeAngle = 180;

const unsigned long motionSettleMs = 400;
const unsigned long reportIntervalMs = 500;
const unsigned long sampleIntervalMs = 20;

Servo diverter;
Servo lid;

bool metalLatched = false;
unsigned long lastReportMs = 0;

void showMetal() {
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
}

void showNoMetal() {
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
}

void reportNow(const __FlashStringHelper* label, int reading) {
  lastReportMs = millis();
  Serial.print(label);
  Serial.print(',');
  Serial.println(reading);
}

void reportReading(const __FlashStringHelper* label, int reading) {
  const unsigned long now = millis();
  if (now - lastReportMs < reportIntervalMs) {
    return;
  }
  reportNow(label, reading);
}

void latchMetal(int reading) {
  metalLatched = true;
  showMetal();
  reportNow(F("metal"), reading);

  lid.write(activeAngle);
  delay(motionSettleMs);
  diverter.write(activeAngle);
}

void releaseMetal(int reading) {
  metalLatched = false;
  reportNow(F("no-metal"), reading);

  diverter.write(homeAngle);
  delay(motionSettleMs);
  lid.write(homeAngle);
  showNoMetal();
}

void setup() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  showNoMetal();

  diverter.attach(diverterPin);
  lid.attach(lidPin);
  diverter.write(homeAngle);
  lid.write(homeAngle);

  Serial.begin(9600);
  Serial.println(F("metal detector ready"));
  Serial.print(F("metal above "));
  Serial.print(sensorThreshold + hysteresis);
  Serial.print(F(", no-metal below "));
  Serial.println(sensorThreshold - hysteresis);
}

void loop() {
  const int reading = analogRead(metalSensorPin);
  const int metalOn = sensorThreshold + hysteresis;
  const int metalOff = sensorThreshold - hysteresis;

  if (!metalLatched && reading > metalOn) {
    latchMetal(reading);
  } else if (metalLatched && reading < metalOff) {
    releaseMetal(reading);
  } else if (metalLatched) {
    reportReading(F("hold"), reading);
  } else {
    reportReading(F("idle"), reading);
  }

  delay(sampleIntervalMs);
}
