#include <Arduino.h>

#include "HTU21D.h"
#include "LEDControl.h"

#define HEATED_WICK_PIN 5
#define BUILTIN_LED_PIN 13
#define HUMIDITY_HIGH_THRESHOLD 99
#define HUMIDITY_LOW_THRESHOLD 90
#define RUN_LOOP_DELAY 1000

bool runBool = false;
bool panic = false;
long lastRunMillis = 0;
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

  if (millis() >= lastRunMillis + RUN_LOOP_DELAY) {
    lastRunMillis = millis();
    runBool = true;
  }
  if (panic == true) {
    LED.doubleBlink();
  }

  if (runBool == true) {
    Serial.print(F("Temperature °C: \t\t"));
    Serial.println(temperature);
    Serial.print(F("Humidity %: \t\t\t"));
    Serial.println(humidity);

    if (humidity >= HUMIDITY_HIGH_THRESHOLD) {
      Serial.println(F("Humidity reading too high, sensor heater on."));
      htu21d.setHeater(HTU21D_ON);
      digitalWrite(HEATED_WICK_PIN, LOW);
      panic = true;
    } else if (humidity <= HUMIDITY_LOW_THRESHOLD) {
      panic = false;
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      digitalWrite(HEATED_WICK_PIN, HIGH);
      Serial.println(F("Heated Wick on"));
      htu21d.setHeater(HTU21D_OFF);
      Serial.println(F("Humidity reading acceptable, sensor heater off."));
      LED.steadyRed();
    } else {
      panic = false;
      digitalWrite(HEATED_WICK_PIN, LOW);
      digitalWrite(BUILTIN_LED_PIN, LOW);
      LED.steadyGreen();
      Serial.println(F("Heated wick off"));
    }
    Serial.println();
    runBool = false;
  } else {
    ;
  }
}