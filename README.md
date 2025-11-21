# WorkSafe IoT

**Integrantes:**
- Vinicius Silva - RM553240
- Victor Didoff - RM552965
- Matheus Zottis - RM94119

**Projeto:** WorkSafe IoT — SmartWorkCare (Global Solutions 2025)

**Link:** https://wokwi.com/projects/447920966323106817

**Resumo rápido**
WorkSafe IoT é um protótipo de estação de monitoramento ambiental e bem‑estar para ambientes de trabalho híbridos. Implementado com **ESP32**, sensores (DHT22, PIR, LDR) e atuadores (LED RGB + buzzer), ele publica dados via **MQTT** para um servidor/ dashboard (ex.: Node-RED). O objetivo é demonstrar como IoT pode aumentar saúde, conforto e produtividade no "Futuro do Trabalho".

---

## Estrutura do repositório 

* `README.md` (este arquivo)
* `src/WorkSafeIoT.ino` — código principal para ESP32
* `WorkSafe_NodeRED_flow.json` — fluxo exportável do Node-RED
* `docs/` — imagens do circuito, prints do Wokwi e screenshots do dashboard
* `video/` — vídeo de até 3 minutos

---

## Componentes (compatíveis com Wokwi)

* ESP32 Dev Board
* Sensor DHT22 (temperatura e umidade)
* Sensor PIR HC-SR501 (presença)
* LDR + resistor 10k (luminosidade)
* LED RGB comum (três resistores 220Ω)
* Buzzer (passivo)
* Jumpers e breadboard

---

## Comunicação

* **MQTT Broker sugerido (teste)**: `broker.hivemq.com` ou `test.mosquitto.org`
* Tópicos publicados pelo dispositivo (exemplos):

  * `worksafe/temperatura`
  * `worksafe/umidade`
  * `worksafe/luminosidade`
  * `worksafe/presenca`
  * `worksafe/alerta`

O fluxo Node-RED usa esses tópicos para montar um dashboard com gráficos, indicadores e alertas.

---

## Como usar

1. Abra o Wokwi e crie um novo projeto ESP32.
2. Adicione os componentes listados na seção "Componentes".
3. Carregue o código `src/WorkSafeIoT.ino` no sketch do Wokwi.
4. Configure o broker MQTT no sketch (padrão: broker.hivemq.com).
5. Importe `node-red/WorkSafe_NodeRED_flow.json` no seu Node-RED local ou na instância pública (ex.: nodered.org sandbox) e conecte ao mesmo broker.

---

