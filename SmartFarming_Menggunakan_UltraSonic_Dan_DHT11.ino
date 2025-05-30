#include <DHT.h>

// Pin Setup
#define DHTPIN D2          // GPIO4
#define DHTTYPE DHT11
#define TRIG_PIN D3        // GPIO0
#define ECHO_PIN D4        // GPIO2
#define RELAY_PIN D5       // GPIO14

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Awalnya relay mati

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Smart Farming - Versi Offline");
}

void loop() {
  float suhu = dht.readTemperature();
  float lembab = dht.readHumidity();

  // Cek jika gagal membaca sensor
  if (isnan(suhu) || isnan(lembab)) {
    Serial.println("Gagal membaca DHT11");
    delay(2000);
    return;
  }

  // Baca tinggi air dengan sensor ultrasonik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long durasi = pulseIn(ECHO_PIN, HIGH);
  float jarak = durasi * 0.034 / 2; // cm

  // Logika penyiraman otomatis
  if (suhu > 30 || lembab < 50 || jarak > 10) {
    digitalWrite(RELAY_PIN, LOW);  // Relay ON (aktif tergantung modul)
    Serial.println("POMPA: AKTIF");
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Relay OFF
    Serial.println("POMPA: MATI");
  }

  // Tampilkan data ke Serial
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C | Kelembaban: ");
  Serial.print(lembab);
  Serial.print(" % | Jarak Air: ");
  Serial.print(jarak);
  Serial.println(" cm");

  delay(2000);
}
