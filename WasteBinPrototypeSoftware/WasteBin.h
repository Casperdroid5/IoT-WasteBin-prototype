#ifndef WASTEBIN_H
#define WASTEBIN_H

#include <WiFi.h>
#include <PubSubClient.h>

// WiFi settings
extern const char* ssid;
extern const char* password;

// MQTT Broker settings
extern const char* mqtt_server;
extern const char* mqtt_username;
extern const char* mqtt_password;
extern const int mqtt_port;

// MQTT topics
extern const char* temperature_sensor_topic;
extern const char* humidity_sensor_topic;
extern const char* lid_sensor_topic;

// PubSubClient setup
extern WiFiClient wifi_client;
extern PubSubClient mqtt_client;
extern unsigned long last_msg_sent;
extern const unsigned long msg_interval;
extern char msg_buffer[50];

void setup();
void loop();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);
void publish_message(const char* topic, const char* payload);

#endif