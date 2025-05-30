#define BLYNK_TEMPLATE_ID "Isi_Template_ID"
#define BLYNK_TEMPLATE_NAME "Sistem Kebakaran"
#define BLYNK_AUTH_TOKEN "Isi_Token_Blynk"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>

char ssid[] = "Nama_WiFi";
char pass[] = "Password_WiFi";

// Sensor & Aktuator
#define DHTPIN 4       // D2
#define DHTTYPE DHT11
#define BUZZER_PIN 0   // D3
#define MQ2_PIN A0
#define SERVO_PIN 2    // D4

DHT dht(DHTPIN, DHTTYPE);
Servo myservo;
BlynkTimer timer;

void sendData() {
  float suhu = dht.readTemperature();
  int gasLevel = analogRead(MQ2_PIN);

  if (isnan(suhu)) {
    Serial.println("Gagal membaca suhu!");
    return;
  }

  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, gasLevel);

  Serial.println("Suhu: " + String(suhu) + " | Gas: " + String(gasLevel));

  // Cek kebakaran
  if (suhu > 45 || gasLevel > 300) {
    digitalWrite(BUZZER_PIN, HIGH);
    myservo.write(90); 
    Blynk.virtualWrite(V2, "🔥 Bahaya! Kebakaran terdeteksi!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    myservo.write(0); 
    Blynk.virtualWrite(V2, "✅ Aman");
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  myservo.attach(SERVO_PIN);
  myservo.write(0); 

  timer.setInterval(2000L, sendData);
}

void loop() {
  Blynk.run();
  timer.run();
}
