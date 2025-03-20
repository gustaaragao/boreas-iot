#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 3 // Definindo o pino para o recebimento dos dados
#define DHTTYPE DHT22 // Definindo o tipo do Sensor

DHT dht(DHTPIN, DHTTYPE); // Criando objeto para o Sensor

#define TEMPO_DE_RESPOSTA 2000 // O DHT22 tem 2 segundos como tempo de resposta

float umidade;
float temperatura;

void setup() {
  // Inicializa o Serial
  Serial.begin(9600);

  // Inicializa o DHT
  dht.begin();

  // Delay para o Tempo de Resposta
  delay(TEMPO_DE_RESPOSTA);
}

void loop() {
  // Leitura dos valores
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature();

  // Impressão da Umidade 
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("%");
  
  // Impressão da Temperatura 
  Serial.print(" / Temperatura: ");
  Serial.print(temperatura);
  Serial.println("*C");

  // Delay para o Tempo de Resposta
  delay(TEMPO_DE_RESPOSTA);
}