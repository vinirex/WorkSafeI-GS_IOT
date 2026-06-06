#include <WiFi.h>
#include <WebServer.h>
#include "DHTesp.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WebServer server(80);
DHTesp dhtSensor;

const int DHT_PIN = 15;
const int LDR_PIN = 34;

const int LED_GREEN = 25;
const int LED_YELLOW = 26;
const int LED_RED = 27;

const int BUZZER_PIN = 14;

float temperatura = 0;
float umidade = 0;
int luminosidade = 0;

String statusEstufa = "SEGURO";

void desligarSaidas() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
}

void handleRoot() {

  String cor = "#00ff00";

  if(statusEstufa == "ALERTA")
    cor = "#ffff00";

  if(statusEstufa == "CRITICO")
    cor = "#ff0000";

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<title>SafeSpace</title>";
  html += "<style>";
  html += "body{background:#050505;color:white;font-family:Arial;text-align:center;}";
  html += ".card{background:#111;padding:20px;border-radius:15px;width:350px;margin:auto;margin-top:40px;}";
  html += ".status{font-size:40px;font-weight:bold;color:" + cor + ";}";
  html += "</style></head><body>";

  html += "<div class='card'>";
  html += "<h1>🌱 SafeSpace</h1>";
  html += "<p class='status'>" + statusEstufa + "</p>";
  html += "<p>🌡 Temperatura: " + String(temperatura,1) + " °C</p>";
  html += "<p>💧 Umidade: " + String(umidade,1) + " %</p>";
  html += "<p>☀ Luminosidade: " + String(luminosidade) + "</p>";
  html += "</div>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("=== SAFESPACE ===");

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();

  Serial.println("Servidor iniciado");
}

void loop() {

  server.handleClient();

  TempAndHumidity dados = dhtSensor.getTempAndHumidity();

  temperatura = dados.temperature;
  umidade = dados.humidity;
  luminosidade = analogRead(LDR_PIN);

  desligarSaidas();

  // CRÍTICO
  if (
      temperatura >= 40 ||
      temperatura <= 5 ||
      umidade <= 20 ||
      luminosidade <= 500
  ) {

    statusEstufa = "CRITICO";

    digitalWrite(LED_RED, HIGH);

    
    digitalWrite(BUZZER_PIN, HIGH);

  }

  // ALERTA
  else if (
      temperatura > 30 ||
      temperatura < 18 ||
      umidade <= 40 ||
      luminosidade <= 2000
  ) {

    statusEstufa = "ALERTA";

    digitalWrite(LED_YELLOW, HIGH);

    digitalWrite(BUZZER_PIN, LOW);
  }

  // SEGURO
  else {

    statusEstufa = "SEGURO";

    digitalWrite(LED_GREEN, HIGH);

    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.println("==========");
  Serial.print("Temp: ");
  Serial.println(temperatura);

  Serial.print("Umidade: ");
  Serial.println(umidade);

  Serial.print("Luz: ");
  Serial.println(luminosidade);

  Serial.print("Status: ");
  Serial.println(statusEstufa);

  delay(2000);
}