const int soilMoisturePin = A0;
const int relayPin = 5; // D1

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // pastikan relay OFF awalnya

  Serial.println("Monitoring Sensor Kelembaban Tanah");
  Serial.println("----------------------------------");
}

void loop() {
  int moistureValue = analogRead(soilMoisturePin);

  Serial.print("Nilai sensor kelembaban: ");
  Serial.println(moistureValue);

  if (moistureValue > 700) {
    Serial.println("Status: Sangat Kering! Pompa ON");
    digitalWrite(relayPin, HIGH); // Pompa hidup
  } else {
    Serial.println("Status: Tanah cukup lembab. Pompa OFF");
    digitalWrite(relayPin, LOW); // Pompa mati
  }

  delay(2000);
}
