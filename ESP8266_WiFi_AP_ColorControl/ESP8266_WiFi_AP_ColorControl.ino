#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    32
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 30
CRGB leds[NUM_LEDS];

/* Set these to your desired credentials. */
const char *ssid = "YOUR NET";
const char *password = "YOUR PASS";
String colorPage = "<!DOCTYPE html><head><title>Color Control Panel</title></head><body><h1>You now control the light color.</h1><form action='/' method='POST'>Red (0-255):<input type='number' name='red' min='0' max='255'><br>Green (0-255):<input type='number' name='green' min='0' max='255'><br>Blue (0-255):<input type='number' name='blue' min='0' max='255'><br><input type='submit'></form></body></html>";

ESP8266WebServer server(80);
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void handleRoot() {
  server.send(200, "text/html", colorPage);
}

void handlePost() {
  if (server.args() == 3) {
    uint8_t red = atol(server.arg(0).c_str());
    uint8_t grn = atol(server.arg(1).c_str());
    uint8_t blu = atol(server.arg(2).c_str());
    Serial.println("got new color...");
    fill_solid(currentPalette, 16, CRGB(red, grn, blu));
  }
  else {
    currentPalette = RainbowColors_p;
  }
  handleRoot();
}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

WiFiClient client;

void setup() {
  delay(1000);
  Serial.begin(115200);
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
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/", HTTP_POST, handlePost);
  server.begin();
  Serial.println("HTTP server started");
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
    
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop() {
  server.handleClient();
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
    
  FillLEDsFromPaletteColors(startIndex);
    
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
