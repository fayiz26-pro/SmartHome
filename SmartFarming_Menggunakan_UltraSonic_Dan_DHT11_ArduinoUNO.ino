#include <DHT.h>

// Pin Setup (gunakan nomor pin digital Arduino UNO)
#define DHTPIN 2        // Pin D2 pada UNO
#define DHTTYPE DHT11

#define TRIG_PIN 3      // Pin D3
#define ECHO_PIN 4      // Pin D4
#define RELAY_PIN 5     // Pin D5

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  // Gunakan 9600 untuk Serial Monitor standar UNO
  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Relay awalnya mati (aktif LOW)

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Smart Farming - Versi Arduino UNO");
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
  float jarak = durasi * 0.034 / 2; // hasil dalam cm

  // Logika penyiraman otomatis
  if (suhu > 30 || lembab < 50 || jarak > 10) {
    digitalWrite(RELAY_PIN, LOW);  // Relay ON (aktif LOW)
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
