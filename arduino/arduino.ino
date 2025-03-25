#include <DHT.h>                    // Biblioteca para lidar com o DHT22 (ou 11)
#include <Ethernet.h>               // Biblioteca para lidar com a conexao Ethernet
#include <PubSubClient.h>           // Biblioteca para Publisher e Subscriber --> Protocolo MQTT
#include <ArduinoJson.h>            // Biblioteca para lidar com JSON
#include <Wire.h>                   // Biblioteca para lidar com a comunicacao com I2C
#include <LiquidCrystal_I2C.h>      // Biblioteca para lidar com o display 16x2

/* CONSTANTES DE CONFIGURACAO DO DHT22 */
#define DHTPIN 3          // Pino para receber os dados do DHT22
#define DHTTYPE DHT22     // Definindo o tipo do DHT (11 ou 12) 

#define TEMPO_DELAY 2000 // Tempo de delay para cada inferencia (>= 2s, que consiste no Tempo de Resposta do DHT22) 

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
}

/* FUNCAO RESPONSAVEL PELA CONEXAO COM O MQTT BROKER */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ArduinoDHT22")) {
      // Exibir sucesso no Serial
      Serial.println("Conectado!");
      // Exibir sucesso no Display LCD
      lcd.setCursor(0,2);
      lcd.print("- MQTT Conectado!");
      // Exibir sucesso da Inicializacao da Aplicacao no LCD
      lcd.setCursor(0, 3);
      lcd.print("Inicializacao OK!");
      delay(5000);
      lcd.clear();
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

  lcd.clear();

  lcd.setCursor(4, 0);
  lcd.print("Boreas Frost");

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
    // Caso nada tenha sido capturado, retorna uma mensagem de erro no Serial e tenta novamente
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

  // client.publish("home/temperature-humidity", msg); // Enviando o JSON para o tópico "home/temperature-humidity"
  
  // Debbugando no Serial o JSON enviado
  Serial.print("JSON enviado para o MQTTBroker: ");
  Serial.println(msg);
  
  delay(TEMPO_DELAY); // Tempo de Delay para a proxima medicao
}