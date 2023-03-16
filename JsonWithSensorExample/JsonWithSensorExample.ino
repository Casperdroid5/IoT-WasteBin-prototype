// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.
//
// https://arduinojson.org/v6/example/generator/

#include <ArduinoJson.h>

const int button1Pin = 2;  // the number of the pushbutton pin
const int button2Pin = 3;  // the number of the pushbutton pin
int button1State = 0;      // variable for reading the pushbutton status
int button2State = 0;      // variable for reading the pushbutton status

int Button1 = 0;
int Button2 = 0;

StaticJsonDocument<200> doc;

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  while (!Serial) continue;
  // initialize the pushbutton pin as an input:
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
 

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);
}

void loop() {
  // not used in this example
  // read the state of the pushbutton value:
  button1State = digitalRead(button1Pin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (button1State == HIGH) {
    Button1 = 1;
  } else {
    Button1 = 0;
  }

  button2State = digitalRead(button2Pin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (button2State == HIGH) {
    Button2 = 1;
  } else {
    Button2 = 0;
  }


  // Add values in the document
  //
  doc["Button1"] = Button1;
  doc["Button2"] = Button2;

  // Add an array.
  //
  JsonArray data = doc.createNestedArray("ESP32SensorData");
  data.add(Button1);
  data.add(Button2);

  // Generate the minified JSON and send it to the Serial port.
  //
  serializeJson(doc, Serial);
  // The above line prints:
  // {"sensor":""Button1:(0/1)","Button2:(0/1)","data":[0/1,0/1]}

  // Start a new line
  Serial.println();

  // Generate the prettified JSON and send it to the Serial port.
  //
  serializeJsonPretty(doc, Serial);
  // The above line prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }
}

