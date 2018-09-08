#include <MCP3008.h>
#include <ESP8266WiFi.h>
 
// Pin setup... using SPI protocol, I think.  No serial!
#define CLOCK_PIN 0 
#define MISO_PIN 1 //GPIO 1 = TX on esp01
#define MOSI_PIN 2
#define CS_PIN 3 //GPIO 3 = RX on esp01

#define ADC_CHAN_CT 8 //how many channels does our ADC have?

const char* ssid     = "The Internet";
const char* password = "Dykwim2mNO";

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN); //SPI analog/digital converter (pH, moisture, light levels)

WiFiServer server(80); 

void setup() {
  delay(100);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
  
  // Start the server
  server.begin();
}
 
void loop() {
  //handle ADC updating...

  int ADCvals[ADC_CHAN_CT];

  for (int i = 0; i < ADC_CHAN_CT; i++) {
    ADCvals[i] = adc.readADC(i);
  }
  
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
  String response  = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>An ADC Experiment...</title></head><body><p>The ADC reads: </p><span id=\"adc-reading\">";
  for (int i = 0; i < ADC_CHAN_CT; i++) {
    response += "<p>Channel ";
    response += i;
    response += ": ";
    response += ADCvals[i];
    response += "</p>";
  }
  response += "</span></body></html>";

  // Send the response to the client
  client.print(response);
  delay(1000);

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
  
