#include <DHT.h>
#include <FastLED.h>

#define LED_PIN 0
#define NUM_LEDS 32
#define BRIGHTNESS 16
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define DHTTYPE DHT11

// DHT Sensor
const int DHTPin = 2;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType currentBlending;

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(3000);

  dht.begin();
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  currentPalette = CRGBPalette16( 
                                  CRGB::Red,    CRGB::Red,    CRGB::Red,    CRGB::Red,
                                  CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, 
                                  CRGB::Green,  CRGB::Green,  CRGB::Green,  CRGB::Green,
                                  CRGB::Green,  CRGB::Green,  CRGB::Green,  CRGB::Green
                                  );
 
  currentBlending = NOBLEND;
  
  Serial.println("Running...");
}

// runs over and over again
float last_hic = 0.0;

void loop() {
  
  delay(2000);
  float hic;
  
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if ( isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp,"Failed");
    strcpy(fahrenheitTemp, "Failed");
    strcpy(humidityTemp, "Failed");         
  }
  
  else {
    // Computes temperature values in Celsius + Fahrenheit and Humidity
    hic = dht.computeHeatIndex(t, h, false);       
    dtostrf(hic, 6, 2, celsiusTemp);        
    dtostrf(h, 6, 2, humidityTemp);
  }

  Serial.print("Humidity: ");
  Serial.print(humidityTemp);
  Serial.print("; Celsius: ");
  Serial.print(celsiusTemp);
  Serial.println("\n**********"); 

  float delta = hic - last_hic;
  
  // Lighting code  
  for( int i = 0; i < NUM_LEDS; i++ ) {

    /*if ( delta == 0 ) {
      leds[i] = CRGB( 0, 255, 0 );
    }
    else if ( delta < 0 ) {
      leds[i] = CRGB( 0, 0, 255 );
    }
    else {
      leds[i] = CRGB( 255 , 0, 0 );
    }*/

    leds[i] = CRGB( 255, 197, 143 );
  }

  FastLED.show();
  last_hic = hic;
}
