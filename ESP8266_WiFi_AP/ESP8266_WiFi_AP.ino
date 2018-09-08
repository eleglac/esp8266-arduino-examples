/* Create a WiFi access point and provide a web server on it.
ESP8266 Arduino example
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "set apartment color";
const char *password = "redgreenblue";
String colorPage = "<h1>You now control the light color.</h1>";
/*colorPage += "<form action='/' method='POST'>";
colorPage += "<input type='radio' name='blue' value='blue'>Blue<br>";
colorPage += "<input type='radio' value='green' name='green'>Green<br>";
colorPage += "<input type='submit'></form>";*/


ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
 
void handleRoot() {
  server.send(200, "text/html", colorPage);
}

void handlePost() {
  if (server.args() > 0) {
    for (int i = 0; i < server.args(); i++){
      Serial.print("POST Arguments: "); Serial.println(server.args());
      Serial.print("Name: "); Serial.println(server.argName(i));
      Serial.print("Value: "); Serial.println(server.arg(i));
    }
  }
  handleRoot();
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", HTTP_GET, handleRoot);
  /*server.on("/", HTTP_POST, handlePost);*/
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
