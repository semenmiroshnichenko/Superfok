#include <Arduino.h>
#include <ESP32Encoder.h>

const int encoderPin1 = 2;
const int encoderPin2 = 15;

const int directionPin = 32;
const int stepPin      = 33;
const int sleepPin     = 25;
const int resetPin     = 26;
const int stepModeMS2  = 14;
const int stepModeMS1  = 12;
const int enablePin    = 13;

ESP32Encoder encoder;
long lastEncoderPosition = 0;
unsigned long lastMovementTimestamp;

void setup() {
  delay(1);
  // Enable the weak pull down resistors
  ESP32Encoder::useInternalWeakPullResistors=true;
  // set starting count value
  encoder.clearCount();
  // Attach pins for use as encoder pins
  encoder.attachSingleEdge(encoderPin1, encoderPin2);
  
  // setup motor
  pinMode(enablePin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(stepModeMS1, OUTPUT);
  pinMode(stepModeMS2, OUTPUT);

  digitalWrite(enablePin, HIGH); //deactivate driver (LOW active)
  digitalWrite(directionPin, LOW); //LOW or HIGH
  digitalWrite(stepPin, LOW);
  digitalWrite(enablePin, LOW); //activate driver
  // set 8 microsteps
  digitalWrite(stepModeMS1, LOW);
  digitalWrite(stepModeMS2, HIGH);

  lastMovementTimestamp = millis();
  // serial debug
  Serial.begin(9600, SERIAL_8N1);
}

void loop() {
  long encoderPosition = encoder.getCount();
  if(encoderPosition == lastEncoderPosition)
  {
    long timeSinceLastMovementInMs = millis() - lastMovementTimestamp;
    if(timeSinceLastMovementInMs > 1000)
    {
      digitalWrite(enablePin, HIGH);
    }
    return;
  }
  digitalWrite(enablePin, LOW); //activate driver
  
  lastMovementTimestamp = millis();
  if(encoderPosition > lastEncoderPosition)
  {
    digitalWrite(directionPin, LOW);
  }
  else
  {
    digitalWrite(directionPin, HIGH);
  }
  delay(1);
  long stepCount = abs(encoderPosition - lastEncoderPosition);
  lastEncoderPosition = encoderPosition;
  for(int i = 0; i < stepCount * log(stepCount); i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(50);
  }
}