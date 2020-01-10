/**
 * Basic capacitive sensor example using pin 2 and 4
 * 
 * Connect pin 2 and 4 with a 1 to 10MOhm resistor, and pin 2 work as a capacitive sensor.
 * The code prints out the update time (ms), the raw value and a smoothed value.
 * Use the Serial Plotter (Menu Tools > Serial Plotter) to inspect the output.
 * 
 * You can experiment with different values for the resistor and SMOOTH_ALPHA
 * 
 */
#include <CapacitiveSensor.h>

#define SMOOTH_ALPHA 0.3

float smoothVal = 0; // global for the smoothing function
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2);       // 1-10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

void setup()
{
  Serial.begin(9600);
  cs_4_2.set_CS_Timeout_Millis(100); // Make sure that the sensor does not slow down the sketch too much. This makes the function timeout after 100ms.
}

void loop()
{
  long start = millis();
  long val =  cs_4_2.capacitiveSensor(30);

  Serial.print(millis() - start);        // check on performance in milliseconds
  Serial.print("\t");                    // tab character for debug windown spacing
  Serial.print(val);                     // print sensor output 1
  Serial.print("\t");
  Serial.println(smooth(val));           // print smoothed value

  delay(10);                             // arbitrary delay to limit data to serial port
}

// returns the value smoothed with an exponential filter
float smooth(long rawVal) {
  smoothVal = ((float)rawVal * SMOOTH_ALPHA) + (smoothVal * (1 - SMOOTH_ALPHA));
  return smoothVal;
}
