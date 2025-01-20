// Configuration des moteurs
int motor1Pin1 = 15; // Pin du moteur 1, direction 1

// Paramètres PWM
const int freq = 30000;       // Fréquence PWM
const int resolution = 8;     // Résolution en bits
int dutyCycle = 191;          // Rapport cyclique (0-255)

void setup() {

  // Configurer les pins des moteurs comme sorties
  pinMode(motor1Pin1, OUTPUT);
  // Configurer les fonctionnalités PWM
  ledcAttach(motor1Pin1, freq, resolution);
  // Activer PWM
  ledcWrite(motor1Pin1, dutyCycle);

}

void loop() {

}
