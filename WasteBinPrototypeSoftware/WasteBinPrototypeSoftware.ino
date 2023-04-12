#include "src/IoT/IoT.h"
#include "src/tempandhumid/tempandhumid.h"


/** Setup function */
void setup()
{
    void startIoT();
}

/** Loop function */
void loop()
{
    if (!client.connected())
        reconnect();
    client.loop();

    publishMessage(temperaturesensor_topic, String("temperaturesensor_message"), true);
    publishMessage(humiditysensor_topic, String("humiditysensor_message"), true);
    publishMessage(lidsensor_topic, String("lidsensor_message"), true);
    delay(2000); // to prevent data flooding
}
