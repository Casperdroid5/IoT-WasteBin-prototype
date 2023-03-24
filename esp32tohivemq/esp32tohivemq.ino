#include <WiFi.h>
#include <PubSubClient.h>


//---- WiFi settings
const char* ssid = "WWegvanons3"; // don't use guest networks! (firewall issue)
const char* password = ""; 
//---- MQTT Broker settings
const char* mqtt_server = "192.168.111.237";  // replace with your broker url
const char* mqtt_username = "";
const char* mqtt_password = "";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

const char* sensor1_topic = "sensor1";
const char* sensor2_topic = "sensor2";


void setup() {

  Serial.begin(115200);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());

  while (!Serial) delay(1);

  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) reconnect();
  client.loop();


  publishMessage(sensor1_topic, String("sensor1_message"), true);
  publishMessage(sensor2_topic, String("sensor2_message"), true);
  delay(2000); // to prevent data flooding
}

//=======================================================================Function=================================================================================

void reconnect() {
  // Loop until we’re reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    String clientId = "ESP32WasteBin1-";  // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe("/ESEiot/test/");  // subscribe the topics here
      //client.subscribe(command2_topic);   // subscribe the topics here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");  // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//=======================================
// This void is called every time we have a message from the broker

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage += (char)payload[i];
  Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);
  // check for other commands
  /* else if( strcmp(topic,command2_topic) == 0){
if (incommingMessage.equals("1")) { } // do something else
}
*/
}

//======================================= publising as string
void publishMessage(const char* topic, String payload, boolean retained) {
  if (client.publish(topic, payload.c_str(), true))
    Serial.println("Message publised [" + String(topic) + "]: " + payload);
}