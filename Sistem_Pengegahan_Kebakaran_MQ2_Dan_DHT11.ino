#include <DHT.h>
#include <Servo.h>

// Konfigurasi DHT11
#define DHTPIN 4        // D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pin lainnya
#define MQ2_PIN A0
#define BUZZER_PIN 0    // D3
#define SERVO_PIN 2     // D4

Servo ventilasi;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  ventilasi.attach(SERVO_PIN);
  ventilasi.write(0); // posisi awal tertutup

  Serial.println("Sistem Pencegahan Kebakaran Aktif...");
}

void loop() {
  float suhu = dht.readTemperature();
  int gasLevel = analogRead(MQ2_PIN);

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print("°C | Gas: ");
  Serial.println(gasLevel);

  // Logika deteksi kebakaran
  if (suhu > 45 || gasLevel > 300) {
    digitalWrite(BUZZER_PIN, HIGH);
    ventilasi.write(90);  // buka ventilasi
    Serial.println("🔥 BAHAYA! Kebakaran Terdeteksi!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    ventilasi.write(0);   // tutup ventilasi
    Serial.println("✅ Aman");
  }

  delay(2000);
}
