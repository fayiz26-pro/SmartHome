#define LED_PIN 5        // D1
#define BUZZER_PIN 4     // D2
#define TRIG_PIN 14      // D5
#define ECHO_PIN 12      // D6
#define LDR_PIN A0       // Analog pin

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distanceCm = duration * 0.034 / 2;
  return distanceCm;
}

void loop() {
  // --- LDR ---
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR: ");
  Serial.println(ldrValue);

  if (ldrValue < 500) {
    digitalWrite(LED_PIN, HIGH); // Gelap → LED nyala
  } else {
    digitalWrite(LED_PIN, LOW);  // Terang → LED mati
  }

  // --- Ultrasonik ---
  long jarak = readUltrasonicDistance();
  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");

  if (jarak > 0 && jarak < 50) {
    digitalWrite(BUZZER_PIN, HIGH); // Orang dekat → alarm nyala
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Aman
  }

  delay(500);
}