/* WorkSafe IoT - WorkSafeIoT.ino
   ESP32 + DHT22 + PIR + LDR + LED RGB + Buzzer
   Publica via MQTT para tópicos: worksafe/...
   Autor: seu grupo - GS 2025
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
#define PIRPIN 15
#define LDRPIN 34

#define REDPIN 13
#define GREENPIN 12
#define BLUEPIN 14
#define BUZZERPIN 27

DHT dht(DHTPIN, DHTTYPE);

// --- CONFIGURE AQUI ---
const char* ssid = "Wokwi-GUEST"; // ou seu SSID
const char* password = "";        // ou sua senha

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* clientId = "WorkSafeIoT-ESP32-01";
// ------------------------

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
int tempoPresencaSegundos = 0; // contador de presença contínua (em segundos)
bool presencaAnterior = false;

void setupWiFi() {
  delay(50);
  Serial.print("Conectando em ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 40) {
    delay(250);
    Serial.print(".");
    tries++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar WiFi (continuando em modo offline).");
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect(clientId)) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando de novo em 2s");
      delay(2000);
    }
  }
}

void setColor(int r, int g, int b) {
  // PWM 0-255 -> ESP32 analogWrite não disponível nativo, usamos ledc
  ledcWrite(0, r);
  ledcWrite(1, g);
  ledcWrite(2, b);
}

void setupPWM() {
  // Configure channels
  ledcSetup(0, 12000, 8); // canal 0, freq 12kHz, 8-bit
  ledcSetup(1, 12000, 8);
  ledcSetup(2, 12000, 8);
  // Attach pins
  ledcAttachPin(REDPIN, 0);
  ledcAttachPin(GREENPIN, 1);
  ledcAttachPin(BLUEPIN, 2);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIRPIN, INPUT);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(LDRPIN, INPUT);

  dht.begin();
  setupPWM();

  setupWiFi();
  client.setServer(mqtt_server, mqtt_port);

  // inicia LED verde
  setColor(0, 200, 0);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && !client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) { // a cada 2s
    lastMsg = now;

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int presenca = digitalRead(PIRPIN);
    int luzAnalog = analogRead(LDRPIN); // 0-4095
    int luzMap = map(luzAnalog, 0, 4095, 0, 1023); // escala 0-1023 para dashboard

    // presença contínua (contador simples de 2s)
    if (presenca) {
      tempoPresencaSegundos += 2;
    } else {
      tempoPresencaSegundos = 0;
    }

    // Publica no MQTT (strings)
    char buf[64];
    if (isnan(temp)) temp = -999;
    if (isnan(hum)) hum = -999;

    snprintf(buf, sizeof(buf), "%.2f", temp);
    client.publish("worksafe/temperatura", buf);
    snprintf(buf, sizeof(buf), "%.2f", hum);
    client.publish("worksafe/umidade", buf);
    snprintf(buf, sizeof(buf), "%d", luzMap);
    client.publish("worksafe/luminosidade", buf);
    snprintf(buf, sizeof(buf), "%d", presenca);
    client.publish("worksafe/presenca", buf);

    // Lógica de alertas
    bool alerta = false;
    String textoAlerta = "";

    if (temp > 27.0) {
      // temperatura alta
      setColor(255, 0, 0);
      alerta = true;
      textoAlerta = "Temperatura elevada";
    } else if (luzMap < 300) {
      // pouca luz
      setColor(255, 180, 0);
      alerta = true;
      textoAlerta = "Luminosidade baixa";
    } else if (tempoPresencaSegundos >= 1800) { // 30 minutos contínuos
      setColor(0, 0, 255);
      alerta = true;
      textoAlerta = "Hora de pausa";
      tempoPresencaSegundos = 0; // reset após alertar
    } else {
      // condições normais
      setColor(0, 200, 0);
    }

    if (alerta) {
      client.publish("worksafe/alerta", textoAlerta.c_str());
      // buzzer curto
      tone(BUZZERPIN, 2000, 200); // 2kHz por 200ms
      delay(220);
      noTone(BUZZERPIN);
    }

    // debug serial
    Serial.print("T=");
    Serial.print(temp);
    Serial.print(" H=");
    Serial.print(hum);
    Serial.print(" Luz=");
    Serial.print(luzMap);
    Serial.print(" Pres=");
    Serial.print(presenca);
    Serial.print(" TempoPresenca=");
    Serial.println(tempoPresencaSegundos);
  }
}
