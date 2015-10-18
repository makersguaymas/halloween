// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6
#define PIN2           5
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      2

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
//Creamos el objeto del ojo izquierdo
Adafruit_NeoPixel leftEye = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Creamos el objeto del ojo derecho
Adafruit_NeoPixel rightEye = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);
int delayval = 500; // delay for half a second
int fsr = 0; // Aqui se guardan los valores que obtenemos del FSR
int fsrMap = 0; //Aqui se guardan los valores que mapeamos del FSR
int fsrPin = 0; //Pin del FSR

//Colores RGB
int red;
int green;
int blue;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  // End of trinket special code
  Serial.begin(9600);

  
  leftEye.begin(); // This initializes the NeoPixel library.
  rightEye.begin();
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  fsr = analogRead(fsrPin);  //Leemos los datos del FSR
  fsrMap = map(fsr, 0 , 1023, 0 , 255); //Mapeamos los valores del FSR que va desde 0 a 1023, lo mapeamos desde 0 a 255 para modificar los colores
  Serial.println(fsrMap);
  

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    for(int i = 0; i < 2; i++)
    {
      red = fsrMap; //Le asignamos al color rojo el valor mapeado que obtenemos del FSR
      
      
      if(fsrMap > 170 && fsrMap < 210)
      {
//        red = 255;
//        green = 42;
//        blue= 4;
          red = fsrMap;
          blue = 0;
          green = 0;
      }
      else if(fsrMap > 210)
      {
        red = fsrMap;
        green = 0;
        blue = 0;
      }
      else if(fsrMap > 25)
      {
//        red = 248;
//        green = 137;
//        blue = 10;
          red = fsrMap;
          green = 0;
          blue = 0;
      }
      else
      {
        red = 25;
        blue = 0;
        green = 0;  
      }
      leftEye.setPixelColor(0, leftEye.Color(red,green,blue)); // Moderately bright green color.
      rightEye.setPixelColor(0, rightEye.Color(red,green,blue)); // Moderately bright green color.
      leftEye.show(); // This sends the updated pixel color to the hardware.
      rightEye.show(); // This sends the updated pixel color to the hardware.
      
      delay(delayval); // Delay for a period of time (in milliseconds).
    }
  }
