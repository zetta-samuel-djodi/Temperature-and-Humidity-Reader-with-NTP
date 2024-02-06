#include <DHT22.h>
#include <WiFi.h>
#include "time.h"
//define pin data
#define pinDATA SDA // SDA, or D21 pin for DOIT Esp32 DevKit v1
DHT22 dht22(pinDATA); 
// wifi_setup
const char* ssid = "ZettaHouse";
const char* password = "Technoedge";
//ntp_setup
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 8;
const int   daylightOffset_sec = 0;

void wifi_setup(){
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

void dht22sense(){
  float t = dht22.getTemperature();
  float h = dht22.getHumidity();

  if (dht22.getLastError() != dht22.OK) { //error checking
    Serial.print("last error :");
    Serial.println(dht22.getLastError());
    Serial.println(dht22.debug()); //optionnal
  }

  Serial.print("humidity = ");Serial.print(h,1);Serial.print("\t");
  Serial.print("temperature = ");Serial.println(t,1);
  delay(2000); //Collecting period should be : >1.7 second
}

void printLocalTime() {
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  // February 05 2024 at 10:00 Temperature is 36 Celcius and Humidity is 75 %
  Serial.print("On ");
  Serial.print(&timeinfo, "%B %d %Y");
  Serial.print(" at ");
  Serial.println(&timeinfo, "%H:%M");
}

void setup() {
  Serial.begin(115200); //1bit=10µs
  wifi_setup();
}

void loop() {
  printLocalTime();
  dht22sense();
  Serial.println();
}