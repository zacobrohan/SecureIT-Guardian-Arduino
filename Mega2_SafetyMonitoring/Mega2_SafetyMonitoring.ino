/*
  SecureIT Guardian – Mega 2 (Safety Monitoring Unit)
  Features:
  - PIR Motion Detection -> Motion LED ON + Buzzer ON
  - Flame Detection -> Fire LED ON (and can also sound buzzer if you want)
  - Serial monitor output for testing

  Pins:
  Motion Sensor:
    OUT -> 2
    VCC -> 5V
    GND -> GND

  Motion LED:
    LED + 220Ω -> 3
    LED -      -> GND

  Buzzer (Active):
    + -> 4
    - -> GND

  Flame Sensor (digital out mode):
    DO/OUT -> 6
    VCC    -> 5V (or 3.3V depending sensor)
    GND    -> GND

  Fire LED:
    LED + 220Ω -> 10
    LED -      -> GND
*/

// ------------ PINS ------------
#define MOTION_PIN   2
#define MOTION_LED   3
#define BUZZER_PIN   4

#define FLAME_PIN    6
#define FIRE_LED     10

void setup() {
  Serial.begin(9600);

  pinMode(MOTION_PIN, INPUT);
  pinMode(MOTION_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(FLAME_PIN, INPUT);
  pinMode(FIRE_LED, OUTPUT);

  digitalWrite(MOTION_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FIRE_LED, LOW);

  Serial.println("SecureIT Guardian – Mega2 READY");
  Serial.println("System Ready: Motion + Flame");
}

void loop() {
  // ----- MOTION -----
  int motion = digitalRead(MOTION_PIN);

  if (motion == HIGH) {
    digitalWrite(MOTION_LED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("👣 Motion Detected!");
    delay(300); // reduce spam
  } else {
    digitalWrite(MOTION_LED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // ----- FLAME -----
  int flame = digitalRead(FLAME_PIN);

  // Your sensor: fire = HIGH (1)
  if (flame == HIGH) {
    digitalWrite(FIRE_LED, HIGH);
    Serial.println("🔥 FIRE DETECTED");
    // If you also want buzzer for fire, uncomment:
    // digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(FIRE_LED, LOW);
  }

  delay(100);
}
