/**
 * Eight led strips of 8 pixels each are hooked up to pin 6-13.
 * 
 * Touching the sensor is mapped to random flickering leds. 
 * More touching = more leds :)
 * 
 */
#include <FastLED.h>
#include <CapacitiveSensor.h>

// How many leds are in the strip?
#define NUM_STRIPS 8
#define NUM_LEDS_STRIP 8
#define NUM_LEDS NUM_LEDS_STRIP * NUM_STRIPS

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

#define SMOOTH_ALPHA 0.3

float smoothVal = 0; // global for the smoothing function
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2); // 1-10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
float maxVal = 0; // var holding maximum capacitance sensor value

// This function sets up the ledsand tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  FastLED.addLeds<WS2813, 6>(leds,  0 * NUM_LEDS_STRIP, NUM_LEDS_STRIP); // starts at 0 ( 0 * NUM_LEDS )
  FastLED.addLeds<WS2813, 7>(leds,  1 * NUM_LEDS_STRIP, NUM_LEDS_STRIP); // start at 8
  FastLED.addLeds<WS2813, 8>(leds,  2 * NUM_LEDS_STRIP, NUM_LEDS_STRIP); // starts at 16
  FastLED.addLeds<WS2813, 9>(leds,  3 * NUM_LEDS_STRIP, NUM_LEDS_STRIP); // etc...
  FastLED.addLeds<WS2813, 10>(leds, 4 * NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  FastLED.addLeds<WS2813, 11>(leds, 5 * NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  FastLED.addLeds<WS2813, 12>(leds, 6 * NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  FastLED.addLeds<WS2813, 13>(leds, 7 * NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  FastLED.setBrightness(255 / 4); // 25% brightness to save our eyes (and battery) a bit.
  cs_4_2.set_CS_Timeout_Millis(100);
  Serial.begin(9600);
}

// This function runs over and over, ands is where you do the magic to light
// your leds.
int whiteLed = 0;

void loop() {
  long val =  cs_4_2.capacitiveSensor(30);
  FastLED.clear();

  float smoothed = smooth(val);

  //  Serial.print(val);                  // print sensor output 1
  //  Serial.print("\t");
  //  Serial.println(smoothed);            // print smoothed value
  //  Serial.print("\t");
  if ( smoothed > maxVal ) {
    maxVal = smoothed;
  }

  int chanceOfGlitter = map( smoothed, 0, maxVal, 0, 255 );

  for ( int led = 0; led < NUM_LEDS; led++ ) {
    int r = random8();
    if ( r < chanceOfGlitter) {
      leds[ led ] += CRGB::White; // or use CHSV((random(85)),255,255); for random colors for example
    }
  }

  // Show the leds (only one of which is set to white, from above)
  FastLED.show();

  // Wait a little bit
  delay(20);
}


// returns the value smoothed with an exponential filter
float smooth(long rawVal) {
  smoothVal = ((float)rawVal * SMOOTH_ALPHA) + (smoothVal * (1 - SMOOTH_ALPHA));
  return smoothVal;
}
