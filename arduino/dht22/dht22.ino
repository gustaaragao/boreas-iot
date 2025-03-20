#include <DHT.h>
#include <DHT_U.h>
#include <math.h>

#define DHTPIN 3
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

float duracao;
float distancia;

void setup() {
  // Inicializa o Serial
  Serial.begin(9600);

  // Inicializa o DHT
  dht.begin();

  delay(2000);
}

void loop() {
  Serial.print("Umidade: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(dht.readHumidity()); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO
  Serial.print("%"); //IMPRIME O TEXTO NA SERIAL 
  Serial.print(" / Temperatura: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(dht.readTemperature()); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO E REMOVE A PARTE DECIMAL
  Serial.println("*C"); //IMPRIME O TEXTO NA SERIAL
  delay(2000); //INTERVALO DE 2 SEGUNDOS * NÃO DIMINUIR ESSE VALOR
}