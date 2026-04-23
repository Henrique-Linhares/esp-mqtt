# 🌡️ Monitoramento IoT — ESP32 + DHT + MQTT + Node-RED

Sistema de monitoramento ambiental em tempo real utilizando ESP32, sensor DHT, broker MQTT e dashboard Node-RED, com análise estatística dos dados coletados via Python.

---

## 📋 Visão Geral

```
ESP32 + DHT  ──→  Broker MQTT  ──→  Node-RED  ──→  Dashboard
                                        │
                                        └──→  CSV  ──→  Python (análise)
```

O firmware na ESP32 lê temperatura e umidade do sensor DHT e publica os dados no broker MQTT. O Node-RED se inscreve no tópico, exibe os valores em tempo real na dashboard e grava o histórico em um arquivo CSV. O script Python realiza análise estatística (média, mediana, moda, desvio padrão) e gera gráficos de tendência a partir do CSV exportado.

---

## 📁 Estrutura do Repositório

```
├── esp-mqtt.ino       # Firmware Arduino para ESP32 + DHT
├── flows.json         # Fluxo Node-RED (importar no editor)
├── app_mqtt.py        # Script Python para análise estatística do CSV
└── README.md
```

---

## 🔧 Hardware Necessário

- ESP32 
- Sensor DHT11
- Broker MQTT acessível na rede local

## ⚙️ Configuração

### 1. Firmware (`esp-mqtt.ino`)

Edite as constantes no início do arquivo:

```cpp
const char* ssid        = "SEU_WIFI";
const char* password    = "SUA_SENHA";
const char* mqtt_server = "IP_DO_BROKER";  // ex: 10.110.12.65
```

O firmware publica no tópico `iiot/desafio2` com payload JSON:

```json
{ "temperatura": 24.5, "umidade": 49 }
```

### 2. Bibliotecas Arduino (instalar via Library Manager)

- `PubSubClient` — Nick O'Leary
- `DHT sensor library` — Adafruit
- `Adafruit Unified Sensor` — Adafruit

Selecione a placa: **Tools → Board → ESP32 Arduino → ESP32 Dev Module**

### 3. Fluxo Node-RED (`flows.json`)

**Pré-requisito:** instalar o pacote de dashboard:

```
Menu → Manage Palette → Instalar: node-red-dashboard
```

**Importar o fluxo:**

```
Menu → Import → Cole o conteúdo de flows.json → Import
```

Acesse a dashboard em: `http://IP_DO_NODERED:1880/ui`

---

## 📊 Fluxo Node-RED

O fluxo possui duas seções principais:

**Monitoramento em tempo real:**
- Nó `mqtt in` → inscreve no tópico `iiot/desafio2`
- Nó `function` (Parse & Join) → separa temperatura e umidade do objeto JSON
- Saída 1 → `ui_gauge` + `ui_chart` de temperatura
- Saída 2 → `ui_gauge` + `ui_chart` de umidade
- Saída 3 → `function` (Formatar CSV) → `file` (grava `dados_sensores.csv`)

**Exportação de dados:**
- Botão `BAIXAR RELATÓRIO CSV` na dashboard
- Lê o arquivo CSV gravado e dispara download automático no navegador

**Formato do CSV gravado:**

```
Temperatura,Umidade,Timestamp
24.5,49,2026-04-23T20:15:42Z
24.6,50,2026-04-23T20:15:47Z
```

---

## 🐍 Análise Estatística (`app_mqtt.py`)

Script executado no Google Colab. Faz upload do CSV exportado e calcula:

| Métrica | Temperatura | Umidade |
|---------|------------|---------|
| Média | ✅ | ✅ |
| Mediana | ✅ | ✅ |
| Moda | ✅ | ✅ |
| Desvio Padrão | ✅ | ✅ |

Ao final, gera um gráfico de linha com a tendência histórica de ambas as variáveis, incluindo linhas de referência para as médias.


## 🌐 Tópicos MQTT

| Tópico | Direção | Conteúdo |
|--------|---------|----------|
| `iiot/desafio2` | ESP32 → Broker → Node-RED | `{ "temperatura": float, "umidade": float }` |

---

## 🛠️ Tecnologias Utilizadas

- **ESP32** + Arduino IDE
- **MQTT** (broker local na rede)
- **Node-RED** com node-red-dashboard
- **Python** (pandas, matplotlib) via Google Colab
