/*Codigo de prueba para proyecto de Hallowween del grupo Makers Guaymas
 *
 * El codigo prueba la integracion de un sensor ultrasonico y un sensor PIR para detectar si existe una prescencia,
 * y ademas saber a que distancia se encuentra, basado en eso hay un neopixel que cambia de color dependiendo de la distancia.
 *
 * Alexander Hofflichs
 * Saul Urias
 * Viri Cruz
 * Jhonatan Laguna
 * Octubre 2015
 */

//Lo primero que hay que hace es importar nuestras librerias

#include <Adafruit_NeoPixel.h>
#include <NewPing.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <MP3.h> //ELECHOUSE MP3 shield v4.1

//***************************************
//Luego declaramos los pines que contendran nuestros sensores

#define detectarPresencia 3    //Conectamos el sensor PIR a nuestro D3

#define TRIGGER_PIN  4  // Esto es que esta conectado al pin D4 y al  pin Trigger del sensor ultrasonico, que se encarga de activar el sensor
#define ECHO_PIN     5  // Esto es que esta conectado al pin D5 y al  pin Echo del sensor ultrasonico, que se encarga de recibir las ondas de regreso

#define PIN 6   // Conecgtamos nuestro NEOPIXEL al D6

#define SERVO_PIN 10//Conectamos nuestro servo al D9 de nuestro arduino

//****************************************

//Despues definimos parametros de control para nuestros sensores
//PIR (Passive infrarojo Resistor)
int TiempoCalibracion = 10;  //Tiempo de calibracion para el sensor de 10 a 60 seg segun datasheet
long unsigned int lowIn;     // El tiempo cuando el sensor emite un pulso LOW
long unsigned int pause = 5000;  //Tiempo de espera antes de hacer una pausa, ya que todo el movimiento ceso
boolean lockLow = true;
boolean takeLowTime;

//ULTRASONICO
#define MAX_DISTANCE 400 //Distancia maxima que queremos hacer ping (en centimetros). La distancia maxima del sensor esta entre 400-500cm.


//NEOPIXEL
#define NUMPIXELS 2  //La cantidad de NEOPIXELES a utilizar
int delayval = 50; // delay for half a


///MP3 Shield
MP3 mp3;

//Colores RGB
int rojo;
int verde;
int azul;
int ojosEnojado;
int enojadoMil;

//SERVO
int angulo = 0;  //Para el control de los pasos
int contadorPalabras = 0;
//****************************************

//Por ultimo creamos los objetos a utilizar, en base a las librerias importadas sera, el sonar, los ojos y el motor

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Damos de alta los pines y la distancia maxima

//Creamos el objeto del ojo izquierdo
Adafruit_NeoPixel ojoIzquierdo = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Creamos el objeto del ojo derecho
Adafruit_NeoPixel ojoDerecho = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);

//El motor
Servo servo;

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis;
// constants won't change :
const long interval = 8000;
int totales;
void setup() {


  Serial.begin(9600);

  //Inicializamos el sensor PIR como una entrada y decimos que esta "apagado"
  pinMode(detectarPresencia, INPUT);
  digitalWrite(detectarPresencia, LOW);

  //Inicializamos los ojos
  ojoIzquierdo.begin();
  ojoDerecho.begin();

  //Inicializamos el motor
  servo.attach(SERVO_PIN);

  //Inicializa
  mp3.begin(MP3_SOFTWARE_SERIAL);
  mp3.volume(0x1F);

  //Le vamos a dar un tiempo al sensor para que se calibre
  calibrar();
}



void loop() {

  currentMillis = millis();
  if (digitalRead(detectarPresencia) == HIGH) {
    detectar();
    normal();
    alerta();
    molesto();
    enojado();

  }

  if (lockLow) {
    //makes sure we wait for a transition to LOW before any further output is made:
    lockLow = false;
    Serial.println("---");
    Serial.print("motion detected at ");
    Serial.print(millis() / 1000);
    Serial.println(" sec");
    delay(50);
  }

  takeLowTime = true;

  if (digitalRead(detectarPresencia) == LOW) {
    reposo();
  }

}

