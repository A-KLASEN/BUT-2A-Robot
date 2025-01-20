#include <ESP32Servo.h>
Servo myservo;

//servo moteur
int servoPin = 15;
int pos = 180;    // variable to store the servo position

void setup() {
 
  ESP32PWM::allocateTimer(6);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin); // attaches the servo on pin 15 to the servo object
}

// ---------------------------------------------------------------------

void loop() {

myservo.write(0);
delay(1000);
myservo.write(90);
delay(1000);
myservo.write(180);
delay(1000);

}
