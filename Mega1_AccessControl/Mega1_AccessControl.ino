/*
  SecureIT Guardian – Mega 1 (Access Control Unit)
  Features:
  - RC522 RFID Authentication
  - Servo Door Lock (open 90° for valid card)
  - Door LED (ON when door open)
  - LCD 16x2 messages (Scan / Granted / Denied / Closed)

  Pins:
  RC522 (SPI):
    SDA/SS -> 53
    SCK    -> 52
    MOSI   -> 51
    MISO   -> 50
    RST    -> 9
    3.3V   -> 3.3V
    GND    -> GND

  Servo:
    Signal -> 7
    VCC    -> 5V (external recommended)
    GND    -> GND

  Door LED:
    LED + 220Ω -> 8
    LED -      -> GND

  LCD (16x2, 4-bit mode):
    RS -> 30
    E  -> 31
    D4 -> 32
    D5 -> 33
    D6 -> 34
    D7 -> 35
    RW -> GND
    VSS -> GND
    VDD -> 5V
    VO  -> Potentiometer middle pin (contrast)
    A (backlight +) -> 5V via 220Ω
    K (backlight -) -> GND
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// ----- RFID -----
#define SS_PIN 53
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ----- SERVO -----
Servo doorServo;
#define SERVO_PIN 7

// ----- LED -----
#define DOOR_LED 8

// ----- LCD -----
LiquidCrystal lcd(30, 31, 32, 33, 34, 35);

// ----- AUTHORIZED UID -----
String allowedUID = "0C:38:C7:33";   // Replace with your real UID

String getUID() {
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidStr += "0";
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
    if (i != mfrc522.uid.size - 1) uidStr += ":";
  }
  uidStr.toUpperCase();
  return uidStr;
}

void showLCD(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1.substring(0, 16));
  lcd.setCursor(0, 1);
  lcd.print(line2.substring(0, 16));
}

void setup() {
  Serial.begin(9600);

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(DOOR_LED, OUTPUT);
  digitalWrite(DOOR_LED, LOW);

  doorServo.attach(SERVO_PIN);
  doorServo.write(0);   // Door closed position

  lcd.begin(16, 2);

  showLCD("SecureIT Guardian", "Scan your card");
  Serial.println("Mega1 READY: RFID + Servo + LCD");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String scannedUID = getUID();

  Serial.print("Scanned UID: ");
  Serial.println(scannedUID);

  if (scannedUID == allowedUID) {
    // ACCESS GRANTED
    showLCD("Access Granted", "Door Opening...");
    digitalWrite(DOOR_LED, HIGH);
    doorServo.write(90);       // Open door

    delay(3000);

    doorServo.write(0);        // Close door
    digitalWrite(DOOR_LED, LOW);

    showLCD("Door Closed", "Scan Next Card");
    delay(1000);
    showLCD("SecureIT Guardian", "Scan your card");
  } else {
    // ACCESS DENIED
    showLCD("Access Denied", "Invalid Card");

    // Blink LED 3 times
    for (int i = 0; i < 3; i++) {
      digitalWrite(DOOR_LED, HIGH);
      delay(200);
      digitalWrite(DOOR_LED, LOW);
      delay(200);
    }

    showLCD("SecureIT Guardian", "Scan your card");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
