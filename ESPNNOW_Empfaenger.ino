#include <esp_now.h>
#include <WiFi.h>

// Pins für neun LEDs
const byte LEDPins[] = {6, 7, 21, 3, 4, 5, 10, 8, 20};  // Array mit LED Pins

// Erweiterte Datenstruktur mit 9 Eingabewerten und Sender-ID
typedef struct struct_message {
  byte g;
  byte y;
  byte r;
  int senderID;  // Sender-ID
} struct_message;

// Array zum Speichern der LED-Zustände (0 = OFF, 1 = ON)
int ledStates[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // Global deklarieren

struct_message myData;

void setup() {
  Serial.begin(115200);

  // Setze die LED-Pins auf OUTPUT
  for (int i = 0; i < 9; i++) {
    pinMode(LEDPins[i], OUTPUT);
    digitalWrite(LEDPins[i], LOW);  // Alle LEDs auf LOW setzen
  }

  // Setze alle Eingabewerte in myData auf 0
  myData.g = 0;
  myData.y = 0;
  myData.r = 0;
  myData.senderID = -1;  // Unbekannte Sender-ID initialisieren

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);  // Callback-Funktion für empfangene Daten registrieren
}

// Callback-Funktion, die ausgeführt wird, wenn Daten empfangen werden
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  // Kopiere die empfangenen Daten in die Struktur
  memcpy(&myData, incomingData, sizeof(myData));

  // Ausgabe der empfangenen Daten zur Fehlerbehebung
  Serial.println("Received Data:");
  Serial.printf("Sender ID: %d\n", myData.senderID);  // Zeigt die Sender-ID an
  Serial.printf("g: %d, y: %d, r: %d\n", 
    myData.g, myData.y, myData.r);

  // LEDs basierend auf den empfangenen Daten steuern
  if (myData.senderID == 1) {  // Falls der Sender 1 ist
    ledStates[0] = myData.g;
    ledStates[1] = myData.y;
    ledStates[2] = myData.r;
  }
  else if (myData.senderID == 2) {  // Falls der Sender 2 ist
    ledStates[3] = myData.g;
    ledStates[4] = myData.y;
    ledStates[5] = myData.r;
  }
  else if (myData.senderID == 3) {  // Falls der Sender 3 ist
    ledStates[6] = myData.g;
    ledStates[7] = myData.y;
    ledStates[8] = myData.r;
  }

  // Steuerung der LEDs basierend auf den gespeicherten Zuständen
  for (int i = 0; i < 9; i++) {
    digitalWrite(LEDPins[i], ledStates[i] == 1 ? HIGH : LOW);
  }
}

void loop() {
  // Keine spezifische Funktionalität im Loop, da die LEDs durch empfangene Daten gesteuert werden
}
