#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_SHT4x.h>

// WiFi settings
const char* SSID = "WWegvanons3";  // avoid using guest network!
const char* PASSWORD = "JuCasSan27@#";

// MQTT Broker settings
const char* MQTT_SERVER = "192.168.111.237";  // replace with your broker url
const char* MQTT_USERNAME = "mqtt";
const char* MQTT_PASSWORD = "WasteBin5#";
const int MQTT_PORT = 1883;

// MQTT topics
const char* TEMPERATURE_SENSOR_TOPIC = "WasteBin/Temp/";
const char* HUMIDITY_SENSOR_TOPIC = "WasteBin/Humid/";
const char* LID_SENSOR_TOPIC = "WasteBin/LidP/";

// PubSubClient setup
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

// Other variables
unsigned long last_msg_sent = 0;
const unsigned long msg_interval = 2000;  // send message every 2 seconds

// Sensor pin
const int BUTTON_PIN = 4;

// Sensor values
bool lid_position = false;
float humidity_value = 0.0;
float temperature_value = 0.0;

// SHT4x sensor setup
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  // Wait for WiFi connection
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

  // SHT4x sensor setup
  Serial.println("Adafruit SHT4x test");
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (1);
  }
  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);

  // Initialize the pushbutton pin as an input
  pinMode(BUTTON_PIN, INPUT);

  // MQTT client setup
  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt_client.setCallback(callback);
}

void loop() {
  // Read sensor data
  sensors_event_t humidity, temp;
  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);
  timestamp = millis() - timestamp;

  // Update sensor values
  humidity_value = humidity.relative_humidity;
  temperature_value = temp.temperature;

  // Get lid position
  bool button_state = digitalRead(BUTTON_PIN);
  if (button_state == LOW) {
    lid_position = true;  // lid is closed
  } else {
    lid_position = false;  // lid is opened
  }

  // Publish sensor data to MQTT broker
  if (mqtt_client.connected()) {
    if (millis() - last_msg_sent > msg_interval) {
      publish_message(TEMPERATURE_SENSOR_TOPIC, temperature_value);
      publish_message(HUMIDITY_SENSOR_TOPIC, humidity_value);
      publish_message(LID_SENSOR_TOPIC, lid_position);
      last_msg_sent = millis();
    }
  } else {
    // Attempt to reconnect to MQTT broker
    if (!mqtt_client.connect("ESP32WasteBin1", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Failed to connect to MQTT broker");
    }
  }

  // Maintain MQTT connection
  mqtt_client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages
  String incoming_message = "";
  for (int i = 0; i < length; i++) {
    incoming_message += (char)payload[i];
  }
  Serial.println("Message arrived [" + String(topic) + "]: " + incoming_message);
}

void publish_message(const char* topic, const float payload) {
  // Publish sensor data to MQTT broker
  char msg_buffer[20];
  snprintf(msg_buffer, sizeof(msg_buffer), "%.2f", payload);
  if (mqtt_client.publish(topic, msg_buffer, true)) {
    Serial.println("Message published [" + String(topic) + "]: " + String(msg_buffer));
  } else {
    Serial.println("Failed to publish message [" + String(topic) + "]: " + String(msg_buffer));
  }
}