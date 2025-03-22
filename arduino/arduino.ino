#include <DHT.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DHTPIN 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define TEMPO_DELAY 10000

// Configuração de Rede
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 137, 19);    // IP do Arduino
IPAddress server(192, 168, 137, 1); // IP do PC com Docker

EthernetClient ethClient;
PubSubClient client(ethClient);

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  Ethernet.begin(mac, ip);
  delay(1500); // Estabilização
  
  client.setServer(server, 1883);
}

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

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Valores aferidos pelo DHT22: \t");
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print("% / Temperatura: ");
  Serial.print(t);
  Serial.println("°C");

  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na leitura do sensor!");
    return;
  }

  // Criação do JSON com ArduinoJson
  StaticJsonDocument<128> jsonDocument;
  jsonDocument["t"] = t;
  jsonDocument["h"] = h;
  
  char msg[128];
  serializeJson(jsonDocument, msg);

  client.publish("home/temperature-humidity", msg);
  Serial.print("Publicado: ");
  Serial.println(msg);
  
  delay(TEMPO_DELAY);
}