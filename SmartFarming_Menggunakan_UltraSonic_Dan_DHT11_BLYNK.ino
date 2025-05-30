#define BLYNK_TEMPLATE_ID "Isi_Template_ID"
#define BLYNK_TEMPLATE_NAME "SmartFarming"
#define BLYNK_AUTH_TOKEN "Isi_Auth_Token"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char ssid[] = "Nama_WiFi";
char pass[] = "Password_WiFi";

// Define Pin
#define DHTPIN 4         // D2
#define DHTTYPE DHT11
#define TRIG_PIN 0       // D3
#define ECHO_PIN 2       // D4
#define RELAY_PIN 14     // D5

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int manualRelay = 0;

BLYNK_WRITE(V3) {
  manualRelay = param.asInt();
}

void sendSensorData() {
  float suhu = dht.readTemperature();
  float lembab = dht.readHumidity();

  if (isnan(suhu) || isnan(lembab)) {
    Serial.println("Gagal membaca DHT11");
    return;
  }

  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, lembab);

  // Logika otomatis penyiraman
  if (suhu > 30 || lembab < 50 || manualRelay == 1) {
    digitalWrite(RELAY_PIN, LOW); // aktif (LOW untuk aktif relay tergantung modul)
  } else {
    digitalWrite(RELAY_PIN, HIGH); // nonaktif
  }

  // Sensor Ultrasonik (untuk cek tinggi air)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long durasi = pulseIn(ECHO_PIN, HIGH);
  float jarak = durasi * 0.034 / 2;

  Blynk.virtualWrite(V2, jarak);
  Serial.println("Suhu: " + String(suhu) + " | Kelembaban: " + String(lembab) + " | Jarak air: " + String(jarak));
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // pastikan relay mati saat awal

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
