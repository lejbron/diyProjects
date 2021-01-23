/*
  Parts:
	 - x1 https://amperka.ru/product/display-lcd-text-20x4-i2c-3v3-black-on-green
	 - x1 https://amperka.ru/product/troyka-relay
	 - x1 https://amperka.ru/product/arduino-troyka-shield
	 - x1 https://amperka.ru/product/troyka-temperature-humidity-sensor-dht11
	 - x2 https://amperka.ru/product/soil-moisture-sensor
	 - x2 http://amperka.ru/product/solenoid-valve-normally-open
	 - x2 http://amperka.ru/product/troyka-mosfet-v2
	 - x2 http://rushim.ru/product_info.php?products_id=633
	 - x2 http://rushim.ru/product_info.php?products_id=2246
*/

#include "DHT.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

#include "IPlant.h"

/*=========SETTINGS=========*/
#define DEBUG true
#define MINT_CRITICAL_MOS 45
#define ARAUCARIA_CRITICAL_MOS 45
#define VOLUME 20000
/*=========SETTINGS=========*/

/*=============PINS=============*/
#define DHTPIN 8

//fc28 analog pins
int fc28_plant_pin1 = A1;
int fc28_plant_pin2 = A2;

int fc28DigitalPin1 = 9;
int fc28DigitalPin2 = 10;

DHT dht(DHTPIN, DHT11);

int mosfetPin1 = 11;
int mosfetPin2 = 12;

LiquidCrystal_I2C lcd(0x27,20,4);
/*=============PINS=============*/

/*==================================PLANTS==================================*/

IPlant mint(fc28_plant_pin1, fc28DigitalPin1, mosfetPin1, "Mint");
IPlant araucaria(fc28_plant_pin2, fc28DigitalPin2, mosfetPin2, "Araucaria");

/*==================================PLANTS==================================*/

void getDHT11Data()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.clear();

  if (isnan(h) || isnan(t)) {
    lcd.print("Reading error");
    if(DEBUG == true)
      Serial.println("Reading error");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("Air:");
    lcd.print((int)h);
    lcd.print("%");
    lcd.print(" T:");
    lcd.print((int)t);
    lcd.print("C");

    if(DEBUG == true)
    {
      Serial.print("Air:");
      Serial.print((int)h);
      Serial.println("%");
      Serial.print(" T:");
      Serial.print((int)t);
      Serial.println("C");
    }
  }
}

void setup()
{
	delay(2000);
  Serial.begin(9600);

  dht.begin();

  pinMode(mosfetPin1, OUTPUT);
  pinMode(mosfetPin2, OUTPUT);

  digitalWrite(mosfetPin1, LOW);
  digitalWrite(mosfetPin2, LOW);

  lcd.init();
  lcd.backlight();

  //getDHT11Data();
  Serial.println("Setup done");
  lcd.print("Setup done");
}

void loop()
{
  lcd.clear();
  getDHT11Data();
  delay(5000);
  araucaria.getMositure();
  delay(5000);
  lcd.clear();
  lcd.println("Water");
  digitalWrite(mosfetPin1, HIGH);
  delay(2000);
  digitalWrite(mosfetPin1, LOW);
  lcd.println("Finished");
  mint.getMositure();
  delay(5000);
  lcd.clear();
  lcd.println("Water");
  digitalWrite(mosfetPin2, HIGH);
  delay(2000);
  digitalWrite(mosfetPin2, LOW);
  lcd.println("Finished");
  
  //araucaria.waterPlant(500);

  /*if (mint.getMositure() < MINT_CRITICAL_MOS)
  {
    irrigation(mint, MINT_CRITICAL_MOS);
    delay(500);
    lcd.clear();
    sensorsData();
  }

  delay (1000);

  if (araucaria.getMositure() < ARAUCARIA_CRITICAL_MOS)
  {
    irrigation(araucaria, ARAUCARIA_CRITICAL_MOS);
    delay(500);
    lcd.clear();
    sensorsData();
  }*/
}

void sensorsData()
{
  lcd.clear();

  lcd.setCursor(0, 2);
  lcd.print(mint.getName());
  lcd.print(":");
  lcd.print(mint.getMositure());
  lcd.print("%");

  lcd.setCursor(0, 3);
  lcd.print(araucaria.getName());
  lcd.print(":");
  lcd.print(araucaria.getMositure());
  lcd.print("%");

  getDHT11Data();
}

/*void irrigation(IPlant plant, int critical_mos)
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Watering ");
  lcd.print(plant.getName());

  digitalWrite(RELAY_PIN, HIGH);

  do {
    lcd.setCursor(0, 2);
    lcd.print("Current mositure:");
    lcd.setCursor(7, 3);
    lcd.print(plant.getMositure());
    lcd.print("%");
    delay(1000);
    plant.waterPlant(VOLUME);
    delay(1000);
  } while(plant.getMositure() < critical_mos);

  digitalWrite(RELAY_PIN, LOW);

  lcd.setCursor(0, 0);
  lcd.print(plant.getName());
  lcd.print(" is watered.");
  delay(2000);
}*/

/*lejbron*/