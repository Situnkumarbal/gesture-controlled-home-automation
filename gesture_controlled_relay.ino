/**
 * Gesture-Controlled Home Automation System
 * -----------------------------------------
 * Controls two relays based on hand gesture input received via Serial communication.
 * Each gesture controls specific relays, with debounce delay for reliable switching.
 * LED indicators show finger states: Thumb, Index, Middle, Ring, Pinky
 * 
 * Author: Situn Kumar Bal
 * Year: 2025
 */

// === LED Pin Assignments ===
#define THUMB_LED   7
#define INDEX_LED   3
#define MIDDLE_LED  4
#define RING_LED    5
#define PINKY_LED   6

// === Relay Pin Assignments ===
#define RELAY1      8
#define RELAY2      9

// === Relay Timing Variables ===
unsigned long relay1OnTime = 0;
unsigned long relay1OffTime = 0;
bool relay1State = false;

unsigned long relay2OnTime = 0;
unsigned long relay2OffTime = 0;
bool relay2State = false;

void setup() {
  Serial.begin(9600);

  // Set LED pins as output
  pinMode(THUMB_LED, OUTPUT);
  pinMode(INDEX_LED, OUTPUT);
  pinMode(MIDDLE_LED, OUTPUT);
  pinMode(RING_LED, OUTPUT);
  pinMode(PINKY_LED, OUTPUT);

  // Set Relay pins as output
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Ensure relays are OFF initially
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    Serial.println("Received: " + data);

    // Parse finger data (expects 5 bits: 0 or 1)
    int fingers[5] = {0}; // Thumb, Index, Middle, Ring, Pinky
    int idx = 0;
    for (int i = 0; i < data.length(); i++) {
      if (data[i] == '0' || data[i] == '1') {
        if (idx < 5) fingers[idx++] = data[i] - '0';
      }
    }

    // Update LED indicators
    digitalWrite(THUMB_LED, fingers[0]);
    digitalWrite(INDEX_LED, fingers[1]);
    digitalWrite(MIDDLE_LED, fingers[2]);
    digitalWrite(RING_LED, fingers[3]);
    digitalWrite(PINKY_LED, fingers[4]);

    unsigned long currentMillis = millis();

    // === Relay 1 Trigger Condition: 01100 ===
    if (fingers[0]==0 && fingers[1]==1 && fingers[2]==1 && fingers[3]==0 && fingers[4]==0) {
      relay1OffTime = 0;
      if (!relay1State && relay1OnTime == 0) relay1OnTime = currentMillis;
      if (!relay1State && (currentMillis - relay1OnTime >= 1500)) {
        digitalWrite(RELAY1, HIGH);
        relay1State = true;
        Serial.println("Relay 1 ON");
      }
    } else {
      relay1OnTime = 0;
      if (relay1State && relay1OffTime == 0) relay1OffTime = currentMillis;
      if (relay1State && (currentMillis - relay1OffTime >= 2500)) {
        digitalWrite(RELAY1, LOW);
        relay1State = false;
        Serial.println("Relay 1 OFF");
      }
    }

    // === Relay 2 Trigger Condition: 11000 ===
    if (fingers[0]==1 && fingers[1]==1 && fingers[2]==0 && fingers[3]==0 && fingers[4]==0) {
      relay2OffTime = 0;
      if (!relay2State && relay2OnTime == 0) relay2OnTime = currentMillis;
      if (!relay2State && (currentMillis - relay2OnTime >= 1500)) {
        digitalWrite(RELAY2, HIGH);
        relay2State = true;
        Serial.println("Relay 2 ON");
      }
    } else {
      relay2OnTime = 0;
      if (relay2State && relay2OffTime == 0) relay2OffTime = currentMillis;
      if (relay2State && (currentMillis - relay2OffTime >= 2500)) {
        digitalWrite(RELAY2, LOW);
        relay2State = false;
        Serial.println("Relay 2 OFF");
      }
    }
  }
}
