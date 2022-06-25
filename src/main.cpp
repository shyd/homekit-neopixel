#include "HomeSpan.h" 
#include "DevPixel.h"


void setup() {
  
  Serial.begin(115200);
 
  homeSpan.begin(Category::Lighting,"Pixel LEDS" DEVICE_SUFFIX);

  SPAN_ACCESSORY();                                             // create Bridge (note this sketch uses the SPAN_ACCESSORY() macro, introduced in v1.5.1 --- see the HomeSpan API Reference for details on this convenience macro)

  //SPAN_ACCESSORY("Neo RGB");
  //  new NeoPixel_RGB(NEOPIXEL_RGB_PIN,1);                       // create 8-LED NeoPixel RGB Strand with full color control

  SPAN_ACCESSORY("Neo RGBW");
    new NeoPixel_RGBW_Full(NEOPIXEL_RGBW_PIN,7);                    // create 60-LED NeoPixel RGBW Strand  with simulated color temperature control 

}


void loop() {
  homeSpan.poll();
}
