#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <IRremote.h>
//#include <IRrecv.h>
//#include <IRutils.h>
#include <ESP32Servo.h>
Servo servo1;
Servo servo2;
// Published values for SG90 servos; adjust if needed / Utiliser ici pour les MG90S
int minUs = 500;
int maxUs = 2450;
int pos1 = 0;    // variable to store the servo position
int pos2 = 0;    // variable to store the servo position
ESP32PWM pwm;
int servo1Pin = 10;
int servo2Pin = 11;






// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x24, 0x58, 0x7c, 0xda, 0x2d, 0x44};
//24:58:7c:da:2d:44
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define I2C_SDA 1
#define I2C_SCL 2

//ESP32AnalogRead adc;

int const EncoPin1 = 20;
int EncoValue1 = 1;
int const EncoPin2 = 21;
int EncoValue2 = 1;
int Enco_count1 = 0;
int Enco_count2 = 0;

// Paramètres PWM led
const int freq = 40000;       // Fréquence PWM
const int resolution = 8;     // Résolution en bits
int dutyCycle = 128;          // Rapport cyclique (0-255)
const int PWMLED1 = 8;
const int PWMLED2 = 9;

//Capteur Bas IR
const int WhitePin = 18;
int WhiteValue = 0;


/////TB6612FNG
//Motor 1
const int motor1Pin1 = 15;
const int motor1Pin2 = 16;
const int PWMmotor1 = 17;
//Motor 2
const int motor2Pin1 = 6;
const int motor2Pin2 = 7;
const int PWMmotor2 = 5;
//LowMode Disabled pin
//const int enable1Pin = 4;

int speed = 255;
const int frequency_motor = 5000;
const int resolution_motor = 8;

///// Pins pour les capteurs ultrasons
const int trigPinAM = 40; // Avant Milieu (Trig)
const int echoPinAM = 39; // Avant Milieu (Echo)
const int trigPinAG = 42; // Avant Gauche (Trig)
const int echoPinAG = 41; // Avant Gauche (Echo)
const int trigPinAD = 38; // Avant Droite (Trig)
const int echoPinAD = 37;  // Avant Droite (Echo)
const int trigPinCD = 35; // Côté Droit (Trig)
const int echoPinCD =36; // Côté Droit (Echo)
const int trigPinCG = 47; // Côté Gauche (Trig)
const int echoPinCG = 48; // Côté Gauche (Echo)
int ultraAM = 1;
int ultraAG = 2;
int ultraAD = 3;
int ultraCG = 4;
int ultraCD = 5;

// Définition de la vitesse du son en cm/µs
#define SOUND_SPEED 0.034
long duration;       // Durée de l'impulsion
float distanceCm;    // Distance en cm

//Variable machine a etat
int etatRobot = 0;

/////Suiveur IR
//Capteur Droite
const int IRPD = 13;
int IRVD = 1;
//Cateur Gauche
const int IRPG = 12;
int IRVG = 1;

/*
IRrecv IRRD(IRPD);
IRrecv IRRG(IRPG);
decode_results results1;
decode_results results2;
*/

//
int oldTime = millis();
int cycle = 1;



