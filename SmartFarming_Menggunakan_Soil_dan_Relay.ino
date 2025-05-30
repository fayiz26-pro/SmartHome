#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "SmartFarming"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "NamaWiFi";      // Ganti dengan WiFi kamu
char pass[] = "PasswordWiFi";  // Ganti dengan password WiFi

const int soilMoisturePin = A0;
const int relayPin = D1; // GPIO5

BlynkTimer timer;

// Opsional: kontrol pompa manual dari Blynk
BLYNK_WRITE(V2) {
  int val = param.asInt();
  digitalWrite(relayPin, val);
}

void sendSoilData() {
  int moisture = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.println(moisture);

  Blynk.virtualWrite(V0, moisture);

  // Tampilkan LED indikator jika sangat kering
  if (moisture > 700) {
    Blynk.virtualWrite(V1, 1); // LED ON
    digitalWrite(relayPin, HIGH); // Pompa hidup
  } else {
    Blynk.virtualWrite(V1, 0); // LED OFF
    digitalWrite(relayPin, LOW); // Pompa mati
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, sendSoilData);
}

void loop() {
  Blynk.run();
  timer.run();
}
