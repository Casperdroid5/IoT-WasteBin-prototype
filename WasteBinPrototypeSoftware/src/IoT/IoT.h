#ifndef IOT_H
#define IOT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

//---- WiFi settings
const char* ssid = "WWegvanons3";  // don't use guest network!
const char* password = "";
//---- MQTT Broker settings
const char* mqtt_server = "192.168.111.237";  // replace with your broker url
const char* mqtt_username = "mqtt";
const char* mqtt_password = "WasteBin5#";
const int mqtt_port = 1883;

#endif