void setup() {
  //configuration de l'I2C pour l'écran Oled 128X64
  Wire.begin(I2C_SDA, I2C_SCL);
  //Configuration de  timer pour les 2 servo moteurs
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
  servo1.setPeriodHertz(50);      // Standard 50hz servo
	servo2.setPeriodHertz(50);      // Standard 50hz servo
  servo1.attach(servo1Pin, minUs, maxUs);
	servo2.attach(servo2Pin, minUs, maxUs);

  

  pinMode(EncoPin1, INPUT_PULLUP); 
  pinMode(EncoPin2, INPUT_PULLUP);
 // pinMode(enable2Pin, OUTPUT);
//Set pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(PWMmotor1, OUTPUT);
  pinMode(PWMmotor2, OUTPUT);
  pinMode(PWMLED1, OUTPUT);
  pinMode(PWMLED2, OUTPUT);

  
  ledcAttach(PWMmotor1,frequency_motor,resolution_motor);
  ledcAttach(PWMmotor2,frequency_motor,resolution_motor);

  ledcWrite(PWMmotor1, speed);
  ledcWrite(PWMmotor2, speed);
    
  ledcAttach(PWMLED1,30000,resolution);
  ledcAttach(PWMLED2,40000,resolution);

  ledcWrite(PWMLED1, dutyCycle);
  ledcWrite(PWMLED2, dutyCycle);

  pinMode(IRPD,INPUT);
  pinMode(IRPG,INPUT);





  // Configuration des pins des capteurs ultrasons
  pinMode(trigPinAM, OUTPUT); // Définir le trigPin comme une sortie
  pinMode(echoPinAM, INPUT);  // Définir l'echoPin comme une entrée
  pinMode(trigPinAG, OUTPUT);
  pinMode(echoPinAG, INPUT);
  pinMode(trigPinAD, OUTPUT);
  pinMode(echoPinAD, INPUT);
  pinMode(trigPinCD, OUTPUT);
  pinMode(echoPinCD, INPUT);
  pinMode(trigPinCG, OUTPUT);
  pinMode(echoPinCG, INPUT);

  


  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(10);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Hello, world!");
  display.display();


 //digitalWrite(enable1Pin, HIGH);

  


  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("MARCHE PAS");
  display.display();
  //delay(3000);
  //attachInterrupt(EncoPin1, enco1, RISING);
  attachInterrupt(EncoPin2, enco2, RISING);




//
int oldTime = millis();
int cycle = 1;  

//Variable machine a etat
int etatRobot = 0;

//Configuration AnalogRead
pinMode(WhitePin, INPUT);
  
 // IrReceiver.begin(IRPD);

}

void loop() {
  
/*
  if (IrReceiver.decode()){
        
    IRVD = 200;    
    IrReceiver.resume();
  }
  else IRVD = 10;  
  */
  IRsuivie();
  /*
  if (IRRD.decode(&results1)){
    if (results1.value == 0XFFFFFFFF)
      IRVD = 200;
    else IRVD = 100;
  }
  switch (results1.value){
    case 0xFFA25D:
      IRVG = 200;break;
    default : IRVG = 100;break;
  }
  IRRD.resume();
  */
  /*
  IrReceiver.decode();
  
  if(IrReceiver.decodedIRData.command == 0x10){
    IRVD = 200;
  }
  else IRVD = 100;

  

  IrReceiver.resume();
  *//*
  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    if (command == 0x10) {  // On/Off
      IRVD == 200;
    }
    IrReceiver.resume();
  }

  */
  


 // WhiteValue = analogRead(WhitePin);

  //EncoValue1 = digitalRead(EncoPin1);
  //EncoValue2 = digitalRead(EncoPin2);

  ultraAM = readSensor(trigPinAM, echoPinAM);
  ultraAG = readSensor(trigPinAG, echoPinAG);
  ultraAD = readSensor(trigPinAD, echoPinAD);
  ultraCD = readSensor(trigPinCD, echoPinCD);
  ultraCG = readSensor(trigPinCG, echoPinCG);

switch(etatRobot){
    case 0 : 
      avancer_led();
      //avancer();
      break;


    default : break;

  }


affichage();
  

}

void avancer(){
  
  if (ultraAM < 10){
    //backward();
    //delay(100);
    if(ultraAD <= ultraAG){
      gauche();
    } else {
      droite();
    }
    delay(75);

  } else if ( ultraAD < 10) {
    //backward();
    //delay(100);
    gauche();
    delay(75);
  } else if (ultraAG < 10){
    //backward();
    //delay(100);
    droite();
    delay(75);
  } else {
    forward();
  }

}

