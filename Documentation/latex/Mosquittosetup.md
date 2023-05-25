# Handleiding voor het opzetten van de Mosquitto Broker

Deze handleiding biedt begeleiding bij het instellen en gebruiken van de Mosquitto broker met het meegeleverde Arduino-script. De Mosquitto broker maakt communicatie mogelijk tussen het Arduino-apparaat en andere MQTT-clients. Volg de onderstaande instructies om de benodigde instellingen te configureren voor succesvolle communicatie.

## Vereisten

- Arduino-bord (bijv. ESP32)
- Arduino IDE (Integrated Development Environment)
- Geïnstalleerde en actieve Mosquitto broker

## Configuratie van het Arduino-script

Open het Arduino-script (`.ino`-bestand) in de Arduino IDE en zoek het volgende gedeelte:

```cpp
// WiFi-instellingen
const char* SSID = "WWegvanons3";       ///< SSID van het WiFi-netwerk
const char* PASSWORD = "JuCasSan27@#";  ///< Wachtwoord voor het WiFi-netwerk

// MQTT Broker-instellingen
const char* MQTT_SERVER = "192.168.111.237";  ///< IP-adres van de MQTT broker-server
const char* MQTT_USERNAME = "mqtt";           ///< Gebruikersnaam van de MQTT broker
const char* MQTT_PASSWORD = "WasteBin5#";     ///< Wachtwoord van de MQTT broker
const int MQTT_PORT = 1883;                   ///< Poortnummer van de MQTT broker
```

1. **WiFi-instellingen**: Werk de `SSID`- en `PASSWORD`-variabelen bij met de gegevens van jouw WiFi-netwerk.

2. **MQTT Broker-instellingen**: Wijzig de `MQTT_SERVER`, `MQTT_USERNAME`, `MQTT_PASSWORD` en `MQTT_PORT`-variabelen om overeen te komen met jouw Mosquitto broker-configuratie. De `MQTT_SERVER` moet het IP-adres of de hostnaam van jouw MQTT broker bevatten.

## Verbinding maken met de Mosquitto Broker

Om het Arduino-apparaat met de Mosquitto broker te verbinden, zorg ervoor dat de broker actief is en toegankelijk is vanaf het netwerk van de Arduino. Volg deze stappen:

1. **WiFi-verbinding instellen**: Het Arduino-script bevat een `setupWiFi()`-functie die de verbinding met jouw WiFi-netwerk afhandelt. Het maakt automatisch verbinding met behulp van de opgegeven `SSID` en `PASSWORD`. Zorg ervoor dat het Arduino-apparaat binnen het bereik van jouw WiFi-netwerk bevindt.

2. **MQTT-client configureren**: Het script heeft een `setupMQTTClient()`-functie die de MQTT-client configureert met de opgegeven broker-instellingen. Controleer of de `MQTT_SERVER`, `MQTT_USERNAME`, `MQTT_PASSWORD` en `MQTT_PORT`-variabelen overeenkomen met jouw Mosquitto broker-configuratie.

3. **Uploaden en uitvoeren**: Upload het aangepaste Arduino-script naar jouw Arduino-bord en bekijk de Seriële Monitor voor de verbindingsstatus en eventuele foutmeldingen.

4. **Verbinding verifiëren**: Nadat het script succesvol is geüpload, zou het Arduino-apparaat proberen verbinding te maken met de Mosquitto broker. Controleer de Seriële Monitor op het bericht "

WiFi connected" gevolgd door het toegewezen IP-adres van de Arduino. Als de verbinding mislukt, zorg er dan voor dat de broker-instellingen en WiFi-inloggegevens correct zijn.

## Interactie met de Mosquitto Broker

Het Arduino-script voert de volgende taken uit:

- Leest sensordata uit een SHT4x-sensor.
- Update de positie van de deksel op basis van de status van een knop.
- Bestuurt een Peltier-module op basis van temperatuurfout.
- Publiceert sensordata naar de MQTT-broker.

Om te communiceren met de Mosquitto broker en gegevens te ontvangen die door de Arduino zijn gepubliceerd, heb je een MQTT-client nodig, zoals MQTT.fx, MQTT Explorer of een aangepaste toepassing.

1. **Inschrijven op sensordata**: Abonneer je in jouw MQTT-client op het onderwerp dat is gespecificeerd in de `SENSOR_DATA_TOPIC`-variabele ("/data/sensors/"). Hiermee kun je sensordata ontvangen die door de Arduino is gepubliceerd.

2. **Berichten publiceren om Arduino te besturen**: Om berichten naar het Arduino-apparaat te sturen en het gedrag ervan te besturen, publiceer je berichten naar specifieke onderwerpen die het Arduino-script kan verwerken. Raadpleeg de code van het script voor beschikbare onderwerpen en hun overeenkomstige acties.

3. **MQTT-berichten controleren**: Bekijk in jouw MQTT-client de inkomende berichten van de Arduino op het geabonneerde onderwerp. De Arduino zal periodiek sensordata publiceren op basis van de `msg_interval`-variabele.

## Conclusie

Door de instructies in deze handleiding te volgen, kun je de Mosquitto broker en het Arduino-script configureren om communicatie tot stand te brengen tussen jouw Arduino-apparaat en MQTT-clients.