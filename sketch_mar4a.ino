#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

// Initialize LCD (I2C Address: 0x27 or 0x3F, 16x2 Display)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// DHT11 Sensor Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// BMP180 Sensor Setup
Adafruit_BMP085 bmp;

// Fan Setup
const int FAN_PIN = 3; // Fan connected to digital pin 3

void loop() {
  float temperatureDHT = dht.readTemperature();
  float humidity = dht.readHumidity();
  float pressure = bmp.readPressure() / 100.0;

  // Check if DHT readings are valid
  if (isnan(temperatureDHT) || isnan(humidity)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor error!");
    digitalWrite(FAN_PIN, LOW); // Ensure fan is off on sensor error
    delay(2000);
    return;
  }

  char tempStr[6], humStr[6];
  dtostrf(temperatureDHT, 4, 1, tempStr);
  dtostrf(humidity, 4, 1, humStr);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempStr);
  lcd.print("C ");
  lcd.print("H:");
  lcd.print(humStr);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(pressure);
  lcd.print("hPa");

  // Fan control logic
  if (temperatureDHT > 25.0) {
    digitalWrite(FAN_PIN, HIGH);
    Serial.println("Fan ON - Temperature is above 25°C!");
  } else {
    digitalWrite(FAN_PIN, LOW);
    Serial.println("Fan OFF - Temperature is 25°C or below.");
  }

  delay(2000);
}
