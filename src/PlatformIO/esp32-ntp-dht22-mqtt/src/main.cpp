#include <PubSubClient.h>
#include "WiFiClientSecure.h"
#include "credentials.h"

WiFiClientSecure client;
PubSubClient mqtt_client(client);  

void setup() {
  Serial.begin(115200);
  delay(100); 

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

  //Set up the certificates and keys
  client.setCACert(CA_cert);          //Root CA certificate
  client.setCertificate(ESP_CA_cert); //for client verification if the require_certificate is set to true in the mosquitto broker config
  client.setPrivateKey(ESP_RSA_key);  //for client verification if the require_certificate is set to true in the mosquitto broker config

  mqtt_client.setServer(mqtt_server, port);
}

void loop() {  
  Serial.println("\nStarting connection to server...");
  //if you use password for Mosquitto broker
  //if (client.connect("ESP32", mqtt_user , mqtt_pass)) {
  //if you dont use password for Mosquitto broker
  if (mqtt_client.connect("ESP32")) {                       
    Serial.print("Connected, mqtt_client state: ");
    Serial.println(mqtt_client.state());
    //Publsih a demo message to topic LivingRoom/TEMPERATURE with a value of 25
    mqtt_client.publish("LivingRoom/TEMPERATURE", "25");
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