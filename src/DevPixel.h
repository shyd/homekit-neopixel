/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2022 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
// HomeSpan Addressable RGB LED Examples.  Demonstrates use of:
//
//  * HomeSpan Pixel Class that provides for control of single-wire addressable RGB and RGBW LEDs, such as the WS2812 and SK6812
//  * HomeSpan Dot Class that provides for control of two-wire addressable RGB LEDs, such as the APA102 and SK9822
//
// IMPORTANT:  YOU LIKELY WILL NEED TO CHANGE THE PIN NUMBERS BELOW TO MATCH YOUR SPECIFIC ESP32/S2/C3 BOARD
//

#if defined(CONFIG_IDF_TARGET_ESP32)

  #define NEOPIXEL_RGB_PIN       26
  #define NEOPIXEL_RGBW_PIN      32
  #define DOTSTAR_DATA_PIN       33
  #define DOTSTAR_CLOCK_PIN      27
  #define DEVICE_SUFFIX          ""

#elif defined(CONFIG_IDF_TARGET_ESP32S2)

  #define NEOPIXEL_RGB_PIN       17
  #define NEOPIXEL_RGBW_PIN      38
  #define DOTSTAR_DATA_PIN       3
  #define DOTSTAR_CLOCK_PIN      7
  #define DEVICE_SUFFIX          "-S2"

#elif defined(CONFIG_IDF_TARGET_ESP32C3)

  #define NEOPIXEL_RGB_PIN       0
  #define NEOPIXEL_RGBW_PIN      3
  #define DOTSTAR_DATA_PIN       7
  #define DOTSTAR_CLOCK_PIN      2

  #define DEVICE_SUFFIX          "-C3"

#endif
 
#include "HomeSpan.h"
#include "extras/Pixel.h"                       // include the HomeSpan Pixel class

#include "math.h"
//#define DEG_TO_RAD(X) (M_PI*(X)/180)
void HSItoRGBW(float H, float S, float I, int *r, int *g, int *b, int *w);
///////////////////////////////

struct NeoPixel_RGB : Service::LightBulb {      // Addressable single-wire RGB LED Strand (e.g. NeoPixel)
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  Pixel *pixel;
  uint8_t nPixels;
  
  NeoPixel_RGB(uint8_t pin, uint8_t nPixels) : Service::LightBulb(){

    V.setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    pixel=new Pixel(pin);                     // creates Pixel LED on specified pin
    this->nPixels=nPixels;                    // save number of Pixels in this LED Strand
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    int p=power.getNewVal();
    
    float h=H.getNewVal<float>();       // range = [0,360]
    float s=S.getNewVal<float>();       // range = [0,100]
    float v=V.getNewVal<float>();       // range = [0,100]

    Pixel::Color color;

    pixel->set(color.HSV(h*p, s*p, v*p),nPixels);       // sets all nPixels to the same HSV color
          
    return(true);  
  }
};

///////////////////////////////

struct NeoPixel_RGBW : Service::LightBulb {      // Addressable single-wire RGBW LED Strand (e.g. NeoPixel)
 
  Characteristic::On power{0,true};
  Characteristic::Brightness V{100,true};
  Characteristic::ColorTemperature T{140,true};
  Pixel *pixel;
  uint8_t nPixels;
  
  NeoPixel_RGBW(uint8_t pin, uint8_t nPixels) : Service::LightBulb(){

    V.setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    pixel=new Pixel(pin,true);                // creates Pixel RGBW LED (second parameter set to true for RGBW) on specified pin
    this->nPixels=nPixels;                    // save number of Pixels in this LED Strand
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    int p=power.getNewVal();
    
    float v=V.getNewVal<float>();       // range = [0,100]
    float t=T.getNewVal<float>();       // range = [140,500] (140=coldest, 500=warmest)

    float hue=240-(t-140)/3;            // add in a splash of color between blue and green to simulated change of color temperature

    // Pixel::Color color;              // if static HSV method is used (below), there is no need to first create a Color object

    pixel->set(pixel->HSV(hue, 100, v*p, v*p),nPixels);       // sets all nPixels to the same HSV color (note use of static method pixel->HSV, instead of defining and setting Pixel::Color)
          
    return(true);  
  }
};

