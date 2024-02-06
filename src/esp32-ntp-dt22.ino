#include <WiFi.h>
#include "time.h"

const char* ssid = "ZettaHouse";
const char* password = "Technoedge";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 8;
const int   daylightOffset_sec = 0;

float generateRandomFloatNumber(float min, float max);
void printLocalTime(void);

void setup() {
  Serial.begin(115200);
  srand(ESP.getEfuseMac());

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
  // Memperbarui waktu dari server NTP setiap detik
  printLocalTime();
  delay(1000);
}

float generateRandomFloatNumber(float max) {
  // return min + ((float)rand() / RAND_MAX) * (max - min);
  return ((float)rand()/(float)(RAND_MAX)) * max;
}

void printLocalTime() {
  struct tm timeinfo;

  float temperature = generateRandomFloatNumber(50.0);
  float humidity = generateRandomFloatNumber(99.0);

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  // February 05 2024 at 10:00 Temperature is 36 Celcius and Humidity is 75 %
  Serial.print("On ");
  Serial.print(&timeinfo, "%B %d %Y");
  Serial.print(" at ");
  Serial.print(&timeinfo, "%H:%M");
  Serial.printf(" Temperature is %.2f and Humidity is %.2f%%\n", temperature, humidity);
}