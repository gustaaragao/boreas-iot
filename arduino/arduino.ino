#include <DHT.h>                    // Biblioteca para lidar com o DHT22 (ou 11)
#include <Ethernet.h>               // Biblioteca para lidar com a conexao Ethernet
#include <PubSubClient.h>           // Biblioteca para Publisher e Subscriber --> Protocolo MQTT
#include <ArduinoJson.h>            // Biblioteca para lidar com JSON
#include <Wire.h>                   // Biblioteca para lidar com a comunicacao com I2C
#include <LiquidCrystal_I2C.h>      // Biblioteca para lidar com o display 16x2

/* CONSTANTES DE CONFIGURACAO DO DHT22 */
#define DHTPIN 3          // Pino para receber os dados do DHT22
#define DHTTYPE DHT22     // Definindo o tipo do DHT (11 ou 12) 

#define TEMPO_DELAY 4000  // Tempo de delay para cada inferencia (>= 2s, que consiste no Tempo de Resposta do DHT22) 

DHT dht(DHTPIN, DHTTYPE); // Criando objeto DHT

/* CONSTANTES DE CONFIGURACAO DO MQTT */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192, 168, 137, 1);   // IP do MQTT Broker (no nosso caso, o IP da máquina com o MQTT Broker)
IPAddress ip(192, 168, 137, 19);      // IP do Arduino (Lembre-se que ambos precisam está na mesma rede)

#define PORTA_MQTT_BROKER 1883        // Por padrão, o Mosquitto (MQTT Broker) fica hospedado na porta 1883

EthernetClient ethClient;
PubSubClient client(ethClient);

/* CONSTANTES DE CONFIGURACAO DO I2C + DISPLAY LCD 20x4 */
#define NUMERO_COLUNAS 20
#define NUMERO_LINHAS 4
#define ENDERECO 0x27

LiquidCrystal_I2C lcd(ENDERECO, NUMERO_COLUNAS, NUMERO_LINHAS); // Criando o objeto LiquidCrystal para ser usada com o I2C

// Variáveis globais para armazenar os dados processados para exibição no LCD
bool novosDados = false;              // Flag que indica se há novos dados
float tempC, tempK, tempF, umidade;   // Valores que são aferidos pelo DHT processados
char timestamp[20];                   // Tempo atual (vai levar em consideração o tempo do servidor, ou seja, do MQTT Broker)


/* FUNCAO DE INICIALIZACAO DO PROJETO */
void setup() {
  lcd.init();                                     // Inicializa a comunicacao com o Display LCD
  lcd.backlight();                                // Liga a luz do Display
  
  // Exibe o nome da Marca "Boreas Frost"
  lcd.setCursor(0,0);
  lcd.print("Boreas Frost...");
  
  Serial.begin(9600);                             // Inicializa a comunicacao Serial
  Serial.println("Serial Conectado");             // Debuggando Serial
  
  lcd.setCursor(0,1);
  lcd.print("- Serial Conectado!");

  dht.begin();                                    // Inicializa o DHT22
  
  Ethernet.begin(mac, ip);                        // Inicializa a comunicação Ethernet usando o endereço MAC fornecido e configura um IP fixo para o Arduino

  delay(1500);                                    // Delay de 1.5 segundos (BOA PRÁTICA)
  
  client.setServer(server, PORTA_MQTT_BROKER);    // Configura e Inicializa um cliente MQTT
  client.setCallback(callback);                   // Configura a função de recebimento de mensagens
}

/* FUNCAO PARA PROCESSAR MENSAGENS RECEBIDAS (NÃO BLOQUEANTE) */
void callback(char* topic, byte* payload, unsigned int length) {
  // Debug: mostra mensagem recebida
  Serial.print("Mensagem recebida: ");
  Serial.write(payload, length);
  Serial.println();

  // Deserializar o JSON do Payload
  StaticJsonDocument<384> doc;
  deserializeJson(doc, payload, length);

  // Atualiza as variáveis globais diretamente do JSON
  tempC = doc["temperatureCelsius"] | 0.0;
  tempK = doc["temperatureKelvin"] | 0.0;
  tempF = doc["temperatureFahrenheit"] | 0.0;
  umidade = doc["humidity"] | 0.0;
  
  // Faz uma cópia do timestamp recebido pelo JSON
  const char* timestampRecebido = doc["timestamp"] | "00:00:00";
  strncpy(timestamp, timestampRecebido, sizeof(timestamp) - 1);
  timestamp[sizeof(timestamp) - 1] = '\0';

  novosDados = true;  // Sinaliza para atualização do display LCD no loop principal
}

/* FUNCAO RESPONSAVEL PELA CONEXAO COM O MQTT BROKER */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ArduinoDHT22")) {
      Serial.println("Conectado!");
      lcd.setCursor(0,2);
      lcd.print("- MQTT Conectado!");
      lcd.setCursor(0, 3);
      lcd.print("Inicializacao OK!");
      // Aguardar 2s para que o usuário consiga ler as mensagens no display
      delay(2000);
      lcd.clear();
      // Nosso cliente deve estar inscrito no topico "home/temperature-humidity-processed" para receber os dados e exibir no display LCD
      client.subscribe("home/temperature-humidity-processed"); 
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando em 5s...");
      delay(5000);
    }
  }
}

/* FUNCAO PARA ATUALIZAR O DISPLAY DE FORMA NÃO BLOQUEANTE */
void atualizarDisplay() {
  lcd.clear();
  
  // Linha 0: Nome centralizado
  lcd.setCursor(4, 0);
  lcd.print("Boreas Frost");

  // Linha 1: Exibe temperatura em Celsius e Fahrenheit
  lcd.setCursor(0, 1);
  lcd.print(tempC, 2);
  lcd.print("*C | ");
  lcd.print(tempF, 2);
  lcd.print("*F");

    // Linha 1: Exibe temperatura em Kelvin e a umidade do ambiente
  lcd.setCursor(0, 2);
  lcd.print(tempK, 2);
  lcd.print("K | ");
  lcd.print("H:");
  lcd.print(umidade, 1);
  lcd.print("%");

  // Linha 3: Horário dos dados
  lcd.setCursor(0, 3);
  lcd.print(timestamp);

  novosDados = false;
}

/* FUNCAO DE LOOP DO PROJETO */
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  // Mantém a conexão ativa com o MQTT Broker

  if (novosDados) {
    // Caso haja novosDados, atualize o display
    atualizarDisplay();
  }

  static unsigned long ultimaLeitura = 0; // Essa variável é importante para verificar se devemos ou não aferir outro valor de acordo com o TEMPO_DELAY
  if (millis() - ultimaLeitura >= TEMPO_DELAY) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Debug dos valores lidos
    Serial.print("DHT22 - Umidade: ");
    Serial.print(h);
    Serial.print("% / Temp: ");
    Serial.print(t);
    Serial.println("C");

    if (!isnan(h) && !isnan(t)) {
      StaticJsonDocument<128> jsonDocument;
      jsonDocument["t"] = t;
      jsonDocument["h"] = h;
      
      char msg[128];
      serializeJson(jsonDocument, msg);

      client.publish("home/temperature-humidity", msg);
      
      // Debug do JSON enviado
      Serial.print("Publicado: ");
      Serial.println(msg);
    }

    ultimaLeitura = millis();
  }
}