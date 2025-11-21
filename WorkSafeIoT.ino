#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ---------- CONFIGURAÇÕES ----------

// WiFi
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// MQTT
const char* mqtt_server = "test.mosquitto.org";
const char* topicoDist = "worksafe/distancia";
const char* topicoPIR  = "worksafe/movimento";
const char* topicoTemp = "worksafe/temperatura";
const char* topicoUmi  = "worksafe/umidade";
const char* topicoLuz  = "worksafe/luminosidade";

// ---------- PINAGEM ----------
#define TRIG_PIN 5
#define ECHO_PIN 18   // via divisor 5V→3.3V

#define DHTPIN 4
#define PIRPIN 15
#define LDRPIN 34

#define LED_R 13
#define LED_G 12
#define LED_B 14

#define BUZZER 27

// ---------- OBJETOS ----------
DHT dht(DHTPIN, DHT22);
WiFiClient espClient;
PubSubClient client(espClient);

// ---------- FUNÇÕES ----------

long medirDistanciaCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 38000UL);
  if (duracao == 0) return -1;
  return duracao / 58;
}

void setRGB(int r, int g, int b) {
  digitalWrite(LED_R, r);
  digitalWrite(LED_G, g);
  digitalWrite(LED_B, b);
}

// -----------------------------

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(PIRPIN, INPUT);
  pinMode(LDRPIN, INPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado.");

  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("WorkSafeDevice")) {
      Serial.println("conectado");
    } else {
      Serial.print("falhou, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // --- Distância ---
  long dist = medirDistanciaCm();
  if (dist > 0) {
    char msg[10];
    sprintf(msg, "%ld", dist);
    client.publish(topicoDist, msg);

    if (dist < 100) {
      setRGB(1, 0, 0);   // vermelho
      tone(BUZZER, 1000, 100);
    } else {
      setRGB(0, 1, 0);   // verde
    }
  }

  // --- PIR ---
  int movimento = digitalRead(PIRPIN);
  client.publish(topicoPIR, movimento ? "1" : "0");

  // --- DHT22 ---
  float temp = dht.readTemperature();
  float umi  = dht.readHumidity();

  if (!isnan(temp)) {
    char msgT[8];
    dtostrf(temp, 5, 2, msgT);
    client.publish(topicoTemp, msgT);
  }

  if (!isnan(umi)) {
    char msgU[8];
    dtostrf(umi, 5, 2, msgU);
    client.publish(topicoUmi, msgU);
  }

  // --- LDR ---
  int luz = analogRead(LDRPIN);
  char msgL[8];
  sprintf(msgL, "%d", luz);
  client.publish(topicoLuz, msgL);

  delay(600);
}

