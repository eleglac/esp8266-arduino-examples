#include <MCP3008.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
 
// Pin setup... using SPI protocol, I think.  No serial!
#define SPI_CLK   0 
#define MISO_PIN  1 //GPIO 1 = TX on esp01
#define MOSI_PIN  2
#define CS_PIN    3 //GPIO 3 = RX on esp01

// an alternate setup, using I2C... DUAL PROTOCOL ACTION WOO
#define I2C_SCL   0
#define I2C_SDA   1

#define ADC_CHAN_CT 8 //how many channels does our ADC have?

const char* ssid     = "NETGEAR49";
const char* password = "melodicboat600";
String errorMessage = "BMP280 OK";

//MCP3008 adc(SPI_CLK, MOSI_PIN, MISO_PIN, CS_PIN); //SPI analog/digital converter (pH, moisture, light levels)
Adafruit_BMP280 bmp; // I2C pressure/temp sensor
TwoWire wire; //need to change default SDA/SCL for ESP01

WiFiServer server(80); 

void setup() {
  delay(100);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
  
  // Start the server
  server.begin();

  // Start the pressure sensor  
  digitalWrite(CS_PIN, HIGH);   // disable the ADC chip
  wire.begin(I2C_SDA, I2C_SCL); // set the appropriate pins to communicate with BMP280 via I2C
  
  //if (!bmp.begin()) {
    //errorMessage = "BMP280 not detected!";
  //}
}

String getADCReadings() {
  MCP3008 adc(SPI_CLK, MOSI_PIN, MISO_PIN, CS_PIN); //SPI analog/digital converter (pH, moisture, light levels)
  String response = "<h3>MCP3008 Readings: </h3>";

  for (int i = 0; i < ADC_CHAN_CT; i++) {
    response += "<p>Channel ";
    response += i;
    response += ": ";
    response += adc.readADC(i);
    response += "</p>";
  }

  return response;
}

String getBMPReadings() {
  String response = "<h3>BMP280 Readings</h3>";
    
  if (errorMessage == "BMP280 OK") {
    response += "<p>Temperature: ";
    response += bmp.readTemperature();
    response += " degrees Celsius</p>";
    response += "<p>Atmospheric Pressure: ";
    response += bmp.readPressure();
    response += " Pascals</p>";
  } else {
    response += errorMessage;
  }

  return response;
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();
  
  // Match the request to root
  if (req.indexOf("/index") == -1) {
    client.stop();
    return;
  }

  client.flush();

  // Prepare the response
  String response  = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>An ADC Experiment...</title></head><body>";
  //response += getADCReadings();
  //response += getBMPReadings();
  response += "<span><h1>HELLO WORLD</h1>";
  response += "</span></body></html>";

  // Send the response to the client
  client.print(response);

  delay(2000);

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
  
