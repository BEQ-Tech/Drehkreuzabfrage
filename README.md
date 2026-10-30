# Drehkreuzabfrage mit ESP-NOW

Eine drahtlose Statusabfrage für bis zu **drei Drehkreuze bzw. Eingabestationen** auf Basis von ESP32-Mikrocontrollern und **ESP-NOW**.

Jede Station besitzt drei digitale Eingänge. Ein ESP32-Sender überträgt deren Zustände an einen zentralen ESP32-Empfänger, der sie über **neun LED-Ausgänge** darstellt. Die Signale sind im Code als **Grün (G)**, **Gelb (Y)** und **Rot (R)** bezeichnet.

## Funktionen

- Bis zu **3 Sender** mit je **3 digitalen Eingängen**
- **1 zentraler Empfänger** mit **9 LED-Ausgängen**
- Direkte Funkübertragung über **ESP-NOW** (ohne WLAN-Router oder Internetverbindung)
- Eindeutige Zuordnung anhand der Sender-IDs **1, 2 und 3**
- Übertragung beim Wechsel eines Tasterzustands sowie einmalig beim Start des Senders
- Software-Entprellung mit **30 ms** im Sendercode
- Status- und Diagnoseausgaben über die serielle Schnittstelle (**115200 Baud**)

## Aufbau

```text
Drehkreuz / Station 1              ┌────────────────────────────┐
  3 Eingänge ── ESP32 Sender 1 ─ ─ ┤                            │
                                   │                            │
Drehkreuz / Station 2              │   ESP-NOW (2,4 GHz)        │
  3 Eingänge ── ESP32 Sender 2 ─ ─ ┤                            ├── ESP32 Empfänger
                                   │                            │       │
Drehkreuz / Station 3              │                            │       └── 9 LED-Ausgänge
  3 Eingänge ── ESP32 Sender 3 ─ ─ ┤                            │           (3 × G/Y/R)
                                   └────────────────────────────┘
```

## Benötigte Hardware und Software

- **4 ESP32-kompatible Boards**: drei Sender und ein Empfänger; die verfügbaren GPIOs müssen zur Pinbelegung passen
- **9 Taster oder potentialfreie Kontakte** für die Sender (je drei pro Sender)
- **9 LEDs** samt geeigneten Vorwiderständen oder entsprechend geeignete Treiber/Anzeigen am Empfänger
- Passende Stromversorgung und Verbindungskabel
- **Arduino IDE** mit installiertem Boardpaket **„esp32“ von Espressif Systems**

Die verwendeten Bibliotheken `WiFi.h`, `esp_now.h` und `esp_wifi.h` sind Bestandteil des ESP32-Arduino-Umfelds. Zusätzliche Bibliotheken sind im Repository nicht angegeben.

## Dateien

| Datei | Funktion |
| --- | --- |
| `ESPNNOW_Sender1.ino` | Sender für Station 1 (`senderID = 1`) |
| `ESPNNOW_Sender2.ino` | Sender für Station 2 (`senderID = 2`) |
| `ESPNNOW_Sender3.ino` | Sender für Station 3 (`senderID = 3`) |
| `ESPNNOW_Empfaenger.ino` | Zentraler Empfänger mit neun LED-Ausgängen |
| `Get_MAC_Adress.ino` | Gibt die WLAN-STA-MAC-Adresse des ESP32 über die serielle Schnittstelle aus |

## Pinbelegung

### Sender 1, 2 und 3

Alle drei Sender verwenden dieselbe Eingangskonfiguration:

| Eingang | GPIO | Übertragener Wert |
| --- | ---: | --- |
| Taster 1 | 13 | `g` (Grün) |
| Taster 2 | 12 | `y` (Gelb) |
| Taster 3 | 14 | `r` (Rot) |

Die Eingänge sind als `INPUT_PULLUP` konfiguriert. Deshalb wird ein Taster/Kontakt zwischen dem jeweiligen GPIO und **GND** angeschlossen:

- **Kontakt geschlossen / GPIO LOW** → gesendeter Wert `1`
- **Kontakt offen / GPIO HIGH** → gesendeter Wert `0`

### Empfänger

Der Empfänger setzt den jeweiligen GPIO bei einem empfangenen Wert von `1` auf **HIGH**, sonst auf **LOW**.

| Sender | Grün (`g`) | Gelb (`y`) | Rot (`r`) |
| --- | ---: | ---: | ---: |
| 1 | GPIO 6 | GPIO 7 | GPIO 21 |
| 2 | GPIO 3 | GPIO 4 | GPIO 5 |
| 3 | GPIO 10 | GPIO 8 | GPIO 20 |

Für direkt angeschlossene, aktiv-HIGH betriebene LEDs ist ein geeigneter Vorwiderstand erforderlich. Die konkrete Beschaltung hängt vom verwendeten ESP32-Board und der angeschlossenen Anzeige ab.

## Installation und Inbetriebnahme

### 1. Repository herunterladen

```bash
git clone https://github.com/BEQ-Tech/Drehkreuzabfrage.git
```

Alternativ über GitHub **Code → Download ZIP** herunterladen.

### 2. MAC-Adresse des Empfängers ermitteln

