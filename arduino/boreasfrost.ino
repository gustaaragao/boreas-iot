#include <Wire.h> //biblioteca para comunicação I2C (usada para o LCD)
#include <Adafruit_LiquidCrystal.h> //biblioteca para controlar o display LCD via I2C

const int sensorPin = A0; //define o pino A0 para o sensor de temperatura TMP36

//cria um objeto LCD chamado "lcd_1" para comunicação via I2C
Adafruit_LiquidCrystal lcd_1(0);

void setup() {
  Serial.begin(9600); //inicia a comunicação serial com a taxa de 9600 bps (bits por segundo)

  lcd_1.begin(16, 2); //inicializa o LCD com 16 colunas e 2 linhas
  lcd_1.setBacklight(1); //liga a luz de fundo do LCD
  lcd_1.print("BoreasFrost..."); //mensagem inicial no LCD
  delay(2000); //aguarda 2 segundos antes de prosseguir
}

void loop() {
  int leitura = analogRead(sensorPin); //lê o valor do sensor TMP36 (0 a 1023)
  float tensao = leitura * (5.0 / 1023.0); //converte a leitura para tensão (0V a 5V)
  float temperaturaC = (tensao - 0.5) * 100.0; //converte a tensão para temperatura em Celsius
  float temperaturaF = (temperaturaC * 9.0 / 5.0) + 32.0; //converte Celsius para Fahrenheit
  float temperaturaK = temperaturaC + 273.15; //converte Celsius para Kelvin

  //exibe as temperaturas no Monitor Serial
  Serial.print("Temp: ");
  Serial.print(temperaturaC);
  Serial.print(" C, ");
  Serial.print(temperaturaF);
  Serial.print(" F, ");
  Serial.print(temperaturaK);
  Serial.println(" K");

  //atualiza o LCD com as temperaturas
  lcd_1.clear(); //limpa o display antes de escrever novos dados
  lcd_1.setCursor(0, 0); //define o cursor na primeira linha, primeira posição
  lcd_1.print("C:"); 
  lcd_1.print(temperaturaC); //temperatura em Celsius
  lcd_1.print(" F:");
  lcd_1.print(temperaturaF); //temperatura em Fahrenheit

  lcd_1.setCursor(0, 1); //move o cursor para a segunda linha, primeira posição
  lcd_1.print("K:");
  lcd_1.print(temperaturaK); //exibe a temperatura em Kelvin

  delay(1000); //aguarda 1 segundo antes de repetir a leitura
}
