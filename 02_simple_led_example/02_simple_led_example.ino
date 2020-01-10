/**
 * Basic led strip example
 * 
 * Test if the led strip is hook up correctly. 
 * The code should light up each single led in turn.
 * 
 * Connect the led strip GND and 5V to GND and 5V on the Arduino
 * Connect DI to pin 10 on the Arduino (or change the code to another pin)
 * Change NUM_LEDS to the correct number for your strip.
 * Upload.
 * 
 */

#include <FastLED.h>

// How many leds are in the strip?
#define NUM_LEDS 15

// Data pin that led data will be written out over (the pin connected to "DI" on the ledstrip) 
#define DATA_PIN 10

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// This function sets up the leds and tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS);
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
  // Move a single white led
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::White;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(100);

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;
  }
}
