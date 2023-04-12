#include "WasteBin.h"

// WiFi settings
const char* ssid = "WWegvanons3";  // avoid using guest network!
const char* password = "your_wifi_password_here";

// MQTT Broker settings
const char* mqtt_server = "192.168.111.237";  // replace with your broker url
const char* mqtt_username = "mqtt";
const char* mqtt_password = "WasteBin5#";
const int mqtt_port = 1883;

// MQTT topics
const char* temperature_sensor_topic = "/data/temperature";
const char* humidity_sensor_topic = "/data/humidity";
const char* lid_sensor_topic = "/data/lidposition";

// PubSubClient setup
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);
unsigned long last_msg_sent = 0;
const unsigned long msg_interval = 2000;  // send message every 2 seconds
char msg_buffer[50];

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());

  // Wait for serial to be ready
  while (!Serial) {
    delay(1);
  }

  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(callback);
}

void loop() {
  if (!mqtt_client.connected()) {
    reconnect();
  }

  mqtt_client.loop();

  if (millis() - last_msg_sent > msg_interval) {
    publish_message(temperature_sensor_topic, "temperaturesensor_message");
    publish_message(humidity_sensor_topic, "humiditysensor_message");
    publish_message(lid_sensor_topic, "lidsensor_message");
    last_msg_sent = millis();
  }
}

void reconnect() {
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String client_id = "ESP32WasteBin1-";
    client_id += String(random(0xFFFF), HEX);

    if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      // Subscribe to topics here
      mqtt_client.subscribe("/ESEiot/test/");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String incoming_message = "";
  for (int i = 0; i < length; i++) {
    incoming_message += (char)payload[i];
  }
  Serial.println("Message arrived [" + String(topic) + "]: " + incoming_message);

  // Check for other commands
  /*
  if (strcmp(topic, command2_topic) == 0) {
    if (incoming_message.equals("1")) {
      // Do something else
    }
  }
  */
}

void publish_message(const char* topic, const char* payload) {
  if (mqtt_client.publish(topic, payload, true)) {
    Serial.println("Message published [" + String(topic) + "]: " + String(payload));
  }
}