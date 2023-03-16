## Opstelling (Mosquitto, HiveMQ, Node-Red)

Meerdere ESP32-microcontrollers kunnen in combinatie met Node-Red worden gebruikt om een visuele interface te bouwen op basis van de gegevens die zijn gelogd door de ESP32. Elke Waste Bin zal data gaan versturen. Deze data zal eerst naar een MQTT-broker gaan. Wij gebruiken hier Mosquitto voor.

### Onderbouwing ESP32 C3

Wij kiezen voor een ESP32 omdat deze:
- 	Relatief goedkoop zijn (€2 per stuk)
- 	Beschikbaar zijn (1300 in stock bij Mouser)
-	Adequaat voor de taken die moeten worden uitgevoerd
-	Klein formaat

### Waarom geen Arduino of Raspberry Pi als computer voor de WasteBin?

Raspbery pi’s zijn op dit moment zeer lastig verkrijgbaar, erg duur in de aanschaf voor een testplatform en daarnaast veel tijdsintensiever om op te zetten. Hiernaast zijn deze veel te krachtig voor onze doeleinden en is zo’n Raspberry Pi erg overbodig op het gebied van rekenkracht.

Arduino’s zijn redelijke alternatieven, maar duurder dan losse ESP32 modules. Los hiervan kan je een ESP32 wel programmeren via de Arduino IDE met de Arduino code librarys indien gewenst.

Daarnaast denken wij dat het zeker een uitdaging is om te programmeren in een andere IDE dan Arduino IDE en we denken dat het leerzaam is om diep in de C/C++ code te moeten duiken. Bij Arduino is dit grotendeels al voor je uitgekauwd.

### Data forwarding

Data forwarding is de plek waar alle data van de ESP32’s (of wastebins) terecht komt. Het is een optie om daarvoor specifieke hardware te gebruiken zoals een raspberry pi met daarop een broker (zoals mosquitto) en een nodered dashboard). 

Wij kiezen ervoor om met het prototype de broker en dashboard op onze lokale laptop te installeren en geen raspberry pi te gebruiken. We hebben hiervoor meerdere redenen:

1.	We hebben een extra platform nodig (raspberry pi). Alles op 1 apparaat vinden wij makkelijker
2.	Als wij fysiek ergens anders willen werken moeten wij iedere keer de raspberry pi via een beeldscherm en toetsenbord verbinden met het netwerk dat zich op die plek bevind.
3.	Een Raspberry Pi is een extra bron voor fouten in hardware (sd kaart corruptie) en software (Linux problemen) etc
4. Een Raspberry Pi is erg prijzig
 
## Stappen:

Om dit doel 
1.	ESP32 data laten versturen (verbinden met internet)
2.	MQTT-broker opzetten
3.	MQTT-broker data van ESP32 accepteren 
4.	MQTT-broker data doorsturen naar Node-Red (lokaal)
5.	Ingaande data van MQTT-broker omzetten naar dashboardweergave

Data versturen
De data die wij willen versturen vanaf de ESP32:
•	Temperatuur (double)
•	Luchtvochtigheid (double)
•	Klepstand (boolean)
•	Error code(s) (int)
•	WasteBin-ID
Software- oplossing
Voor onze opstelling willen wij een speaker als indicator gebruiken, die verschillende waarschuwingen kan afgeven wanneer deze zich voordoen. Dit zijn:
-	De klep staat te lang open (misschien)
-	De binnenkant van de bak is te warm (vermoedelijk klep open of slechte koeling)
-	General error (Ander probleem)
Wij kiezen ervoor om deze waarschuwingen ook mee te sturen naar ons dashboard mochten deze zich voordoen. Deze waarschuwingen worden weergegeven als een error code.

Verstuur interval
In principe word alle data verstuurd elke 15 minuten. In het geval van een error zal dit interval worden genegeerd en zal dit meteen worden verstuurd. Het interval, mochten één of meerdere errors zich blijven voordoen, bedraagt 2 minuten.

MQTT Password: WasteBin5#

IoT-model
