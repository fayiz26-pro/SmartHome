#define BLYNK_TEMPLATE_ID "Isi_dari_Blynk"
#define BLYNK_TEMPLATE_NAME "SmartHome"
#define BLYNK_AUTH_TOKEN "Masukkan_Token_kamu"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Nama_WiFi";
char pass[] = "Password_WiFi";

#define LDR_PIN A0
#define LED_PIN 14       // D5
#define BUZZER_PIN 12    // D6
#define TRIG_PIN 13      // D7
#define ECHO_PIN 15      // D8

int manualLamp = 0;

BlynkTimer timer;

BLYNK_WRITE(V2) {
  manualLamp = param.asInt();
}

void sendSensor() {
  // LDR
  int ldrValue = analogRead(LDR_PIN);
  Blynk.virtualWrite(V0, ldrValue);
  
  if (ldrValue < 500 || manualLamp == 1) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Ultrasonic
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  Blynk.virtualWrite(V1, distance);

  if (distance < 100 && distance > 0) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}