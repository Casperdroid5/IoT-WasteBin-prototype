#include "tempandhumid.h"

void startTempAndHumid();
{

  Serial.println("Adafruit SHT4x test");
  if (!sht4.begin())
  {
    Serial.println("Couldn't find SHT4x");
    while (1)
      delay(1);
  }

  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);

  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  // You can have 6 different heater settings
  // higher heat and longer times uses more power
  // and reads will take longer too!
  sht4.setHeater(SHT4X_HIGH_HEATER_1S);

  sensors_event_t humidity, temp;

  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
  timestamp = millis() - timestamp;

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");
  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");

  Serial.print("Read duration (ms): ");
  Serial.println(timestamp);
}
