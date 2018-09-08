#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MCP3008.h>
//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BMP280.h>

// Pin setup... using SPI protocol, I think.  No serial!
#define SPI_CLK   0 
#define MISO_PIN  1 //GPIO 1 = TX on esp01
#define MOSI_PIN  2
#define CS_PIN    3 //GPIO 3 = RX on esp01... do not pull high, pull low

// an alternate setup, using I2C... DUAL PROTOCOL ACTION WOO
//#define I2C_SCL   0
//#define I2C_SDA   1

#define ADC_CHAN_CT 8 //how many channels does our ADC have?

const char* ssid     = "NETGEAR49";
const char* password = "melodicboat600";
String errorMessage = "BMP280 OK";
MCP3008 adc(SPI_CLK, MOSI_PIN, MISO_PIN, CS_PIN); //SPI analog/digital converter (pH, moisture, light levels)
//Adafruit_BMP280 bmp; // I2C pressure/temp sensor
//TwoWire wire; //need to change default SDA/SCL for ESP01

String indexPage = "<!DOCTYPE html><html><head><title>audio thing</title></head><body><h1>yup</h1></body></html>";
WiFiClient client;
ESP8266WebServer server(80);

int readingSeries[102];

void setupAverages() {
  readingSeries[0] = 0; //short-term moving average
  readingSeries[1] = 0; //long-term moving average
  for(int i = 2; i < 102; i++) {
    readingSeries[i] = adc.readADC(7); //always read from channel 7... don't forget!
  }
}

void getNewReading() {
  for(int i = 102; i > 2; i--) {
    readingSeries[i] = readingSeries[i-1];
  }
  readingSeries[2] = adc.readADC(7);
}

void setSTMA() {
  int accum = 0;
  for(int i = 2; i < 27; i++) {
    accum += readingSeries[i];
  }
  readingSeries[0] = accum/25;
}

void setLTMA() {
  int accum = 0;
  for(int i = 2; i < 102; i++) {
    accum += readingSeries[i];
  }
  readingSeries[1] = accum/100;
}

void updateAverages() {
  getNewReading();
  setSTMA();
  setLTMA();
}

void handleRoot() {
  server.send(200, "text/html", indexPage);
}

/*void handleADC() {
  String response = "<h3>MCP3008 Readings: </h3>";

  for (int i = 0; i < ADC_CHAN_CT; i++) {
    response += "<p>Channel ";
    response += i;
    response += ": ";
    response += adc.readADC(i);
    response += "</p>";
  }

  server.send(200, "text/html", response);
}*/

void handleADC() {
  String response = "<h3>Is it loud?</h3>";
  response += "<p>STMA (~250ms): ";
  response += readingSeries[0];
  response += "</p><p>LTMA (~1000ms): ";
  response += readingSeries[1];
  response += "</p><p>MACD: ";
  response += readingSeries[0] - readingSeries[1];
  response += "</p><p>Actual readings: </p>";
  for(int i = 2; i < 102; i++) {
    response += readingSeries[i];
    response += " ";
  }

  server.send(200, "text/html", response);
}

void setup() {
  delay(1000);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.on("/", HTTP_GET, handleRoot);
  server.on("/adc", HTTP_GET, handleADC);
  server.begin();

  setupAverages();
}

void loop() {
  server.handleClient();
  updateAverages();
  delay(10);
  
}
