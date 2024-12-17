
#include <esp_now.h>
#include <WiFi.h>

// Pins for nine LEDs
const byte LEDPins[] = {3, 4, 5, 6, 7, 21, 10, 9, 20};  // Array with LED pins

// Define data structure with 9 inputs (for 9 LEDs)
typedef struct struct_message {
  byte g1;
  byte y1;
  byte r1;
  byte g2;
  byte y2;
  byte r2;
  byte g3;
  byte y3;
  byte r3;
} struct_message;

struct_message myData;

void setup() {
  Serial.begin(115200);

  // Set LED pins as OUTPUT
  for (int i = 0; i < 9; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  // Hier kannst du auf die MAC-Adresse des sendenden Geräts zugreifen
  const uint8_t *mac = recv_info->des_addr;

  // Kopiere die empfangenen Daten in die Struktur
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Ausgabe der empfangenen Daten zur Fehlerbehebung
  Serial.println("Received Data:");
  Serial.printf("g1: %d, y1: %d, r1: %d, g2: %d, y2: %d, r2: %d, g3: %d, y3: %d, r3: %d\n", 
    myData.g1, myData.y1, myData.r1, myData.g2, myData.y2, myData.r2, myData.g3, myData.y3, myData.r3);

  // Control LEDs based on received data
  digitalWrite(LEDPins[0], myData.g1 == 1 ? HIGH : LOW);
  digitalWrite(LEDPins[1], myData.y1 == 1 ? HIGH : LOW);
  digitalWrite(LEDPins[2], myData.r1 == 1 ? HIGH : LOW);
  digitalWrite(LEDPins[3], myData.g2 == 1 ? HIGH : LOW);
  digitalWrite(LEDPins[4], myData.y2 == 1 ? HIGH : LOW);
  digitalWrite(LEDPins[5], myData.r2 == 1 ? HIGH : LOW);
  digitalWrite(LEDPins[6], myData.g3 == 1 ? HIGH : LOW);
  digitalWrite(LEDPins[7], myData.y3 == 1 ? HIGH : LOW);
  digitalWrite(LEDPins[8], myData.r3 == 1 ? HIGH : LOW);
}

void loop() {
  // No specific loop functionality, as LEDs are controlled by received data
}