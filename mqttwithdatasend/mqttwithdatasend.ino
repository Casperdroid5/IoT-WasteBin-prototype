#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <ArduinoJson.h>

const int buttonPin = 2;  // the number of the pushbutton pin
int buttonState = 0;      // variable for reading the pushbutton status
bool litd = false;        // boolean for lit position

// Update these with values suitable for your network.
const char* ssid = "WWegvanons3-guest";
const char* password = "super3333";
const char* mqtt_server = "broker.hivemq.com";

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
#define SEALEVELPRESSURE_HPA (1013.25)

StaticJsonDocument<200> doc;
Adafruit_BMP3XX bmp;
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
  Serial.println("Adafruit BMP388 / BMP390 test");

  if (!bmp.begin_I2C()) {  // hardware I2C mode, can pass in address & alt Wire
                           //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode
                           //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1)
      ;
  }
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  Serial.setTimeout(500);  // Set time out for
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void loop() {
  if (!bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  Serial.print("Temperature = ");
  Serial.print(bmp.temperature);
  Serial.println(" *C");

  Serial.print("Presure = ");
  Serial.print(bmp.pressure);
  Serial.println("hPa");

  Serial.print("Litposition = ");
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    litd = false;  // lid is opened
  } else {
    litd = true;  // lid is closed
  }

  Serial.println(litd);

  float tempd = bmp.readTemperature();
  float presd = bmp.readPressure();

  delay(1500);  // to stop the data flooding

  client.loop();
  char buffer[500];
  sprintf(buffer, "tempd = %lf, presd = %lf, litd = %s", tempd, presd, litd ? "true" : "false");


  client.publish(MQTT_SERIAL_PUBLISH_CH, buffer);
}
