#include <Arduino.h>

#include "HTU21D.h"

#define HEATED_WICK_PIN 5
#define BUILTIN_LED_PIN 13
#define RED_LED_PIN 4
#define GREEN_LED_PIN 6
#define HUMIDITY_THRESHOLD 90

HTU21D htu21d;
float temperature, humidity = 0;

void setupHTU21D() {
  while (htu21d.begin() != true) {
    Serial.println("HTU21D failed or not connected");
    delay(1000);
  }
  Serial.println("HTU21D active");
  Serial.println();
}

void setupPins() {
  pinMode(HEATED_WICK_PIN, OUTPUT);
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(HEATED_WICK_PIN, LOW);
  digitalWrite(BUILTIN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  Serial.println("Pins setup complete");
}

void panicBlink() {
  digitalWrite(RED_LED_PIN, HIGH);
  delay(250);
  digitalWrite(RED_LED_PIN, LOW);
  delay(250);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(500);
  digitalWrite(RED_LED_PIN, LOW);
  delay(500);
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  setupHTU21D();
  setupPins();
}

void loop() {
  humidity = htu21d.readHumidity();
  temperature = htu21d.readTemperature();

  Serial.print(F("Temperature °C: \t\t"));
  Serial.println(temperature);
  Serial.print(F("Humidity %: \t\t\t"));
  Serial.println(humidity);

  if (humidity <= HUMIDITY_THRESHOLD) {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    digitalWrite(HEATED_WICK_PIN, HIGH);
    Serial.println(F("Heated Wick on"));
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
  } else {
    digitalWrite(HEATED_WICK_PIN, LOW);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    Serial.println(F("Heated wick off"));
  }

  delay(1000);
  Serial.println();
}