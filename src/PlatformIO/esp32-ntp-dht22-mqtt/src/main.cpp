#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include "time.h"
#include <DHT.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "WiFiClientSecure.h"
#include "credentials.h"

#define pinDATA SDA

const char* ntpServer = "pool.ntp.org"; // NTP server
const long  gmtOffset_sec = 3600 * 8; // Set Timezone to WITA (GMT +8)
const int   daylightOffset_sec = 0;

typedef struct DHT_Value {
  float temperature;
  float humidity;
} DHT_value;


WiFiClientSecure client;
PubSubClient mqtt_client(client);

// StaticJsonDocument<32> doc; -> deprecated format

const size_t capacity = 32;
DynamicJsonDocument doc(capacity);

DHT dht22(pinDATA, DHT22); 

String getCurrentTime(void);
DHT_Value senseDHT22(void);
void publishSensorData(float temperature, float humidity);

void setup() {
  Serial.begin(115200);
  dht22.begin();

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("Initial Time : ");
  Serial.print(getCurrentTime());
  Serial.println("");

  //Set up the certificates and keys
  client.setCACert(CA_cert);          //Root CA certificate
  client.setCertificate(ESP_CA_cert); //for client verification if the require_certificate is set to true in the mosquitto broker config
  client.setPrivateKey(ESP_RSA_key);  //for client verification if the require_certificate is set to true in the mosquitto broker config

  mqtt_client.setServer(mqtt_server, port);
}

void loop() { 

  DHT_Value dhtValue = senseDHT22(); 
  Serial.println("\nStarting connection to server...");
  if (mqtt_client.connect("ESP32")) {                       
    Serial.print("Connected, mqtt_client state: ");
    Serial.println(mqtt_client.state());

    //Publsih a demo message to topic LivingRoom/TEMPERATURE with a value of 25
    mqtt_client.publish("ZettaHouse/TemperatureAndHumidity", "25");

    if (!isnan(dhtValue.temperature) && !isnan(dhtValue.humidity)) {
    Serial.print(getCurrentTime());
    Serial.printf(" Temperature: %.2f Celcius and Humidity is %.2f%%\n", dhtValue.temperature, dhtValue.humidity);
    publishSensorData(dhtValue.temperature,dhtValue.humidity);
    }
  }
  else {
    Serial.println("Connected failed!  client state:");
    Serial.print(mqtt_client.state());
    Serial.println("WiFiClientSecure client state:");
    char lastError[100];
    client.lastError(lastError,100);  //Get the last error for WiFiClientSecure
    Serial.print(lastError);
  }
  delay(10000);
}

// void printLocalTime() {
//   struct tm timeinfo;

  // if(!getLocalTime(&timeinfo)){
  //   Serial.println("Failed to obtain time");
  //   return;
  // }
  
//   // Output full time information :
//   // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

//   Serial.print("On ");
//   Serial.print(&timeinfo, "%B %d %Y");
//   Serial.print(" at ");
//   Serial.print(&timeinfo, "%H:%M");
// }

String getCurrentTime(void) {
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }

  char currentTime[30];
  strftime(currentTime, sizeof(currentTime), "%B %d %Y - %H:%M", &timeinfo);

  return String(currentTime);
}

DHT_Value senseDHT22() {
  DHT_Value dht_value;

  dht_value.temperature = dht22.readTemperature();
  dht_value.humidity = dht22.readHumidity();

  if (!isnan(dht_value.temperature) && !isnan(dht_value.humidity)) {
    return dht_value;
  } else {
    Serial.println("Failed to read from DHT sensor!");
    dht_value.temperature = NAN;
    dht_value.humidity = NAN;
    return dht_value;
  }
}

void publishSensorData(float temperature, float humidity) {
  doc["time"] = getCurrentTime();
  doc["temperature"] = String(temperature, 2);
  doc["humidity"] = String(humidity, 2);
  String output;
  serializeJson(doc, output);
  mqtt_client.publish("ZettaHouse/DHT22", output.c_str());
}