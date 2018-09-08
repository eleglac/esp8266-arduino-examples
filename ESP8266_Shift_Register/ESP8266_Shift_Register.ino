/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 Arduino module, will be 
 *  printed to Serial when the module is connected.
 */

// #include <ESP8266WiFi.h>
#define SER_CLK 0
#define SER_DATA 2

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO for serial output to drive shift register
  pinMode(SER_CLK, OUTPUT);
  pinMode(SER_DATA, OUTPUT);
  digitalWrite(SER_CLK, LOW);
  digitalWrite(SER_DATA, LOW);
  
  //Display serial messages
  Serial.println();
  Serial.println();
  Serial.println("This is the serial shift-register demo.");
  Serial.println("I wonder if it will work...");  
}

void pushBit(int bitToPush) {
  if(bitToPush){
    digitalWrite(SER_DATA, HIGH);
  } else {
    digitalWrite(SER_DATA, LOW);
  }
  
  digitalWrite(SER_CLK, HIGH);
  delay(20);
  digitalWrite(SER_CLK, LOW);
  digitalWrite(SER_DATA, LOW);
}

int byteToDisplay = 0;

void loop() {  
  
  if (Serial.available()) {
    while (Serial.available() > 0) {
      byteToDisplay = Serial.read();
      for(int i=0;i<8;i++) {
        pushBit((byteToDisplay >> i) & 0x01); 
      }
    }    
  }
}
