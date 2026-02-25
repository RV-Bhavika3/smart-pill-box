#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// RTC object
RTC_DS3231 rtc;

// LED and Buzzer pins
const int ledPin = 3;
const int buzzerPin = 2;

// Alarm time (24-hour format)
const int alarmHour = 16;
const int alarmMinute = 10;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // RTC setup
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.print("Smart Pill Box Ready");
  display.display();
  delay(2000);
}

void loop() {
  DateTime now = rtc.now();

  // Show Date and Time on OLED
  display.clearDisplay();

  // Date
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Date: ");
  display.print(now.day());
  display.print('/');
  display.print(now.month());
  display.print('/');
  display.print(now.year());

  // Time
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(now.hour());
  display.print(':');
  if (now.minute() < 10) display.print('0');
  display.print(now.minute());
  display.print(':');
  if (now.second() < 10) display.print('0');
  display.print(now.second());

  // Alarm check
  if (now.hour() == alarmHour && now.minute() == alarmMinute) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);

    // Show alert
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print(">> Take Your Pill! <<");
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  display.display();
  delay(1000);
}
