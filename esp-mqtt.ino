/*
 * Exemplo de conexão do ESP32 à rede WiFi e ao broker MQTT, publicando dados do sensor DHT22
 * Utiliza o broker público do HiveMQ
*/

// Inclui a biblioteca WiFi do ESP32
#include <WiFi.h>

// Inclui a biblioteca PubSubClient para comunicação MQTT
#include <PubSubClient.h>

// Inclui a biblioteca para usar sensores da família DHT (DHT11, DHT22, etc.)
#include "DHT.h"

// Define o pino do ESP32 onde o sensor DHT está conectado
#define DHTPIN 4

// Define o tipo de sensor utilizado (pode ser DHT11, DHT22 ou DHT21)
#define DHTTYPE DHT11

// Dados da rede Wi-Fi (no Wokwi, usa-se Wokwi-GUEST)
const char* ssid = "ads_IoT2026";
const char* password = "ads_IoT2026";

// Endereço do broker MQTT (HiveMQ público neste exemplo)
const char* mqtt_server = "10.110.18.11";

// Cria um objeto WiFiClient para comunicação TCP
WiFiClient espClient;

// Cria um cliente MQTT usando o cliente TCP
PubSubClient client(espClient);

// Cria um objeto da classe DHT com as configurações definidas acima
DHT dht(DHTPIN, DHTTYPE);

// Função para conectar ao Wi-Fi
void initWiFi() {
  WiFi.mode(WIFI_STA);             // Modo estação (cliente)
  WiFi.begin(ssid, password);      // Conecta à rede Wi-Fi
  Serial.print("Conectando-se ao WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("\nConectado com IP: ");
  Serial.println(WiFi.localIP());
}

// Função que tenta conectar ao broker MQTT
void reconnectMQTT() {
  // Fica tentando reconectar até conseguir
  while (!client.connected()) {
    Serial.println("Tentando conexão MQTT...");
    // Define um ID único para o client (pode ser qualquer string)
    String clientId = "ADSINIOT" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao broker MQTT");
    } else {
      Serial.print("Falha na conexão. Código: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);      // Inicia a comunicação serial
  initWiFi();                // Conecta à rede Wi-Fi
  client.setServer(mqtt_server, 1883); // Define o broker MQTT e a porta
  dht.begin(); // Inicializa o sensor DHT (configura pino e tipo)
}

void loop() {
  // Se o cliente MQTT estiver desconectado, tenta reconectar
  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();  // Mantém o cliente MQTT ativo e verificando novas mensagens

  // Lê a temperatura (em graus Celsius) e armazena na variável temperatura
  float temperatura = dht.readTemperature();

  // Lê a umidade
  float umidade = dht.readHumidity();

  // Formata o dado de temperatura em texto e publica no tópico iniot/andre/temperatura
  String mensagemTemp = String(temperatura, 1);
  Serial.println(mensagemTemp); // Imprime no monitor serial a mensagem formatada
  client.publish("ads/iniot/temperatura", mensagemTemp.c_str());

  // Formata o dado de umidade em texto e publica no tópico iniot/andre/temperatura
  String mensagemUmid = String(umidade, 1);
  Serial.println(mensagemUmid); // Imprime no monitor serial a mensagem formatada
  client.publish("ads/iniot/umidade", mensagemUmid.c_str());

  delay(5000);  // Aguarda 5 segundos antes de publicar novamente
}
