// I2C https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/
// WIFI-NOW https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x24, 0x58, 0x7c, 0xda, 0x2d, 0x44};
//24:58:7c:da:2d:44
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define I2C_SDA 17
#define I2C_SCL 16
const int AnalogPin = 34;
int AnalogValue = 4095;


void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(115200);


  delay(10);



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


}

void loop() {

  // Reading potentiometer value
  AnalogValue = analogRead(AnalogPin);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(16, 0);
  // Display static text

  if (AnalogValue < 100)   display.println("Signal recu");
  else if (AnalogValue > 4000)   display.println("Signal non recu");
  display.display();

}
