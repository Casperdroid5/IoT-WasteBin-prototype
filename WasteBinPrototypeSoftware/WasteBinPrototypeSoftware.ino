#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_SHT4x.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
// Define the data pin and number of LEDs
#define DATA_PIN 8
#define NUM_LEDS 1

Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);


// WiFi settings
const char* SSID = "WWegvanons3";  // avoid using guest network!
const char* PASSWORD = "JuCasSan27@#";

// MQTT Broker settings
const char* MQTT_SERVER = "192.168.111.237";  // replace with your broker url
const char* MQTT_USERNAME = "mqtt";
const char* MQTT_PASSWORD = "WasteBin5#";
const int MQTT_PORT = 1883;


// MQTT topics
const char* SENSOR_DATA_TOPIC = "/data/sensors/";


// Device settings
const char* CLIENT_NAME = "ESP32WasteBin";
const uint8_t peltierandfanpin = 3;
const long interval = 20000;   // peltier and fan interval
int peltierandfanstate = LOW;  // ledState used to set the LED

unsigned long previousMillis = 0;  // will store last time LED was updated

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
    Serial.print("trying to connect... ");
  }

  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());

  // Peltier + fan pin
  pinMode(peltierandfanpin, OUTPUT);

  strip.begin();
  strip.show();  // Initialize all pixels to 'off'

  // SHT4x sensor setup
  Serial.println("Adafruit SHT4x test");
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    delay(1000);
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

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (peltierandfanstate == LOW) {
      peltierandfanstate = HIGH;
      // Set the color of the LED
      strip.setPixelColor(0, strip.Color(0, 0, 255));
      // Show the updated LED
      strip.show();
    } else {
      peltierandfanstate = LOW;
      // Set the color of the LED to a different color
      strip.setPixelColor(0, strip.Color(255, 0, 0));
      // Show the updated LED
      strip.show();
    }

    // set the LED with the ledState of the variable:
    digitalWrite(peltierandfanpin, peltierandfanstate);
  }

  // Publish sensor data to MQTT broker
  if (mqtt_client.connected()) {
    if (millis() - last_msg_sent > msg_interval) {
      publish_message(SENSOR_DATA_TOPIC, temperature_value, humidity_value, lid_position);
      last_msg_sent = millis();
    }
  } else {
    // Attempt to reconnect to MQTT broker
    if (!mqtt_client.connect(CLIENT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
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

void publish_message(const char* topic, const float temperature, const float humidity, const bool lid_position) {
  // Create JSON object
  auto macAdress = WiFi.macAddress();
  StaticJsonDocument<200> jsonDoc;

  jsonDoc["macAdress"] = macAdress;
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["lid_position"] = lid_position;

  // Serialize JSON object to string
  String message;
  serializeJson(jsonDoc, message);

  // Publish message to MQTT broker
  if (mqtt_client.publish(topic, message.c_str())) {
    Serial.println("Message published [" + String(topic) + "]: " + message);
  } else {
    Serial.println("Failed to publish message [" + String(topic) + "]: " + message);
  }
}
