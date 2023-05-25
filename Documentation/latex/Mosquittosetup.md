# Mosquitto Broker Setup Manual

This manual provides guidance on setting up and using the Mosquitto broker with the provided Arduino script. The Mosquitto broker enables communication between the Arduino device and other MQTT clients. Follow the instructions below to configure the necessary settings for successful communication.

## Requirements

- Arduino board (e.g., ESP32)
- Arduino IDE (Integrated Development Environment)
- Mosquitto broker installed and running

## Arduino Script Configuration

Open the Arduino script (`.ino` file) in the Arduino IDE and locate the following section:

```cpp
// WiFi settings
const char* SSID = "WWegvanons3";       ///< SSID of the WiFi network
const char* PASSWORD = "JuCasSan27@#";  ///< Password for the WiFi network

// MQTT Broker settings
const char* MQTT_SERVER = "192.168.111.237";  ///< MQTT broker server IP address
const char* MQTT_USERNAME = "mqtt";           ///< MQTT broker username
const char* MQTT_PASSWORD = "WasteBin5#";     ///< MQTT broker password
const int MQTT_PORT = 1883;                   ///< MQTT broker port number
```

1. **WiFi settings**: Update the `SSID` and `PASSWORD` variables with the credentials of your WiFi network.

2. **MQTT Broker settings**: Modify the `MQTT_SERVER`, `MQTT_USERNAME`, `MQTT_PASSWORD`, and `MQTT_PORT` variables to match your Mosquitto broker configuration. The `MQTT_SERVER` should contain the IP address or hostname of your MQTT broker.

## Connecting to Mosquitto Broker

To connect the Arduino device to the Mosquitto broker, ensure that the broker is running and accessible from the Arduino's network. Follow these steps:

1. **Setup WiFi Connection**: The Arduino script includes a `setupWiFi()` function, which handles connecting to your WiFi network. It automatically connects using the provided `SSID` and `PASSWORD`. Make sure the Arduino device is within range of your WiFi network.

2. **Configure MQTT Client**: The script has a `setupMQTTClient()` function that sets up the MQTT client with the provided broker settings. Verify that the `MQTT_SERVER`, `MQTT_USERNAME`, `MQTT_PASSWORD`, and `MQTT_PORT` variables match your Mosquitto broker configuration.

3. **Upload and Run**: Upload the modified Arduino script to your Arduino board and monitor the Serial Monitor for connection status and any error messages.

4. **Verify Connection**: After successfully uploading the script, the Arduino device should attempt to connect to the Mosquitto broker. Check the Serial Monitor for the "WiFi connected" message followed by the Arduino's assigned IP address. If the connection fails, ensure that the broker settings and WiFi credentials are correct.

## Interacting with the Mosquitto Broker

The Arduino script performs the following tasks:

- Reads sensor data from an SHT4x sensor.
- Updates the lid position based on a button state.
- Controls a Peltier module based on temperature error.
- Publishes sensor data to the MQTT broker.

To interact with the Mosquitto broker and receive data published by the Arduino, you need an MQTT client such as MQTT.fx, MQTT Explorer, or a custom application.

1. **Subscribe to Sensor Data**: In your MQTT client, subscribe to the topic specified in the `SENSOR_DATA_TOPIC` variable ("/data/sensors/"). This allows you to receive sensor data published by the Arduino.

2. **Publish Messages to Control Arduino**: To send messages to the Arduino device and control its behavior, publish messages to specific topics that the Arduino script can handle. Refer to the script's code for available topics and their corresponding actions.

3. **Monitor MQTT Messages**: In your MQTT

 client, observe the incoming messages from the Arduino on the subscribed topic. The Arduino will publish sensor data periodically based on the `msg_interval` variable.

## Conclusion

By following the instructions provided in this manual, you can configure the Mosquitto broker and the Arduino script to establish communication between your Arduino device and MQTT clients.
