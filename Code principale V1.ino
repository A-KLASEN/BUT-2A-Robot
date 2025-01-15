#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP32Servo.h>
Servo myservo;

#define SCREEN_WIDTH 128 // Largeur de l'écran OLED en pixels
#define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED en pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define I2C_SDA 5
#define I2C_SCL 6

// Configuration des moteurs
int motor1Pin1 = 15; // Pin du moteur 1, direction 1
int motor1Pin2 = 16; // Pin du moteur 1, direction 2
int enable1Pin = 7;  // Pin d'activation du moteur 1
int motor2Pin1 = 17; // Pin du moteur 2, direction 1
int motor2Pin2 = 18; // Pin du moteur 2, direction 2
int enable2Pin = 8;  // Pin d'activation du moteur 2

// Paramètres PWM
const int freq = 30000;       // Fréquence PWM
const int resolution = 8;     // Résolution en bits
int dutyCycle = 255;          // Rapport cyclique (0-255)

// Pins pour les capteurs ultrasons
const int trigPinAM = 12; // Avant Milieu (Trig)
const int echoPinAM = 11; // Avant Milieu (Echo)
const int trigPinAG = 14; // Avant Gauche (Trig)
const int echoPinAG = 13; // Avant Gauche (Echo)
const int trigPinAD = 10; // Avant Droite (Trig)
const int echoPinAD = 9;  // Avant Droite (Echo)
const int trigPinCD = 41; // Côté Droit (Trig)
const int echoPinCD = 40; // Côté Droit (Echo)
const int trigPinCG = 38; // Côté Gauche (Trig)
const int echoPinCG = 39; // Côté Gauche (Echo)

int ultraAM = 200;
int ultraAG = 200;
int ultraAD = 200;
int ultraCG = 200;
int ultraCD = 200;

// Définition de la vitesse du son en cm/µs
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;       // Durée de l'impulsion
float distanceCm;    // Distance en cm

//servo moteur
int servoPin = 45;
int pos = 0;    // variable to store the servo position


//
int oldTime = millis();
int cycle = 1;
//Variable machine a etat
int etatRobot = 0;

//Capteur Droite
const int AnalogPin1 = 1;
int AnalogValue1 = 4095;
//Cateur Gauche
const int AnalogPin2 = 2;
int AnalogValue2 = 4095;

// ---------------------------------------------------------------------

void setup() {
  // Initialisation de l'I2C
  Wire.begin(I2C_SDA, I2C_SCL);

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

  // Configurer les pins des moteurs comme sorties
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // Capteur IR
  pinMode(AnalogPin1, INPUT);
  pinMode(AnalogPin2, INPUT);

  dutyCycle = 255;
  pos = 90;
  // Configurer les fonctionnalités PWM
  ledcAttach(enable1Pin, freq, resolution);
  ledcAttach(enable2Pin, freq, resolution);

  ledcWrite(enable1Pin, dutyCycle);
  ledcWrite(enable2Pin, dutyCycle);
  
  // Déclaration de l'écran 
  //  !!! Si non déclaré : reset en boucle de l'esp et blue screen de l'ordinateur -> upload un code vierge !!!
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  delay(20);

  ESP32PWM::allocateTimer(6);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  display.println("ROBOT A2 A2I");
  display.display();

}

// ---------------------------------------------------------------------

void loop() {

  ultraAM = readSensor(trigPinAM, echoPinAM);
  ultraAG = readSensor(trigPinAG, echoPinAG);
  ultraAD = readSensor(trigPinAD, echoPinAD);
  //ultraCD = readSensor(trigPinCD, echoPinCD);
  //ultraCG = readSensor(trigPinCG, echoPinCG);

  AnalogValue1 = analogRead(AnalogPin1);
  AnalogValue2 = analogRead(AnalogPin2);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  affichageUltrason();
  affichageCycle();
  affichageIR();

  display.display();

  switch(etatRobot){
    case 0 : 
      avancer();
      break;


    default : break;

  }

/*
  if (ultraAM < 20){
    motorStop();
  } else {
    forward();
  }
*/
  myservo.write(pos);

}

// ---------------------------------------------------------------------

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

void affichageUltrason(){
// Affichage des distances des ultrasons sur l'écran arrières
  // ULtrason Avant
  display.setCursor(48, 0);
  display.println(ultraAM);
  
  // ULtrason Avant Gauche
  display.setCursor(0, 0);
  display.println(ultraAG);

  // ULtrason Avant Droite
  display.setCursor(96, 0);
  display.println(ultraAD);

  // ULtrason Arrière Gauche
  display.setCursor(0, 16);
  display.println(ultraCG);

  // Ultrason Arrière Droite
  display.setCursor(96, 16);
  display.println(ultraCD);

}


void affichageCycle(){
// Affichage du nombres de cycles et millisecondes par cycle
// Celà permet un débug le prog
  //Nombre de cycles
  display.setCursor(0, 48);
  display.print("Cycle : ");
  display.println(cycle);
  cycle = cycle + 1;

  //Milliseconde par cycle
  display.setCursor(100, 48);
  display.println(millis() - oldTime);
  oldTime = millis();
}

void affichageIR(){
// Affichage de la variable pos du servomoteur du cateur infrarouge
  display.setCursor(0, 32);
  display.print("Pos : ");
  display.println(pos);
  display.setCursor(64, 32);
  display.print(AnalogValue1);
  display.print("  ");
  display.println(AnalogValue2);
}

