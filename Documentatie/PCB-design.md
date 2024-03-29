# PCB-ontwerp

**Voorlopig prototype:**

Voor het voorlopige prototype maken we gebruik van twee specifieke PCB's: één voor de conversie en één voor het eindproduct. De "IoT" PCB bevat alle functionele componenten die het prototype onderscheiden van het uiteindelijke product. De overige componenten bevinden zich op de "eindproduct" PCB.

Voor het prototype moeten we de volgende gegevens kunnen uitlezen:

1. Luchtvochtigheid
2. Temperatuur
3. Klepstand (indien van toepassing)

Voor het eindproduct moeten we de volgende gegevens kunnen uitlezen:

1. Temperatuur (in graden Celsius)
2. Klepstand (open/gesloten)

**Discussie prototype:**

*Optie 1:*
Gebruik van één specifieke PCB voor het prototype en één specifieke PCB voor het eindproduct heeft de volgende voordelen:

- Prototypes zijn vaak duurder. Door te besparen op productiekosten van 30 vuilnisbakken, kunnen we veel ontwerptijd besparen. Het gebruik van een wat eenvoudigere printplaat met duurdere componenten kan uiteindelijk net zo kosteneffectief zijn.
- Het eindproduct heeft geen IoT-functionaliteit, dus een nieuw en compacter PCB-ontwerp is zinvol bij grootschalige productie.

*Optie 2:*
Een alternatieve benadering is om een coöperatieve PCB te gebruiken. Hierbij wordt één hoofd-PCB met IoT-functionaliteit fysiek verbonden met de PCB van het eindproduct. Het IoT PCB wordt verwijderd tijdens de productie. De redenen hiervoor zijn als volgt:

- Het hergebruiken van de 30 prototype-printplaten voor het uiteindelijke ontwerp kan kosteneffectiever zijn voor de prototypes.
- Het tegenargument is dat dit punt bij een grootschalige productie van bijvoorbeeld 1000 vuilnisbakken bijna verwaarloosbaar kan worden en daarmee het argument niet meer van toepassing is.
- Het kan zo zijn dat het ontdekken van fouten in de hardware makkelijker is als de printplaten los van elkaar te koppelen zijn.

**Keuze voor printplaten:**

Na een gesprek met de opdrachtgever is het akkoord gegeven om vanuit optie 1 te werk te gaan. Het prototype printplaat word ontworpen op de ESP32 C3 Devkit module. Deze module is gemakkelijk op de printplaat te schuiven door middel van de 2 headers. De printplaat is volledig afgestemd op het prototype en word ontworpen als een proof of concept voor het uiteindelijke resultaat. De printplaat voor het eindproduct zal immers meerdere testfases heen moeten voordat deze in productie kan. Vandaar dat wij ons hier nu niet op focussen.

## PCB V0.1 Ontwerp

Het ontwerp van de eerste printplaatversie (V0.1) is een bijna volledige kopie van het prototype met enkele wijzigingen. De buck converter is vervangen door een eenvoudige LDO omdat de buck converter overgespecificeerd was. Bovendien zijn er headers toegevoegd voor extra voeding en GPIO pinnen, waardoor het prototype flexibel blijft en mogelijke tests kunnen worden uitgevoerd, zelfs als deze printplaat die functies nog niet heeft. Het schema van deze printplaat ziet er als volgt uit:

![schematicv0.1](schematicv0.1.png)

Hierbij hoort ook een 3D-ontwerp dat als .step-bestand wordt geëxporteerd naar de IPO-studenten. Bij het indelen van de componenten heb ik groepen gemaakt van alle modules. De module met hoge stroom, de Peltier en het bijbehorende schakelmateriaal bevinden zich dicht bij de voedingsjack om te voorkomen dat er hoge stroom door het hele bord loopt en datalijnen mogelijk worden verstoord. Dit maakt het ook gemakkelijker om verbindingen te leggen. Daarnaast heb ik ervoor gezorgd dat alles zeer toegankelijk en gemakkelijk te solderen is, voor zover mogelijk. Tot slot heb ik de silkscreen geplaatst bij alle pinnen, zodat het gemakkelijker is om te zien waar welke pin zich bevindt, zonder telkens het schema of het printplaatontwerp te raadplegen. De silkscreen helpt ook bij het minimaliseren van mogelijke foutieve aansluitingen door de gebruiker.

![3ddesignv0.1](3ddesignv0.1.png)
