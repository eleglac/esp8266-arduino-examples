#include <FastLED.h>

#define LED_PIN     0
#define TOGGLE      2
#define NUM_LEDS    32
#define BRIGHTNESS  32
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType currentBlending;

//extern CRGBPalette16 myRedWhiteBluePalette;
//extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
  delay(3000); //power-up safety delay
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  fill_solid( currentPalette, 16, CRGB::Green );
 
  currentBlending = LINEARBLEND;

  pinMode(TOGGLE, INPUT);

}

bool activeLow = false;

void loop()
{
  uint8_t colorIndex = 0;
  uint8_t brightness = 255;

  /*activeLow = digitalRead(TOGGLE);

  if (activeLow) {
    currentPalette[0] = CRGB::Red;
  } else {
    currentPalette[0] = CRGB::Blue;
  }*/
    
  for( int i = 1; i < NUM_LEDS; i++ ) {
    currentPalette[i] = currentPalette[i - 1];
  } 

  for( int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = currentPalette[i];
  }
  
  FastLED.show();
  delay(50);
}

/*
void FillLEDsFromPaletteColors( uint8_t colorIndex ) {
  uint8_t brightness = 255;

  for( int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 7;
  }
}*/

/* void ChangePalettePeriodically() {
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if( lastSecond != secondHand ) {
    lastSecond = secondHand;
    switch( secondHand ) {
      case 0: currentPalette = RainbowColors_p;           currentBlending = LINEARBLEND;  break;
      case 10: currentPalette = RainbowStripeColors_p;    currentBlending = NOBLEND;      break;
      case 15: currentPalette = RainbowStripeColors_p;    currentBlending = LINEARBLEND;  break;
      case 20: SetupPurpleAndGreenPalette();              currentBlending = LINEARBLEND;  break;
      case 25: SetupTotallyRandomPalette();               currentBlending = LINEARBLEND;  break;
      case 30: SetupBlackAndWhiteStripedPalette();        currentBlending = NOBLEND;      break;
      case 35: SetupBlackAndWhiteStripedPalette();        currentBlending = LINEARBLEND;  break;
      case 40: currentPalette = CloudColors_p;            currentBlending = LINEARBLEND;  break;
      case 45: currentPalette = PartyColors_p;            currentBlending = LINEARBLEND;  break;
      case 50: currentPalette = myRedWhiteBluePalette_p;  currentBlending = NOBLEND;      break;
      case 55: currentPalette = myRedWhiteBluePalette_p;  currentBlending = LINEARBLEND;  break;
    }
  }
} */

/* void SetupTotallyRandomPalette() {
  for( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8() );
  }
}

void SetupBlackAndWhiteStripedPalette() {
  fill_solid( currentPalette, 16, CRGB::Black );
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;
}

void SetupPurpleAndGreenPalette() {
  CRGB purple = CHSV( HUE_PURPLE, 255, 255 );
  CRGB green = CHSV( HUE_GREEN, 255, 255 );
  CRGB black = CRGB::Black;

  currentPalette = CRGBPalette16( green,  green,  black,  black, 
                                  purple, purple, black,  black, 
                                  green,  green,  black,  black, 
                                  purple, purple, black,  black );
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,
  
  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,
  
  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black  
}; */

// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
//
// taken from: https://randomnerdtutorials.com/guide-for-ws2812b-addressable-rgb-led-strip-with-arduino/

