#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>  // Servo library

// Slot booking flags
bool slot1Booked = false;
bool slot2Booked = false;

// Pin Definitions for Slots
const int trig1 = 13, echo1 = 12;
const int trig2 = 15, echo2 = 2;

const int red1 = 27, green1 = 26, blue1 = 25;
const int red2 = 16, green2 = 17, blue2 = 5;

const int buzzer1 = 14;
const int buzzer2 = 4;

// Gate pins
const int gateTrig = 33;
const int gateEcho = 32;
const int gateRedLED = 18;
const int gateGreenLED = 19;

Servo gateServo;
const int gateServoPin = 23;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Timing for buzzer
unsigned long lastBeepTime1 = 0;
int beepInterval1 = 600;

unsigned long lastBeepTime2 = 0;
int beepInterval2 = 600;

// Gate timing
bool gateOpen = false;
unsigned long gateOpenTime = 0;
const unsigned long gateOpenDuration = 5000;
unsigned long lastGateCheck = 0;
const unsigned long gateCheckInterval = 100;

// LCD state
enum SlotStatus { FREE, BOOKED, OCCUPIED, UNKNOWN };
SlotStatus prevSlot1Status = UNKNOWN;
SlotStatus prevSlot2Status = UNKNOWN;
bool prevGateOpen = false;
bool prevNoBookingMessage = false;

// Distance helper
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

// RGB LED control
void setLED(int r, int g, int b, bool R, bool G, bool B) {
  digitalWrite(r, R);
  digitalWrite(g, G);
  digitalWrite(b, B);
}

// Buzzer handler
void handleBuzzer(int buzzerPin, long distance, unsigned long &lastBeepTime, int &beepInterval) {
  unsigned long now = millis();
  if (distance < 0) {
    digitalWrite(buzzerPin, LOW);
    return;
  }

  if (distance > 20) {
    beepInterval = 600;
    digitalWrite(buzzerPin, LOW);
    return;
  } else if (distance > 15) {
    beepInterval = 400;
  } else if (distance > 10) {
    beepInterval = 200;
  } else if (distance > 5) {
    beepInterval = 100;
  } else {
    digitalWrite(buzzerPin, HIGH);
    return;
  }

  if (now - lastBeepTime >= (unsigned long)beepInterval) {
    lastBeepTime = now;
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
  }
}

bool isAnyBookedSlotFree() {
  long d1 = getDistance(trig1, echo1);
  long d2 = getDistance(trig2, echo2);
  return (slot1Booked && (d1 < 0 || d1 > 15)) || (slot2Booked && (d2 < 0 || d2 > 15));
}

