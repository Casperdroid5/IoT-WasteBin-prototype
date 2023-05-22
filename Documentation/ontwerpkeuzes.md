# Ontwerpkeuzes

In dit document worden de functionele en technische specificaties besproken voor het ontwerp van het product. De functionele specificaties geven een overzicht van de gewenste functionaliteiten, terwijl de technische specificaties de gebruikte technologieën en componenten beschrijven.

## Functionele specificaties

| # | MoSCoW | Beschrijving |
|---|--------|------------|
| F1 | M | Het product moet de verspreiding van de GFE-afvalgeur voorkomen. |
| F1 | M | Het GFE-rottingsproces moet vier dagen worden vertraagd door het product. |
| F1.1 | M | Het GFE-afval moet gekoeld worden. |
| F1.1.1 | S | Het product moet zo efficiënt mogelijk de bak gekoeld krijgen en behouden. |
| F2 | M | De prijs moet onder de €50 blijven voor de consument bij afname van 100 stuks. |
| F3 | S | Het product moet gebruikersindicatoren hebben voor te lang openstaan. |
| F3.1 | S | Het product moet een temperatuurindicator (te warm/te koud) hebben. |
| F3.2 | C | Het product moet een inhoudshoeveelheidindicator (vol/leeg) hebben. |
| F4 | M | Het product prototype moet draadloze gegevensoverdracht hebben voor testdoeleinden. |
| F5 | C | Het product moet feedback aan de gebruiker geven. |
| F5.1 | C | Het product moet feedback geven via audio. |
| F5.2 | C | Het product moet feedback geven via licht/beeld. |
| F6 | W | Het product moet een "off the grid" functie hebben. |
| F7 | S | Het product moet simpel en intuïtief zijn in gebruik. |
| F8 | M | De prullenbakinhoud moet 6 liter zijn, afgestemd op het gemiddelde GFE-afval van een 4-persoonshuishouden. |
| F9 | S | Het product moet zo stil mogelijk zijn tijdens gebruik. |
| F10 | M | Het product moet de klepstand kunnen bepalen. |
| F11 | S | Het product moet zou energie moeten "terugwinnen" (door GFE-preservatie). |
| F12 | C | Het product (prototype) moet een logfunctie hebben voor diagnose. |

## Technische specificaties

| # | Beschrijving |
|---|------------|
| 1 | Het koelen van het product wordt gedaan met behulp van een Peltier-module. |
| 2 | D.m.v. LED-lampjes toont het product of de temperatuur te warm of te koud is. |
| 3 | Het product gebruikt een Piezo-speaker om de gebruiker te waarschuwen wanneer de klep te lang openstaat. |
| 4 | De klepstand wordt gemeten d.m.v. een Hall-effectsensor of een knop. |
| 5 | Bij logfuncties in het eindproduct is een externe EEPROM een optie. |
| 6 | Het product krijgt een externe voeding om de repareerbaarheid te verbeteren en de complexiteit te verminderen. |
| 7 | Het product gebruikt een Microchip ATTINY 13A als eindproduct microcontroller vanwege zijn kosten en tot nu toe voorziene geschiktheid. Ook beschikt hij over een programming lock voor extra beveiliging van het product. |
| 8 | De SHT40 wordt gebruikt om de temperatuur en luchtvochtigheid in het systeem te meten. Er bestaan goedkopere sensoren zoals de LM35, DHT22 of LM75 (communiceert via I2C). Deze goedkopere alternatieven of derden zijn te overwegen voor het eindproduct. |
