/**********************************************
 * Catalin Batrinu bcatalin@gmail.com 
 * Read temperature and pressure from BMP280
 * and send it to thingspeaks.com
**********************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

Adafruit_BMP280 bme; // I2C
TwoWire wire; //need to change default SDA/SCL for ESP01

// replace with your channel’s thingspeak API key,

/**************************  
 *   S E T U P
 **************************/
void setup() {
  Serial.begin(115200);
  Serial.println("BMP280 test");

  wire.begin(2, 0);
  
  if (!bme.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  
}

  /**************************  
 *  L O O P
 **************************/
void loop() {
    Serial.print("T=");
    Serial.print(bme.readTemperature());
    Serial.print(" *C");
    
    Serial.print(" P=");
    Serial.print(bme.readPressure());
    Serial.print(" Pa");

    Serial.print(" A= ");
    Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
    Serial.println(" m");

    delay(5000);
}