void updateSlotLCD(int slotNum, SlotStatus currentStatus) {
  if (slotNum == 1 && currentStatus != prevSlot1Status) {
    lcd.setCursor(0, 0);
    if (currentStatus == FREE) lcd.print("Slot1: Free     ");
    else if (currentStatus == BOOKED) lcd.print("Slot1: Booked   ");
    else if (currentStatus == OCCUPIED) lcd.print("Slot1: Occupied ");
    prevSlot1Status = currentStatus;
  } else if (slotNum == 2 && currentStatus != prevSlot2Status) {
    lcd.setCursor(0, 1);
    if (currentStatus == FREE) lcd.print("Slot2: Free     ");
    else if (currentStatus == BOOKED) lcd.print("Slot2: Booked   ");
    else if (currentStatus == OCCUPIED) lcd.print("Slot2: Occupied ");
    prevSlot2Status = currentStatus;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(trig1, OUTPUT); pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT); pinMode(echo2, INPUT);
  pinMode(red1, OUTPUT); pinMode(green1, OUTPUT); pinMode(blue1, OUTPUT);
  pinMode(red2, OUTPUT); pinMode(green2, OUTPUT); pinMode(blue2, OUTPUT);
  pinMode(buzzer1, OUTPUT); pinMode(buzzer2, OUTPUT);
  pinMode(gateTrig, OUTPUT); pinMode(gateEcho, INPUT);
  pinMode(gateRedLED, OUTPUT); pinMode(gateGreenLED, OUTPUT);

  digitalWrite(gateRedLED, HIGH);
  digitalWrite(gateGreenLED, LOW);

  gateServo.attach(gateServoPin, 500, 2400);
  gateServo.write(0);

  lcd.init();
  lcd.backlight();
  setLED(red1, green1, blue1, LOW, HIGH, LOW);
  setLED(red2, green2, blue2, LOW, HIGH, LOW);

  lcd.setCursor(0, 0); lcd.print("Slot1: Free     ");
  lcd.setCursor(0, 1); lcd.print("Slot2: Free     ");
  prevSlot1Status = FREE;
  prevSlot2Status = FREE;
  prevGateOpen = false;
  prevNoBookingMessage = false;
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input == "book 1") { slot1Booked = true; Serial.println("Slot 1 Booked"); }
    else if (input == "book 2") { slot2Booked = true; Serial.println("Slot 2 Booked"); }
    else if (input == "release 1") { slot1Booked = false; Serial.println("Slot 1 Released"); }
    else if (input == "release 2") { slot2Booked = false; Serial.println("Slot 2 Released"); }
  }

  unsigned long currentMillis = millis();

  if (currentMillis - lastGateCheck >= gateCheckInterval) {
    lastGateCheck = currentMillis;
    long gateDist = getDistance(gateTrig, gateEcho);
    bool canOpenGate = (slot1Booked || slot2Booked) && isAnyBookedSlotFree();

    if (gateDist > 0 && gateDist < 10 && canOpenGate) {
      if (!gateOpen) {
        
        gateServo.write(90);
        digitalWrite(gateRedLED, LOW);
        digitalWrite(gateGreenLED, HIGH);
        gateOpen = true;
        gateOpenTime = currentMillis;
        Serial.println("Gate opened");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gate Opening...");
        prevGateOpen = true;
        prevNoBookingMessage = false;
      }
    } else if (!canOpenGate && !prevNoBookingMessage && !gateOpen) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No booking      ");
      prevNoBookingMessage = true;
      prevGateOpen = false;
    }

    if (gateOpen && (currentMillis - gateOpenTime > gateOpenDuration)) {
      gateServo.write(0);
      digitalWrite(gateRedLED, HIGH);
      digitalWrite(gateGreenLED, LOW);
      gateOpen = false;
      Serial.println("Gate closed");

      lcd.clear();
      updateSlotLCD(1, prevSlot1Status);
      updateSlotLCD(2, prevSlot2Status);
      prevGateOpen = false;
    }
  }

  // --- SLOT 1 LOGIC ---
  long dist1 = getDistance(trig1, echo1);
  SlotStatus currentSlot1Status;
  if (!slot1Booked) currentSlot1Status = FREE;
  else if (dist1 < 0 || dist1 > 15) currentSlot1Status = BOOKED;
  else currentSlot1Status = OCCUPIED;

  if (currentSlot1Status != prevSlot1Status) {
    if (currentSlot1Status == FREE) setLED(red1, green1, blue1, LOW, HIGH, LOW);
    else if (currentSlot1Status == BOOKED) setLED(red1, green1, blue1, LOW, LOW, HIGH);
    else if (currentSlot1Status == OCCUPIED) setLED(red1, green1, blue1, HIGH, LOW, LOW);
    updateSlotLCD(1, currentSlot1Status);
    prevSlot1Status = currentSlot1Status;
  }

  if (!slot1Booked || dist1 < 0 || dist1 > 15) {
    digitalWrite(buzzer1, LOW);
    lastBeepTime1 = currentMillis;
  } else {
    if (dist1 <= 5) {
      static unsigned long occupiedStart1 = 0;
      static bool timingStarted1 = false;
      if (!timingStarted1) {
        occupiedStart1 = currentMillis;
        timingStarted1 = true;
      }
      if (currentMillis - occupiedStart1 <= 10000) digitalWrite(buzzer1, HIGH);
      else digitalWrite(buzzer1, LOW);
    } else {
      static bool timingStarted1 = false;
      timingStarted1 = false;
      handleBuzzer(buzzer1, dist1, lastBeepTime1, beepInterval1);
    }
  }

  // --- SLOT 2 LOGIC ---
  long dist2 = getDistance(trig2, echo2);
  SlotStatus currentSlot2Status;
  if (!slot2Booked) currentSlot2Status = FREE;
  else if (dist2 < 0 || dist2 > 15) currentSlot2Status = BOOKED;
  else currentSlot2Status = OCCUPIED;

  if (currentSlot2Status != prevSlot2Status) {
    if (currentSlot2Status == FREE) setLED(red2, green2, blue2, LOW, HIGH, LOW);
    else if (currentSlot2Status == BOOKED) setLED(red2, green2, blue2, LOW, LOW, HIGH);
    else if (currentSlot2Status == OCCUPIED) setLED(red2, green2, blue2, HIGH, LOW, LOW);
    updateSlotLCD(2, currentSlot2Status);
    prevSlot2Status = currentSlot2Status;
  }

  if (!slot2Booked || dist2 < 0 || dist2 > 15) {
    digitalWrite(buzzer2, LOW);
    lastBeepTime2 = currentMillis;
  } else {
    if (dist2 <= 5) {
      static unsigned long occupiedStart2 = 0;
      static bool timingStarted2 = false;
      if (!timingStarted2) {
        occupiedStart2 = currentMillis;
        timingStarted2 = true;
      }
      if (currentMillis - occupiedStart2 <= 10000) digitalWrite(buzzer2, HIGH);
      else digitalWrite(buzzer2, LOW);
    } else {
      static bool timingStarted2 = false;
      timingStarted2 = false;
      handleBuzzer(buzzer2, dist2, lastBeepTime2, beepInterval2);
    }
  }

  delay(50);  // For stability
}
