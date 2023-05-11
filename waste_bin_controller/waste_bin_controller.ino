#include "waste_bin_controller.h"

// WiFi settings
const char* SSID = "WWegvanons3";       ///< SSID of the WiFi network
const char* PASSWORD = "JuCasSan27@#";  ///< Password for the WiFi network

// MQTT Broker settings
const char* MQTT_SERVER = "192.168.111.237";  ///< MQTT broker server IP address
const char* MQTT_USERNAME = "mqtt";           ///< MQTT broker username
const char* MQTT_PASSWORD = "WasteBin5#";     ///< MQTT broker password
const int MQTT_PORT = 1883;                   ///< MQTT broker port number

const char* CLIENT_NAME = "ESP32WasteBin";  ///< Name of the MQTT client

// MQTT topics
const char* SENSOR_DATA_TOPIC = "/data/sensors/";  ///< Topic for sensor data

// Device settings
const uint8_t peltierandfanpin = 3;  ///< Pin for Peltier module and fan
const long interval = 20000;         ///< Interval for Peltier and fan control
int peltierandfanstate = LOW;        ///< Current state of the Peltier module and fan

// PubSubClient setup
WiFiClient wifi_client;                 ///< WiFi client for MQTT connection
PubSubClient mqtt_client(wifi_client);  ///< MQTT client

// Other variables
unsigned long last_msg_sent = 0;           ///< Time when the last MQTT message was sent
const unsigned long msg_interval = 15000;  ///< Interval for sending MQTT messages
unsigned long previousMillis = 0;          ///< Previous timestamp for interval calculations

const int setPoint = 15;  ///< Temperature set point in Celsius
const int tolerance = 2;  ///< Temperature tolerance in Celsius
const int pwmFreq = 200;  ///< PWM frequency in Hz

// Sensor pin
const int BUTTON_PIN = 4;  ///< Pin for the lid position sensor

// Sensor values
bool lid_position = false;      ///< Current state of the lid position
float humidity_value = 0.0;     ///< Current humidity value
float temperature_value = 0.0;  ///< Current temperature value

// SHT4x sensor setup
Adafruit_SHT4x sht4 = Adafruit_SHT4x();  ///< SHT4x sensor object

/**
 * @brief Set up the WiFi connection.
 */
void setupWiFi() {
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
}

/**
 * @brief Set up the Peltier module and fan pin.
 */
void setupPeltierAndFan() {
  // Peltier + fan pin
  pinMode(peltierandfanpin, OUTPUT);
  analogWriteFrequency(pwmFreq);
}

/**
 * @brief Set up the SHT4x sensor.
 */
void setupSHT4x() {
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
}

/**
 * @brief Set up the MQTT client.
 */
void setupMQTTClient() {
  // MQTT client setup
  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt_client.setCallback(handleIncomingMessage);
}

/**
 * @brief Read sensor data from the SHT4x sensor.
 */
void readSensorData() {
  // Read sensor data
  sensors_event_t humidity, temp;
  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);
  timestamp = millis() - timestamp;

  // Update sensor values
  humidity_value = humidity.relative_humidity;
  temperature_value = temp.temperature;
}

/**
 * @brief Update the current lid position based on the button state.
   @brief Whenever the lid is opened longer than threshold, sound will be played trough piezo and led will light up.

 */
const int led_pin = 1;       // Pin for the LED
const int piezo_pin = 5;     // Pin for the piezo speaker

unsigned long lidOpenStartTime = 0;  // Variable to store the lid open start time
bool isBeeping = false;               // Flag to track if the piezo speaker is beeping

void updateLidPosition() {
  // Get lid position
  bool button_state = digitalRead(BUTTON_PIN);
  if (button_state == LOW) {
    lid_position = true;  // lid is closed
    lidOpenStartTime = millis();  // Reset lid open start time
    digitalWrite(led_pin, LOW);   // Turn off the LED

    // Stop the piezo speaker from beeping
    if (isBeeping) {
      noTone(piezo_pin);
      isBeeping = false;
    }
  } else {
    lid_position = false;  // lid is opened

    // Check if the lid has been opened for longer than 10 seconds
    if (millis() - lidOpenStartTime >= 10000) {
      // Blink the LED every 2 seconds
      if ((millis() / 2000) % 2 == 0) {
        digitalWrite(led_pin, HIGH);  // Turn on the LED

        // Start the piezo speaker beeping if it's not already beeping
        if (!isBeeping) {
          tone(piezo_pin, 500, 750);  // Beep for 500ms at 750Hz
          isBeeping = true;
        }
      } else {
        digitalWrite(led_pin, LOW);   // Turn off the LED

        // Stop the piezo speaker from beeping
        if (isBeeping) {
          noTone(piezo_pin);
          isBeeping = false;
        }
      }
    } else {
      digitalWrite(led_pin, LOW);   // Turn off the LED

      // Stop the piezo speaker from beeping
      if (isBeeping) {
        noTone(piezo_pin);
        isBeeping = false;
      }
    }
  }
}


/**
 * @brief Control the Peltier module based on the temperature error.
 */
void controlPeltierModule() {
  float error = setPoint - temperature_value;  // Calculate error
  Serial.println(temperature_value);
  Serial.println(error);

  if (error > tolerance) {
    // If temperature is too HIGH, turn off Peltier module
    analogWrite(peltierandfanpin, 0);  // Set maximum duty cycle
  } else if (error < -tolerance) {
    // If temperature is too LOW, turn on Peltier module
    analogWrite(peltierandfanpin, 255);  // Set minimum duty cycle
  }
  // Otherwise, maintain current state of Peltier module
}

/**
 * @brief Publish sensor data to the MQTT broker.
 */
void publishSensorData() {
  // Publish sensor data to MQTT broker
  if (mqtt_client.connected()) {
    if (millis() - last_msg_sent > msg_interval) {
      publishMessage(SENSOR_DATA_TOPIC, temperature_value, humidity_value, lid_position);
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

/**
 * @brief Handle incoming MQTT messages.
 * @param topic The topic of the incoming message.
 * @param payload The payload of the incoming message.
 * @param length The length of the payload.
 */
void handleIncomingMessage(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages
  String incoming_message = "";
  for (int i = 0; i < length; i++) {
    incoming_message += (char)payload[i];
  }
  Serial.println("Message arrived [" + String(topic) + "]: " + incoming_message);
}

/**
 * @brief Publish a message to the MQTT broker.
 * @param topic The topic to publish the message to.
 * @param temperature The temperature value to include in the message.
 * @param humidity The humidity value to include in the message.
 * @param lid_position The lid position value to include in the message.
 */
void publishMessage(const char* topic, const float temperature, const float humidity, const bool lid_position) {
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

/**
 * @brief Perform the initial setup of the system.
 */
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

  setupPeltierAndFan();
  setupSHT4x();
  setupMQTTClient();
}

/**
 * @brief The main program loop.
 */
void loop() {
  readSensorData();
  updateLidPosition();
  controlPeltierModule();
  publishSensorData();
}
