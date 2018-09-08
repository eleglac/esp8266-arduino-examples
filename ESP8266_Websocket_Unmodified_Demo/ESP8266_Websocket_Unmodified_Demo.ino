#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

const char* ssid     = "The Internet";
const char* password = "Dykwim2mNO";
char path[] = "/";
char host[] = "echo.websocket.org";
  
WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(5000);
  

  // Connect to the websocket server
  if (client.connect("echo.websocket.org", 80)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while(1) {
      // Hang on failure
    }
  }

  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  Serial.println("Attempting handshake...");
  while (!webSocketClient.handshake(client)) {
    Serial.print(".");
    delay(100);
  } 
}


void loop() {
  String data;

  if (client.connected()) {
    
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
    }
    
    data = "A string literal";
    
    webSocketClient.sendData(data);
    
  } else {
    Serial.println("Client disconnected.");
  }
  
  // wait to fully let the client disconnect
  delay(3000);
  
}
