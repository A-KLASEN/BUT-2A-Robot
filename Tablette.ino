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
const int potPin = 34;
int potValue = 100;
const int GF = 5;
const int GB = 18;
const int DF = 19;
const int DB = 21;
int buttonState_GF = digitalRead(GF);
int buttonState_GB = digitalRead(GB);
int buttonState_DF = digitalRead(DF);
int buttonState_DB = digitalRead(DB);

const int trigPinAM = 2;
const int echoPinAM= 4;
float ultra = 0;
#define SOUND_SPEED 0.034
long duration;
float distanceCm;

//ESP NOW
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  
  int a;
  int b;
  int c;
  int d;
  int v;

} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}




void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(potPin, INPUT); // config GPIO21 as input pin and enable the internal pull-up resistor
  pinMode(GF, INPUT_PULLUP); // config GPIO21 as input pin and enable the internal pull-up resistor
  pinMode(GB, INPUT_PULLUP); // config GPIO21 as input pin and enable the internal pull-up resistor
  pinMode(DF, INPUT_PULLUP); // config GPIO21 as input pin and enable the internal pull-up resistor
  pinMode(DB, INPUT_PULLUP); // config GPIO21 as input pin and enable the internal pull-up resistor

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
  potValue = analogRead(potPin);
  Serial.println(potValue);

  buttonState_GF = digitalRead(GF);
  buttonState_GB = digitalRead(GB);
  buttonState_DF = digitalRead(DF);
  buttonState_DB = digitalRead(DB);



  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32, 0);
  // Display static text
  display.println(potValue);

  display.setCursor(64, 0);
  display.println(myData.v);
  
  display.setCursor(32, 16);
  display.println(buttonState_GF);
  display.setCursor(32, 32);
  display.println(buttonState_GB);

  display.setCursor(64, 16);
  display.println(buttonState_DF);
  display.setCursor(64, 32);
  display.println(buttonState_DB);


  display.display();

  // Set values to send
  myData.a = buttonState_GF;
  myData.b = buttonState_GB;
  myData.c = buttonState_DF;
  myData.d = buttonState_DB;
  myData.v = map(potValue, 0, 4095, 0, 255);
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2);

}
