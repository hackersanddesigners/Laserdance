/**
 * Combines the previous two examples. Now, touching the sensor
 * should move the lit up led over the strip.
 */
 
#include <FastLED.h>
#include <CapacitiveSensor.h>

#define SMOOTH_ALPHA 0.3

float smoothVal = 0; // global for the smoothing function
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
float maxVal = 0; // variable holding maximum capacitance sensor value

// How many leds are in the strip?
#define NUM_LEDS 15

// Data pin that led data will be written out over
#define DATA_PIN 6

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];



// This function sets up the ledsand tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS);
  cs_4_2.set_CS_Timeout_Millis(100);
  Serial.begin(9600);
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
  long start = millis();
  long val =  cs_4_2.capacitiveSensor(30);

//  Serial.print(millis() - start);        // check on performance in milliseconds
//  Serial.print("\t");                    // tab character for debug windown spacing

  float smoothed = smooth(val);
  Serial.print(val);                  // print sensor output 1
  Serial.print("\t");
  Serial.print(smoothed);            // print smoothed value
  Serial.print("\t");
  
  if ( smoothed > maxVal ) {
    maxVal = smoothed; // remember the highest value
  }

  int currLed = map( smoothed, 0, maxVal, 0, NUM_LEDS -1 ); // calculate which led should light up
  Serial.print(maxVal);
  Serial.print("\t");
  Serial.println(currLed);
  
  
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    if ( currLed == whiteLed ) {
      leds[whiteLed] = CRGB::White;
    } else {
      leds[whiteLed] = CRGB::Black;
    }
  }

  FastLED.show();
  maxVal *= 0.999; // slowly decrease the highest value
  delay(20);

}

// returns the value smoothed with an exponential filter
float smooth(long rawVal) {
  smoothVal = ((float)rawVal * SMOOTH_ALPHA) + (smoothVal * (1 - SMOOTH_ALPHA));
  return smoothVal;
}
