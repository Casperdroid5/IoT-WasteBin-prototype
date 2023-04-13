#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

const int buttonPin = 2;  // the number of the pushbutton pin
int buttonState = 0;      // variable for reading the pushbutton status
bool litd = false;        // boolean for lit position

// Update these with values suitable for your network.
const char* ssid = "WWegvanons3";
const char* password = "JuCasSan27@#";
const char* mqtt_server = "test.mosquitto.org";

#define mqtt_port 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_SERIAL_PUBLISH_CH "ESEiot/test/"
#define MQTT_SERIAL_RECEIVER_CH "ESEiot/test/"  //required

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
//i2c pins are gpio 22 SCL, gpio 21SDA on esp32 devkitwroom

WiFiClient wifiClient;

PubSubClient client(wifiClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe(MQTT_SERIAL_RECEIVER_CH);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("-------new message from broker-----");
  Serial.print("channel:");
  Serial.println(topic);
  Serial.print("data:");
  Serial.write(payload, length);
  Serial.println();
}

void setup() {

  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Adafruit SHT4x test");
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }
  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);

  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  Serial.setTimeout(500);  // Set time out for wifi
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void loop() {

  sensors_event_t humidity, temp;
  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);  // populate temp and humidity objects with fresh data
  timestamp = millis() - timestamp;

  float humidityValue = humidity.temperature;
  float tempValue = temp.temperature;


  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");
  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");

  Serial.print("Read duration (ms): ");
  Serial.println(timestamp);

  Serial.print("Litposition = ");
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    litd = false;  // lid is opened
  } else {
    litd = true;  // lid is closed
  }

  Serial.println(litd);


  delay(1500);  // to stop the data flooding

  client.loop();
  char buffer[500];
  sprintf(buffer, "tempd = %lf, presd = %lf, litd = %s", tempValue, humidityValue, litd ? "true" : "false");


  client.publish(MQTT_SERIAL_PUBLISH_CH, buffer);
}
