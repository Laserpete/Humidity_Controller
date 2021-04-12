#include "LEDControl.h"

#include <Arduino.h>
bool alternateBool = false;

void LEDControl::setupLEDs(void) {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void LEDControl::doubleBlink(void) {
  long lastMillis = 0;

  if (millis() >= lastMillis + DOUBLE_BLINK_DELAY_TIME) {
    lastMillis = millis();
    alternateBool = !alternateBool;
    Serial.println(alternateBool);
  }
  if (alternateBool == true) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
  } else if (alternateBool == false) {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
  }
}
void LEDControl::panic(void) {
  // Red on, green blinks

  digitalWrite(RED_LED_PIN, HIGH);
  delay(250);
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(250);
  digitalWrite(GREEN_LED_PIN, LOW);
  delay(250);
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(250);
  digitalWrite(GREEN_LED_PIN, LOW);
  delay(250);
  digitalWrite(RED_LED_PIN, LOW);

  // Green on, red blinks
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(250);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(250);
  digitalWrite(RED_LED_PIN, LOW);
  delay(250);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(250);
  digitalWrite(RED_LED_PIN, LOW);
  delay(250);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void LEDControl::steadyRed() {
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
}
void LEDControl::steadyGreen() {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
}