1. In der Arduino IDE `Get_MAC_Adress.ino` öffnen und auf das **Empfänger-Board** laden.
2. Seriellen Monitor auf **115200 Baud** einstellen.
3. Die angezeigte WLAN-STA-MAC-Adresse notieren, z. B. `AA:BB:CC:DD:EE:FF`.

### 3. Empfänger programmieren

1. `ESPNNOW_Empfaenger.ino` öffnen.
2. Die verwendeten LED-GPIOs und deren Verdrahtung prüfen und gegebenenfalls anpassen.
3. Sketch auf den Empfänger laden.
4. Seriellen Monitor bei **115200 Baud** öffnen.

### 4. Sender konfigurieren und programmieren

In **allen drei** Sender-Dateien die vorgegebene Empfänger-MAC durch die zuvor ermittelte MAC-Adresse ersetzen:

```cpp
// Beispieladresse: durch die eigene Empfänger-MAC ersetzen!
uint8_t broadcastAddress[] = {
  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};
```

Die Sender-IDs sind bereits passend vorbelegt:

```cpp
const int senderID = 1;  // In Sender2: 2, in Sender3: 3
```

Danach jeden Sketch auf das entsprechende Sender-Board laden und die Taster/Kontakte anschliessen.

### 5. Funktion prüfen

1. Zuerst den Empfänger einschalten.
2. Danach die drei Sender einschalten.
3. Einen der Eingänge gegen GND schalten: Die zugehörige LED am Empfänger sollte angehen.
4. Den Eingang wieder öffnen: Die LED sollte ausgehen.
5. Bei Problemen die seriellen Ausgaben auf Sender und Empfänger prüfen.

Bei der Arduino IDE kann es nötig sein, jeden `.ino`-Sketch in einem gleichnamigen Sketch-Ordner zu öffnen bzw. zu speichern.

## So funktioniert die Übertragung

Sender und Empfänger verwenden dieselbe Datenstruktur:

```cpp
typedef struct struct_message {
  byte g;
  byte y;
  byte r;
  int senderID;
} struct_message;
```

- `g`, `y`, `r`: Statuswerte `0` oder `1`
- `senderID`: Zuordnung des Datenpakets zu einer der drei Stationen

Beim Erkennen einer Zustandsänderung übermittelt ein Sender **alle drei aktuellen Statuswerte** an den Empfänger. Dieser speichert die Werte des betreffenden Senders und aktualisiert die LED-Ausgänge für alle drei Stationen.

Die Nachrichten werden im vorhandenen Code mit `peerInfo.encrypt = false` **unverschlüsselt** übertragen. Der Peer ist mit `peerInfo.channel = 0` konfiguriert; für eine zuverlässige ESP-NOW-Verbindung müssen die Geräte denselben Funkkanal verwenden.

## Fehlerbehebung

| Problem | Mögliche Ursache / Prüfung |
| --- | --- |
| Keine LED reagiert | Empfänger-MAC in allen Sendern, Stromversorgung und Funkkanal prüfen |
| Nur eine Station funktioniert | Korrekte `senderID` (`1`, `2` oder `3`) und Flash-Datei prüfen |
| Falsche LED leuchtet | GPIO-Zuordnung und Reihenfolge `g`, `y`, `r` am Empfänger prüfen |
| LED reagiert invertiert | Eingänge sind aktiv-LOW, Ausgänge aktiv-HIGH; Verdrahtung prüfen |
| `Failed to add peer` | Peer-Konfiguration und ESP-NOW-Initialisierung prüfen |
| Übertragung schlägt fehl | Empfangsgerät eingeschaltet? MAC-Adresse richtig? Gleicher WLAN-Kanal? |
| Sketch kompiliert nicht | Kompatibilität der ESP-NOW-Callback-Signaturen mit der installierten ESP32-Boardpaket-Version prüfen |

## Bekannte Einschränkungen

Diese Version ist eine **einfache Statusübertragung**, kein vollständig ausfallsicheres Überwachungssystem:

- Es gibt **keinen periodischen Heartbeat**, keine automatische Offline-Erkennung und keinen Timeout für veraltete LED-Zustände. Fällt ein Sender aus, kann der letzte Status am Empfänger stehen bleiben.
- Die Sender übertragen Änderungen und einen einmaligen Initialzustand beim Start. **Verlorene Pakete werden nicht automatisch erneut übertragen.**
- Ein bereits beim Einschalten gedrückt gehaltener Taster wird aufgrund der Startinitialisierung möglicherweise erst nach einem Zustandswechsel korrekt erkannt.
- Die ESP-NOW-Kommunikation ist im veröffentlichten Stand **nicht verschlüsselt**. Eine Authentifizierung der Sender anhand ihrer MAC-Adresse ist im Empfängercode nicht implementiert; die Zuordnung erfolgt über die übertragene `senderID`.
- Der Empfänger prüft vor `memcpy` die **empfangene Paketlänge nicht**. Sender und Empfänger müssen deshalb eine kompatible Datenstruktur verwenden; für robusten Betrieb sollte eine Längenprüfung ergänzt werden.
- Es sind weder Durchgangszählung noch eine Freigabe- oder Sicherheitslogik für Drehkreuze implementiert.