void avancer_led(){
  
  if (ultraAM < 10){
    //backward();
    //delay(100);
    if(ultraAD <= ultraAG){
      gauche();
    } else {
      droite();
    }
    delay(75);

  } else if ( ultraAD < 10) {
    //backward();
    //delay(100);
    gauche();
    delay(75);
  } else if (ultraAG < 10){
    //backward();
    //delay(100);
    droite();
    delay(75);
  } else {
    forward();
  }

  if ( IRVD == 0 && IRVG == 0 ){   
    forward();

  } else if ( IRVD == 0 && IRVG == 1) {
    droite();
    delay(25);

  } else if ( IRVD == 1 && IRVG == 0){
    gauche();
    delay(25);
  } else if ( IRVD == 1 && IRVG == 1){ 

  }
}

void forward() {

  // Move the DC motor forward at maximum speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

}

void gauche() {

  // Move the DC motor forward at maximum speed
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

}

void droite() {

  // Move the DC motor forward at maximum speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

}

void backward() {
  // Faire reculer les moteurs à pleine vitesse
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void motorStop() {
  // Arreter les moteurs
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void affichage() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Affichage du nombres de cycles et millisecondes par cycle
  // Celà permet un débug le prog
  //Nombre de cycles
  display.setCursor(0, 32);
  display.print("Cycle : ");
  display.println(cycle);
  cycle = cycle + 1;

  //Milliseconde par cycle
  display.setCursor(100, 32);
  display.println(millis() - oldTime);
  oldTime = millis();

  // Affichage de la variable pos du servomoteur du cateur infrarouge
  display.setCursor(0, 16);
  display.print("Pos1 : ");
  display.println(pos1);
  display.print("Pos2 : ");
  display.println(pos2);
  display.setCursor(64, 16);
  display.print("IR : ");
  display.print(IRVD);
  display.print(" ");
  display.println(IRVG);

  /////Capteur Ultrason
  //capteur Avant Milieu
  display.setCursor(48, 0);
  display.println(ultraAM);
  //Capteur Avant Gauche
  display.setCursor(0, 0);
  display.println(ultraAG);
  //Capteur Avant Droite
  display.setCursor(96, 0);
  display.println(ultraAD);
  //Capteur Arrière Gauche
  display.setCursor(24, 6);
  display.println(ultraCG);
  //Capteur Arrière Droite
  display.setCursor(72, 6);
  display.println(ultraCD);


  display.setCursor(0, 54);
  // Display static text
  display.println(EncoValue1);
  display.setCursor(32, 54);
  // Display static text
  display.println(EncoValue2);
  display.setCursor(64, 54);
  // Display static text
  display.println(Enco_count1);
  display.setCursor(96, 54);
  // Display static text
  display.println(Enco_count2);
  
  //display.setCursor(0, 46);
    //    display.println(IrReceiver.decodedIRData.command, HEX);


display.display();
}


int readSensor(int trigPin, int echoPin){
  
  
  // Fonction pour mesurer la distance avec un capteur ultrason
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH); // Lire la durée de l'écho
  distanceCm = duration * SOUND_SPEED / 2; // Calculer la distance
  return distanceCm;
}

void enco1 (){
  Enco_count1 = Enco_count1 + 1;

}

void enco2 (){
  Enco_count2 = Enco_count2 + 1;
  
}

void IRsuivie(){

  IRVD = digitalRead(IRPD);
  IRVG = digitalRead(IRPG);


}

void servoTest(){
  pos1 = 0;
  pos2 = 0;
  servo1.write(pos1);
  servo2.write(pos2);
  affichage();
  delay(1000);
  pos1 = 90;
  pos2 = 90;
  servo1.write(pos1);
  servo2.write(pos2);
  affichage();
  delay(1000);
  pos1 = 180;
  pos2 = 180;
  servo1.write(pos1);
  servo2.write(pos2);
  affichage();
  delay(1000);
}

void servoBallon(){
  pos2 = 180;
  servo2.write(pos2);
  delay(1000);
  pos2 = 0;
  servo2.write(pos2);
  affichage();
  delay(3000);

}
