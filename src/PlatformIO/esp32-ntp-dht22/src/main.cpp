#include <Arduino.h>
#include <Adafruit_Sensor.h>

#include <WiFi.h>
#include "time.h"
#include <DHT.h>

#define pinDATA SDA

const char* ssid = "ZettaHouse";
const char* password = "Technoedge";

const char* ntpServer = "pool.ntp.org"; // NTP server
const long  gmtOffset_sec = 3600 * 8; // Set Timezone to WITA (GMT +8)
const int   daylightOffset_sec = 0; // No Daylight Saving

DHT dht22(pinDATA, DHT22); 

void printLocalTime(void);
void senseDHT22(void);

void setup() {
  Serial.begin(115200);
  dht22.begin();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi connected");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("Initial Time : ");
  printLocalTime();
  Serial.println("");

  // //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
  // Expected Output : On February 05 2024 at 10:00 Temperature is 36 Celcius and Humidity is 75 %
  senseDHT22();
  delay(1000);
}

void printLocalTime() {
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  // Output full time information :
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  Serial.print("On ");
  Serial.print(&timeinfo, "%B %d %Y");
  Serial.print(" at ");
  Serial.print(&timeinfo, "%H:%M");
}

void senseDHT22() {
  float temperature = dht22.readTemperature();
  float humidity = dht22.readHumidity();

  // If error, debug and get error log
  if (!isnan(temperature) && !isnan(humidity)) {
    printLocalTime();
    Serial.printf(" Temperature: %.2f Celcius and Humidity is %.2f%%\n", temperature, humidity);
  } else {
    printLocalTime();
    Serial.println("Failed to read from DHT sensor!");
  }
}