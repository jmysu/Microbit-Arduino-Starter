// This is a demo of the matrix driver code
// works with Adafruit GFX commands - its just
// a really small screen!
// https://learn.adafruit.com/adafruit-gfx-graphics-library

#include <Adafruit_Microbit.h>
Adafruit_Microbit_Matrix matrix;
Adafruit_Microbit ubitBLE;

const uint8_t
  smile_bmp[] =
  { B00000,
    B01010,
    B00000,
    B10001,
    B01110, };

//#include <Adafruit_NeoPixel.h> //Not works, change to FastLED
/*
//NeoPixels
const int numPixels = 4;
const int pixelPin =  16;
Adafruit_NeoPixel neo = Adafruit_NeoPixel(numPixels, pixelPin, NEO_RGB);
uint32_t foreground = neo.Color(0x00, 0xFF, 0xFF); // r, g, b - blue
uint32_t background = neo.Color(0x40, 0x40, 0x40); // dim white
*/

#include <FastLED.h>
#define NUM_LEDS 4
CRGBArray<NUM_LEDS> leds;

void setup() {  
  Serial.begin(9600);
  
  Serial.println("matrix matrix demo is ready!");

  //matrix.begin();

  //neo.setBrightness(200);
  //neo.begin(); // start the NeoPixel display
  //neo.show();
  FastLED.addLeds<NEOPIXEL, 16>(leds, 4); 

  //BLE custom services and characteristics can be added as well
  ubitBLE.BTLESerial.setLocalName("ubitBLE");
  ubitBLE.BTLESerial.begin();
  ubitBLE.begin();
}

// forward received from Serial to microbit.BTLESerial and vice versa
void forwardBLE() {
  if (ubitBLE.BTLESerial && Serial) {
    int byte;
    if (ubitBLE.BTLESerial.available()) {
      Serial.write(ubitBLE.BTLESerial.read());
    }
    char buffer[10];
    memset(buffer, 0x0, 10);
    int idx = 0;
    
    while (Serial.available() && idx != 10) {
       buffer[idx] = Serial.read();
       idx++;
    }
    if (idx) {
      ubitBLE.BTLESerial.write(buffer, idx);
    }
  }
  delay(1);
}

// echo all received data back
void loopbackBLE() {
  if (ubitBLE.BTLESerial) {
    int byte;
    while ((byte = ubitBLE.BTLESerial.read()) > 0) {
        ubitBLE.BTLESerial.write(byte);
    }
  }
}

// periodically sent time stamps
void spamBLE() {
  if (ubitBLE.BTLESerial) {
    ubitBLE.BTLESerial.print(millis());
    ubitBLE.BTLESerial.println(" tick-tacks!");
    delay(1000);
  }
}
//-----------------------------------------------------------------------------
int8_t k=0;    
void loop(){
  //BLE service -----------------
  ubitBLE.BTLESerial.poll();
  forwardBLE();
  //loopback();
  spamBLE();
  //-----------------------------
  if ((millis()%100)==0) {
      int h = random(80);
      leds[0] = CHSV(  0+h,220,16); //R
      leds[1] = CHSV( 85+h,220, 8); //G
      leds[2] = CHSV(170+h,220,32); //B
      leds[3] = CHSV(  0+h,  0, 8); //W
      FastLED.show();
      }
  //-----------------------------
  if ((millis()%500)==0) {
    matrix.begin();
    k++; if (k>5) k=0;
    switch (k) {
      case 0:// Fill screen
        matrix.fillScreen(LED_ON);
        break;
      case 1:// draw a heart
        matrix.show(matrix.HEART);
        break;
      case 2:// draw a no cross
        matrix.show(matrix.NO);
        break;
      case 3:// draw a yes check
        matrix.show(matrix.YES);
        break;
      case 4:// draw a custom made bitmap face
        matrix.show(smile_bmp);
        break;
      case 5:// neopixel udpate
        matrix.clear();
        break;
      }
    }
    
}