void detectar() {

  ojosEnojado = sonar.ping_cm();
  enojadoMil = map(ojosEnojado, 0, 200, 255, 0);
  Serial.print ("Movimiento detectado a: ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");
}

void enojado() {

  if (ojosEnojado > 0   && ojosEnojado < 80) {
       
    }

    for (int i = 0; i < 2; i++) {
      reir();
      rojo = 255;
      verde = 0;
      azul = 0;
      ojoIzquierdo.setPixelColor(0, ojoIzquierdo.Color(rojo, verde, azul));
      ojoDerecho.setPixelColor(0, ojoDerecho.Color(rojo, verde, azul));
      ojoIzquierdo.show(); // This sends the updated pixel color to the hardware.
      ojoDerecho.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).
    }
      mp3.play_usb_disk(0x0001);
      delay(8000);
      Serial.println("Reproduciendo Audio 1");
}

void molesto() {

  if (ojosEnojado > 81 && ojosEnojado < 130) {
    for (int i = 0; i < 2; i++) {
      respirar();
      rojo = 0;
      verde = 0;
      azul = 255;
      ojoIzquierdo.setPixelColor(0, ojoIzquierdo.Color(rojo, verde, azul));
      ojoDerecho.setPixelColor(0, ojoDerecho.Color(rojo, verde, azul));
      ojoIzquierdo.show(); // This sends the updated pixel color to the hardware.
      ojoDerecho.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).
    }
    mp3.play_usb_disk(0x0002);
    Serial.println("Reproduciendo Audio 2");
    delay(8000);
    


  }

}

void alerta() {

  if (ojosEnojado > 131 && ojosEnojado < 200) {
    for (int i = 0; i < 2; i++) {
    
      rojo = 0;
      verde = 255;
      azul = 0;
      ojoIzquierdo.setPixelColor(0, ojoIzquierdo.Color(rojo, verde, azul));
      ojoDerecho.setPixelColor(0, ojoDerecho.Color(rojo, verde, azul));
      ojoIzquierdo.show(); // This sends the updated pixel color to the hardware.
      ojoDerecho.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).
    }
    hablar();
    mp3.play_usb_disk(0x0003);
    Serial.println("Reproduciendo Audio 3");
    delay(8000);
    
  }
}

void normal() {
  if (ojosEnojado > 200) {
    for (int i = 0; i < 2; i++) {
      rojo = 255;
      verde = 255;
      azul = 255;
      ojoIzquierdo.setPixelColor(0, ojoIzquierdo.Color(rojo, verde, azul));
      ojoDerecho.setPixelColor(0, ojoDerecho.Color(rojo, verde, azul));
      ojoIzquierdo.show(); // This sends the updated pixel color to the hardware.
      ojoDerecho.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).
    }

  }
}

void calibrar() {
  Serial.print("Calibrando sensor");
  for (int i = 0; i < TiempoCalibracion; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Listo");
  Serial.println("SENSOR ACTIVO");
  delay(50);
}

void reposo() {

  if (takeLowTime) {
    lowIn = millis();          //save the time of the transition from high to LOW
    takeLowTime = false;       //make sure this is only done at the start of a LOW phase
  }
  //if the sensor is low for more than the given pause,
  //we assume that no more motion is going to happen
  if (!lockLow && millis() - lowIn > pause) {
    //makes sure this block of code is only executed again after
    //a new motion sequence has been detected
    lockLow = true;
    Serial.print("motion ended at ");      //output
    Serial.print((millis() - pause) / 1000);
    Serial.println(" sec");
    delay(50);
  }
}
void hablar() {
  servo.write(110);
  delay(200);
  servo.write(140);
  delay(200);
}
void reir() {
  servo.write(110);
  delay(150);
  servo.write(140);
  delay(150);
}
void respirar() {
  servo.write(110);
  delay(500);
  servo.write(130);
  delay(440);
}
