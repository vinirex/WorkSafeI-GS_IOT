# WorkSafe IoT
**Link:** [Wokwi|Projects](https://wokwi.com/projects/466046417375426561)
# 🚀 SafeSpace – Guardião de Habitats Extremos


## 👨‍💻 Integrantes

| Nome | RM |
|--------|--------|
| Vinicius Silva | RM 553240 |
| Victor Didoff | RM 552965 |
| Matheus Zottis | RM 94119 |

---

## 📋 Descrição do Projeto

O **SafeSpace** é um sistema inteligente de monitoramento ambiental desenvolvido para habitats espaciais em ambientes extremos, como colônias na Lua ou em Marte.

O objetivo é garantir a segurança dos astronautas por meio da análise contínua das condições internas do habitat, classificando o ambiente em três estados:

- 🟢 **Seguro**
- 🟡 **Alerta**
- 🔴 **Crítico**

A classificação é realizada utilizando conceitos de **TinyML (Tiny Machine Learning)**, onde um conjunto de dados é utilizado para treinar um modelo e as regras aprendidas são posteriormente convertidas em estruturas condicionais (`if/else`) executadas diretamente no ESP32.

---

## 🎯 Problema Resolvido

Em habitats espaciais, pequenas alterações ambientais podem comprometer a sobrevivência da tripulação.

O SafeSpace monitora:

- Temperatura interna
- Umidade relativa do ar
- Nível de luminosidade/radiação simulada

e alerta imediatamente os ocupantes em caso de condições perigosas.

---

## 🌎 Alinhamento com a ODS

### ODS 11 – Cidades e Comunidades Sustentáveis

O projeto contribui para o desenvolvimento de ambientes habitáveis resilientes, sustentáveis e seguros, aplicando tecnologias embarcadas e Inteligência Artificial para monitoramento contínuo.

---

## 🧠 Conceito TinyML

O projeto segue o fluxo:

1. Coleta de dados simulados no Wokwi
2. Geração do dataset CSV
3. Treinamento do modelo no Edge Impulse
4. Análise dos padrões aprendidos
5. Conversão das regras para estruturas `if/else`
6. Execução da inferência diretamente no ESP32

### Classes Utilizadas

| Classe | Significado |
|----------|----------|
| Seguro | Ambiente dentro dos parâmetros ideais |
| Alerta | Ambiente apresenta tendência de falha |
| Crítico | Ambiente oferece risco à tripulação |

---

## 🔧 Tecnologias Utilizadas

- ESP32
- Wokwi Simulator
- Edge Impulse
- TinyML
- C++
- HTML
- WebServer ESP32
- GitHub

---

## 🛠 Componentes Utilizados

| Componente | Função |
|------------|---------|
| ESP32 DevKit V4 | Controlador principal |
| DHT22 | Sensor de temperatura e umidade |
| Sensor LDR | Simulação de luminosidade/radiação |
| LED Verde | Estado Seguro |
| LED Amarelo | Estado Alerta |
| LED Vermelho | Estado Crítico |
| Buzzer | Alarme de emergência |
| Resistores 220Ω | Proteção dos LEDs |

<img width="733" height="636" alt="image" src="https://github.com/user-attachments/assets/8693ab0c-a4db-4378-b189-ebabff3aa0c6" />

---

## 🔌 Tabela de Conexões

| Componente | GPIO ESP32 |
|------------|------------|
| DHT22 DATA | GPIO 15 |
| LDR AO | GPIO 34 |
| LED Verde | GPIO 25 |
| LED Amarelo | GPIO 26 |
| LED Vermelho | GPIO 27 |
| Buzzer | GPIO 14 |

---

## 📊 Regras de Classificação

### 🔴 CRÍTICO

```cpp
temperatura >= 40
ou
temperatura <= 5

ou

umidade <= 20

ou

luminosidade <= 1000
```

### 🟡 ALERTA

```cpp
temperatura > 30
ou
temperatura < 18

ou

umidade <= 50

ou

luminosidade <= 3000
```

### 🟢 SEGURO

```cpp
Todos os parâmetros dentro da faixa ideal.
```

---

## 📡 Funcionamento

### Estado Seguro

- LED Verde ligado
- Buzzer desligado

### Estado Alerta

- LED Amarelo ligado
- Buzzer desligado

### Estado Crítico

- LED Vermelho ligado
- Buzzer ativado

---

## 🌐 Interface Web

O ESP32 hospeda uma página HTML acessível pela rede local contendo:

- Status atual do habitat
- Temperatura
- Umidade
- Luminosidade

A página é atualizada automaticamente a cada 2 segundos.

<img width="1315" height="820" alt="image" src="https://github.com/user-attachments/assets/93902c71-6de2-4326-ab2b-5af1918b4e66" />


---

## 📁 Estrutura do Repositório

```text
SafeSpace/
│
├── sketch.ino
├── diagram.json
├── libraries.txt
│
├── dataset/
│   └── habitat_dataset.csv
│
├── model/
│   └── model.h
│
├── video/
│   └── demonstracao.mp4
│
└── README.md
```

---

## ▶ Como Executar

### Wokwi

1. Abrir o projeto no Wokwi
2. Iniciar a simulação
3. Alterar os valores do DHT22
4. Alterar a luminosidade do LDR
5. Observar a mudança dos LEDs e do buzzer
6. Acessar a página web do ESP32

---

## 📹 Vídeo Demonstrativo

O vídeo apresenta:

- Estrutura do projeto
- Circuito montado no Wokwi
- Funcionamento dos sensores
- LEDs e buzzer
- Interface Web
- **Link:**  [Video](https://canva.link/8nf1fup5wi0bezj)
---


## 📚 Referências

- Edge Impulse
- ESP32 Documentation
- TinyML Foundation
- Wokwi Simulator
- FIAP - Global Solution

---

# SafeSpace

**Monitoramento Inteligente para Habitats Espaciais Extremos utilizando TinyML e ESP32.** 🚀🌕🛰️
