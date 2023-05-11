#ifndef WASTE_BIN_CONTROLLER_H
#define WASTE_BIN_CONTROLLER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_SHT4x.h>
#include <ArduinoJson.h>

// WiFi settings
extern const char* SSID;
extern const char* PASSWORD;

// MQTT Broker settings
extern const char* MQTT_SERVER;
extern const char* MQTT_USERNAME;
extern const char* MQTT_PASSWORD;
extern const int MQTT_PORT;

extern const char* CLIENT_NAME;

// MQTT topics
extern const char* SENSOR_DATA_TOPIC;

// Device settings
extern const uint8_t peltierandfanpin;
extern const long interval;
extern int peltierandfanstate;

// PubSubClient setup
extern WiFiClient wifi_client;
extern PubSubClient mqtt_client;

// Other variables
extern unsigned long last_msg_sent;
extern const unsigned long msg_interval;
extern unsigned long previousMillis;

extern const int setPoint;
extern const int tolerance;
extern const int pwmFreq;

// Sensor pin
extern const int BUTTON_PIN;

// Sensor values
extern bool lid_position;
extern float humidity_value;
extern float temperature_value;

// SHT4x sensor setup
extern Adafruit_SHT4x sht4;

void setupWiFi();
void setupPeltierAndFan();
void setupSHT4x();
void setupMQTTClient();

void readSensorData();
void updateLidPosition();
void controlPeltierModule();
void publishSensorData();
void handleIncomingMessage(char* topic, byte* payload, unsigned int length);
void publishMessage(const char* topic, const float temperature, const float humidity, const bool lid_position);

void setup();
void loop();

#endif  // WASTE_BIN_CONTROLLER_H
