#include <Arduino.h>

#include "HTU21D.h"
#include "LEDControl.h"

#define HEATED_WICK_PIN 5
#define BUILTIN_LED_PIN 13

#define HUMIDITY_THRESHOLD 90

LEDControl LED;

HTU21D htu21d;
float temperature, humidity = 0;

void setupHTU21D() {
  while (htu21d.begin() != true) {
    Serial.println("HTU21D failed or not connected");
    delay(1000);
  }
  htu21d.softReset();
  htu21d.setResolution(HTU21D_RES_RH8_TEMP12);
  Serial.println("HTU21D active");
  Serial.println();
}

void setupPins() {
  pinMode(HEATED_WICK_PIN, OUTPUT);
  pinMode(BUILTIN_LED_PIN, OUTPUT);

  digitalWrite(HEATED_WICK_PIN, LOW);
  digitalWrite(BUILTIN_LED_PIN, LOW);

  Serial.println("Pins setup complete");
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  setupHTU21D();
  setupPins();
  LED.setupLEDs();
}

void loop() {
  humidity = htu21d.readCompensatedHumidity();
  temperature = htu21d.readTemperature();

  Serial.print(F("Temperature °C: \t\t"));
  Serial.println(temperature);
  Serial.print(F("Humidity %: \t\t\t"));
  Serial.println(humidity);

  if (humidity >= 99) {
    Serial.println(F("Humidity reading too high, sensor heater on"));
    htu21d.setHeater(HTU21D_ON);
    digitalWrite(HEATED_WICK_PIN, LOW);
    LED.panic();
    htu21d.softReset();
  } else if (humidity <= HUMIDITY_THRESHOLD) {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    digitalWrite(HEATED_WICK_PIN, HIGH);
    Serial.println(F("Heated Wick on"));
    LED.steadyRed();
  } else {
    htu21d.setHeater(HTU21D_OFF);
    digitalWrite(HEATED_WICK_PIN, LOW);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    LED.steadyGreen();
    Serial.println(F("Heated wick off"));
  }

  delay(1000);
  Serial.println();
}