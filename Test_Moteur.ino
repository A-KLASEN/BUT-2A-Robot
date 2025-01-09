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

void setup() {
  // Configurer les pins des moteurs comme sorties
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // Configurer les fonctionnalités PWM
  ledcAttach(enable1Pin, freq, resolution);
  ledcAttach(enable2Pin, freq, resolution);
  // Activer PWM
  ledcWrite(enable1Pin, dutyCycle);
  ledcWrite(enable2Pin, dutyCycle);

  // Marche Avant
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);

  // Marche Arrière
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(1000);

  // Tourne sur place vers la droite
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(1000);

  // Tourne sur place vers gauche
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);

  // Arreter les moteurs
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);

}

void loop() {

}
