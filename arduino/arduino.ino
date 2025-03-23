#include <DHT.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

/* CONSTANTES DE CONFIGURACAO DO DHT22 */
#define DHTPIN 3          // Pino para receber os dados do DHT22
#define DHTTYPE DHT22     // Definindo o tipo do DHT (11 ou 12) 

#define TEMPO_DELAY 100000 // Tempo de delay para cada inferencia (>= 2s, que consiste no Tempo de Resposta do DHT22) 

DHT dht(DHTPIN, DHTTYPE); // Criando objeto DHT

/* CONSTANTES DE CONFIGURACAO DO MQTT */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192, 168, 137, 1);   // IP do MQTT Broker (no nosso caso, o IP da máquina com o MQTT Broker)
IPAddress ip(192, 168, 137, 19);      // IP do Arduino (Lembre-se que ambos precisam está na mesma rede)

#define PORTA_MQTT_BROKER 1883        // Por padrão, o Mosquitto (MQTT Broker) fica hospedado na porta 1883

EthernetClient ethClient;

PubSubClient client(ethClient);

/* FUNCAO DE INICIALIZACAO DO PROJETO */
void setup() {
  Serial.begin(9600);                             // Inicializa a comunicacao Serial
  
  Serial.println("Serial Conectado");             // Debuggando Serial

  dht.begin();                                    // Inicializa o DHT22
  
  Ethernet.begin(mac, ip);                        // Inicializa a comunicação Ethernet usando o endereço MAC fornecido e configura um IP fixo para o Arduino

  delay(1500);                                    // Delay de 1.5 segundos (BOA PRÁTICA)
  
  client.setServer(server, PORTA_MQTT_BROKER);    // Configura e Inicializa um cliente MQTT
}

/* FUNCAO RESPONSAVEL PELA CONEXAO COM O MQTT BROKER */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ArduinoDHT22")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando em 5s...");
      delay(5000);
    }
  }
}

/* FUNCAO DE LOOP DO PROJETO */
void loop() {
  if (!client.connected()) {
    // Tenta a conexao com MQTT Broker caso nao haja
    reconnect();
  }

  client.loop(); // Essa funcao mantem a conexao com o MQTT Broker sempre ativa

  float h = dht.readHumidity();       // Medindo a UMIDADE pelo DHT22
  float t = dht.readTemperature();    // Medindo a TEMPERATURA pelo DHT22

  // Debbugando no Serial os valores aferidos
  Serial.print("Valores aferidos pelo DHT22: \t");
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print("% / Temperatura: ");
  Serial.print(t);
  Serial.println("°C");

  if (isnan(h) || isnan(t)) {
    // Caso nada tenha sido capturado, retorna uma mensagem no Serial e para a execução
    Serial.println("Falha na leitura do sensor!");
    return;
  }

  // Criação do JSON com ArduinoJson --> O MQTT Broker está configurado para receber um JSON
  // o JSON tem o seguinte formado: {"t": float, "h": float}
  StaticJsonDocument<128> jsonDocument;
  jsonDocument["t"] = t;
  jsonDocument["h"] = h;
  
  char msg[128];
  serializeJson(jsonDocument, msg);

  client.publish("home/temperature-humidity", msg); // Enviando o JSON para o tópico "home/temperature-humidity"
  
  // Debbugando no Serial o JSON enviado
  Serial.print("JSON enviado para o MQTTBroker: ");
  Serial.println(msg);
  
  delay(TEMPO_DELAY); // Tempo de Delay para a proxima medicao
}