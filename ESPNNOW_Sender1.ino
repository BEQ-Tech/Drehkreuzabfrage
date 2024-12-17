
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x64, 0xe8, 0x33, 0x85, 0xc3, 0x7c};
esp_now_peer_info_t peerInfo;

// Pins for three buttons
const byte buttonPin1 = 13; //D13
const byte buttonPin2 = 12; //D12
const byte buttonPin3 = 14; //D14

byte lastButtonState1 = LOW;
byte lastButtonState2 = LOW;
byte lastButtonState3 = LOW;
byte buttonState1 = LOW;
byte buttonState2 = LOW;
byte buttonState3 = LOW;

const byte debounceTime = 30;
unsigned long lastButtonTimeChange = 0;

// Sender ID (each sender gets a unique ID)
const int senderID = 1; // Change this for each sender, e.g., 1, 2, 3

// Define data structure with three inputs
typedef struct struct_message {
  byte g;
  byte y;
  byte r;
  int senderID;  // Unique sender ID
} struct_message;

struct_message myData;

void OnDataSent(const unsigned char *mac_addr, esp_now_send_status_t status) {
  Serial.print("Data sent to: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(", Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Success");
  } else {
    Serial.println("Fail");
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize buttons as input with pull-up resistors
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false; 

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Send initial values of 0 for all buttons when starting up
  myData.g = 0; // Set button 1 state to 0 (not pressed)
  myData.y = 0; // Set button 2 state to 0 (not pressed)
  myData.r = 0; // Set button 3 state to 0 (not pressed)
  myData.senderID = senderID; // Add sender ID
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  Serial.println("Sent initial button states: 0 0 0");
}

void loop() {
  if (millis() - lastButtonTimeChange > debounceTime) {
    // Read button states
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
    buttonState3 = digitalRead(buttonPin3);

    // Check if button 1 state has changed
    if (buttonState1 != lastButtonState1) {
      lastButtonTimeChange = millis();
      lastButtonState1 = buttonState1;
      if (buttonState1 == LOW) {
        Serial.printf("Button 1 pressed!\n");
        myData.g = !buttonState1; // Toggle value for g
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      } else {
        Serial.printf("Button 1 released!\n");
        myData.g = !buttonState1; // Toggle value for g
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      }
    }

    // Check if button 2 state has changed
    if (buttonState2 != lastButtonState2) {
      lastButtonTimeChange = millis();
      lastButtonState2 = buttonState2;
      if (buttonState2 == LOW) {
        Serial.printf("Button 2 pressed!\n");
        myData.y = !buttonState2; // Toggle value for y
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      } else {
        Serial.printf("Button 2 released!\n");
        myData.y = !buttonState2; // Toggle value for y
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      }
    }

    // Check if button 3 state has changed
    if (buttonState3 != lastButtonState3) {
      lastButtonTimeChange = millis();
      lastButtonState3 = buttonState3;
      if (buttonState3 == LOW) {
        Serial.printf("Button 3 pressed!\n");
        myData.r = !buttonState3; // Toggle value for r
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      } else {
        Serial.printf("Button 3 released!\n");
        myData.r = !buttonState3; // Toggle value for r
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      }
    }
  }
}
