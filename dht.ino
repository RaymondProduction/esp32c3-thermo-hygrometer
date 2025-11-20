#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("ESP32-C3 DHT22 Thermo-Hygrometer start");

  // I2C for ESP32-C3 Super Mini: SDA = 8, SCL = 9
  Wire.begin(8, 9);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // address 0x3CC
    Serial.println("SSD1306 allocation failed");
    for (;;); // halt here if display not found
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Hello Raymond. Booting...");
  display.display();

  // Initialize DHT
  dht.begin();
  Serial.println("DHT22 init done");

  delay(1000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();  // °C

  if (isnan(h) || isnan(t)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("DHT22 ERROR");
    display.display();
    delay(1000);
    return;
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("DHT22 Sensor");

  // Temperature in large font
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.print(t, 1);

  display.cp437(true);       // code 248 is symbol °
  display.write(248);
  display.println("C");

  // Humidity
  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print(h, 1);
  display.println("%");

  display.display();

  delay(2000);
}