///////////////////////////////

struct NeoPixel_RGBW_Full : Service::LightBulb {      // Addressable single-wire RGBW LED Strand (e.g. NeoPixel)
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  Pixel *pixel;
  uint8_t nPixels;
  
  NeoPixel_RGBW_Full(uint8_t pin, uint8_t nPixels) : Service::LightBulb(){

    V.setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    pixel=new Pixel(pin,true);                // creates Pixel RGBW LED (second parameter set to true for RGBW) on specified pin
    this->nPixels=nPixels;                    // save number of Pixels in this LED Strand
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    int p=power.getNewVal();

    float h=H.getNewVal<float>();       // range = [0,360]
    float s=S.getNewVal<float>();       // range = [0,100]
    float v=V.getNewVal<float>();       // range = [0,100]

    int r, g, b, w;
    HSItoRGBW(h, s/100.0, v/100.0*p, &r, &g, &b, &w);
    Pixel::Color color;
    color = color.RGB(r, g, b, w);
    pixel->set(color, nPixels);
         
    return(true);  
  }
};

///////////////////////////////

struct DotStar_RGB : Service::LightBulb {      // Addressable two-wire RGB LED Strand (e.g. DotStar)
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  Dot *pixel;
  uint8_t nPixels;
  
  DotStar_RGB(uint8_t dataPin, uint8_t clockPin, uint8_t nPixels) : Service::LightBulb(){

    V.setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    pixel=new Dot(dataPin,clockPin);          // creates Dot LED on specified pins
    this->nPixels=nPixels;                    // save number of Pixels in this LED Strand
    update();                                 // manually call update() to set pixel with restored initial values
    update();                                 // call second update() a second time - DotStar seems to need to be "refreshed" upon start-up
  }

  boolean update() override {

    int p=power.getNewVal();
    
    float h=H.getNewVal<float>();       // range = [0,360]
    float s=S.getNewVal<float>();       // range = [0,100]
    float v=V.getNewVal<float>();       // range = [0,100]

    Dot::Color color[nPixels];          // create an arrary of Colors

    float hueStep=360.0/nPixels;        // step size for change in hue from one pixel to the next

    for(int i=0;i<nPixels;i++)
      color[i].HSV(h+i*hueStep,s,100,v*p);   // create spectrum of all hues starting with specified Hue; use current-limiting parameter (4th argument) to control overall brightness, instead of PWM
      
    pixel->set(color,nPixels);          // set the colors according to the array
          
    return(true);  
  }
};

void HSItoRGBW(float H, float S, float I, int *r, int *g, int *b, int *w){
// https://www.neltnerlabs.com/saikoled/how-to-convert-from-hsi-to-rgb-white
float cos_h, cos_1047_h;
  H = fmod(H,360); // cycle H around to 0-360 degrees
  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
  
  if(H < 2.09439) {
    cos_h = cos(H);
    cos_1047_h = cos(1.047196667-H);
    *r = S*255*I/3*(1+cos_h/cos_1047_h);
    *g = S*255*I/3*(1+(1-cos_h/cos_1047_h));
    *b = 0;
    *w = 255*(1-S)*I;
  } else if(H < 4.188787) {
    H = H - 2.09439;
    cos_h = cos(H);
    cos_1047_h = cos(1.047196667-H);
    *g = S*255*I/3*(1+cos_h/cos_1047_h);
    *b = S*255*I/3*(1+(1-cos_h/cos_1047_h));
    *r = 0;
    *w = 255*(1-S)*I;
  } else {
    H = H - 4.188787;
    cos_h = cos(H);
    cos_1047_h = cos(1.047196667-H);
    *b = S*255*I/3*(1+cos_h/cos_1047_h);
    *r = S*255*I/3*(1+(1-cos_h/cos_1047_h));
    *g = 0;
    *w = 255*(1-S)*I;
  }
}

///////////////////////////////