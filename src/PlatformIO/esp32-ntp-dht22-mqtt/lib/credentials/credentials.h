#ifndef CREDENTIALS_H
#define CREDENTIALS_H

// WiFi credentials
extern const char* ssid;
extern const char* password;

// MQTT Broker credentials
extern const char* mqtt_server;
extern const char* topic;
extern const int port;

// SSL certificates
extern const char* CA_cert;
extern const char* ESP_CA_cert;
extern const char* ESP_RSA_key;

#endif  // CREDENTIALS_H
