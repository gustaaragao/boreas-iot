# Bóreas (Medidor de Temperatura e Umidade)
<div style="text-align: center;">
  <img src="images/boreas.jpg" alt="Imagem do Bóreas" style="width: 100%; max-width: 600px;">
  <p style="font-style: italic; margin-top: 10px; font-size: smaller;">Figura 1: Representação artística de <b>Bóreas</b>, o Deus Grego dos ventos do norte.</p>
</div>

O **Bóreas** é um **projeto IoT** que utiliza um **Arduino** para medir **temperatura** e **umidade do ambiente** a partir do sensor **DHT22**. 

Os dados são enviados para um **broker MQTT (Mosquitto)** hospedado em um servidor Linux. O **Node-RED** atua como uma ponte de dados (**data bridge**), realizando a comunicação entre o **Mosquitto** e o banco de dados **InfluxDB**, onde os dados são armazenados. 

O **Grafana** consome os dados do **InfluxDB** para gerar um **dashboard** interativo e visualmente atraente. Além disso, o último dado aferido é exibido em um display **LCD 20x4** que está acoplado ao Arduino.

Todo o sistema é conteinerizado utilizando **Docker**, o que facilita a implantação e o gerenciamento dos serviços necessários, como **Mosquitto**, **Node-RED**, **InfluxDB** e **Grafana**.

<div style="text-align: center; margin-top: 20px;">
  <img src="images/diagrama.png" alt="Diagrama do Sistema" style="width: 100%; max-width: 800px;">
  <p style="font-style: italic; margin-top: 10px; font-size: smaller;">Figura 2: Diagrama do sistema IoT do projeto Bóreas.</p>
</div>

<div style="display: flex; justify-content: center; gap: 20px; margin-top: 20px;">
  <div style="text-align: center;">
    <img src="images/projeto-real-life.jpg" alt="Foto do Projeto Real - Visão Frontal" style="width: 100%; max-width: 300px;">
    <p style="font-style: italic; margin-top: 10px; font-size: smaller;">Figura 3: Foto do projeto embarcado.</p>
  </div>
  <div style="text-align: center;">
    <img src="images/projeto-real-life-lcd.jpg" alt="Foto do Projeto Real - Visão Lateral" style="width: 100%; max-width: 300px;">
    <p style="font-style: italic; margin-top: 10px; font-size: smaller;">Figura 4: Foto do Display LCD 20x4.</p>
  </div>
</div>

<div style="text-align: center; margin-top: 20px;">
  <img src="images/dashboard-grafana.jpg" alt="Diagrama do Sistema" style="width: 100%; max-width: 800px;">
  <p style="font-style: italic; margin-top: 10px; font-size: smaller;">Figura 5: Dashboard gerado no Grafana.</p>
</div>

*Este projeto foi desenvolvido durante a disciplina de **Fundamentos de Sistemas Embarcados** durante o Curso de **Ciência da Computação** na **Universidade Federal de Sergipe (UFS)**.

## Componentes Utilizados (e Recomendados)

- **Arduino Mega**

- **Shield Ethernet W5100 compatível com o Mega**

- **Cabo Ethernet**

- **Sensor DHT22** (Temperatura e Umidade)

- **Display LCD 20x4** com módulo I2C

- **Jumpers** para conexões
  - Para a conexão com o módulo I2C é necessário usar jumpers com pelo menos uma entrada fêmea.

- **Protoboard** para montagem do circuito

- **Computador** para programação e execução dos serviços  
  - No desenvolvimento deste projeto, utilizamos um computador como servidor para a aplicação do servidor. Inicialmente, a ideia era implementar o sistema em um **Raspberry Pi**, mas isso não foi possível. No entanto, como toda a aplicação do servidor está conteinerizada, a migração para um **Raspberry Pi** é bastante simples e prática.

## Softwares Necessários

Para criar e executar o projeto **Bóreas**, você precisará dos seguintes softwares:

- **Arduino IDE**: Para programar o Arduino Mega.

- **Docker**: Para conteinerizar e gerenciar os serviços do projeto.
  - A partir do `docker-compose.yml` vamos instalar todas as dependências necessárias para o Servidor.
    - **Mosquitto**: Broker MQTT para comunicação entre dispositivos.
    - **Node-RED**: Para criar fluxos de dados e integrar os serviços.
    - **InfluxDB**: Banco de dados para armazenar as medições de temperatura e umidade.
    - **Grafana**: Para criar dashboards interativos e monitorar os dados.

- **Bibliotecas Arduino**: estão presentes na pasta `arduino/libs/`.

Certifique-se de instalar todas as dependências e configurar os serviços conforme descrito na seção de configuração do ambiente.

## Configuração do Ambiente

### Estrutura do Projeto
```
sensor-temperatura-umidade-arduino-iot/
├── .gitignore
├── diagrama.excalidraw                # Diagrama Geral do Projeto feito no Excalidraw
├── docker-compose.yml                 # Arquivo para montar os containers
├── README.md
├── arduino/                           # Bibliotecas Utilizadas no Arduino
│   ├── arduino.ino                    # Código do Projeto Arduino
│   ├── libs/
│   │   ├── Adafruit_Sensor-1.1.15.zip
│   │   ├── ArduinoJson-7.3.1.zip
│   │   ├── DHT-sensor-library-1.4.6.zip
│   │   ├── Ethernet-2.0.2.zip
│   │   ├── LiquidCrystal_I2C-1.1.3.zip
│   │   └── pubsubclient-2.8.zip
├── images/                            # Imagens usadas na documentação
│   ├── boreas.jpg
│   ├── dashboard-grafana.jpg
│   ├── diagrama.png
│   ├── projeto-real-life-lcd.jpg
│   └── projeto-real-life.jpg
├── server/                            # Pasta para armazenar os volumes (configurações) dos containers
│   ├── configs/
│   │   ├── grafana/
│   │   │   ├── custom.ini
│   │   │   ├── dashboards/
│   │   │   └── provisioning/
│   │   ├── mosquitto/
│   │   │   └── config/
│   │   └── node-red/
│   │       ├── .config.nodes.json
│   │       └── package.json
```

### Configuração do Arduino

1. Instale as dependências presentes na pasta `arduino/libs`.

2. Configure as constantes para acessar o **MQTT Broker (Mosquitto)**
```cpp
// Configuração do MQTT Broker
IPAddress server(192, 168, 137, 1);   // Endereço IP do MQTT Broker (neste caso, o IP da máquina que hospeda o Mosquitto)
IPAddress ip(192, 168, 137, 19);      // Endereço IP do Arduino (certifique-se de que ambos estão na mesma rede)

#define PORTA_MQTT_BROKER 1883        // Porta padrão do Mosquitto (MQTT Broker)
```

2. Configure as constantes para o **módulo I2C** e o **Display LCD 20x4**:

```cpp
// Configuração do módulo I2C e Display LCD 20x4
#define NUMERO_COLUNAS 20             // Número de colunas do display
#define NUMERO_LINHAS 4               // Número de linhas do display
#define ENDERECO 0x27                 // Endereço do módulo I2C (varia conforme o modelo utilizado, verifique o seu)
```

3. Embarque o código na sua placa.

### Configuração do Servidor

1. Instale o **Docker** na sua máquina (e o **Docker Desktop** ou o **Podman** para facilitar o gerenciamento dos containers). 

2. Certifique-se de que o arquivo `docker-compose.yml` está configurado corretamente. Ele já inclui os serviços necessários, como **Mosquitto**, **Node-RED**, **InfluxDB** e **Grafana**.

3. Execute o seguinte comando para iniciar os containers:

```sh
docker-compose up --build -d
```

4. *(Opcional)* Use o [**MQTTBox**](https://github.com/workswithweb/MQTTBox) para verificar se o Mosquitto está funcionando.

5. Configure o **InfluxDB**: criar um usuário, uma organização chamada `ufs` e um bucket `home/temperature-humidity`.

6. *(Opcional)* Configure o **NODE-RED**: talvez seja necessário instalar manualmente a dependência de comunicação com InfluxDB, chamada `node-red-contrib-influxdb`.

7. Configure o **NODE-RED**: substitua o **API Token do InfluxDB** presente no projeto pelo o seu. Recomendo esse [tutorial](https://docs.influxdata.com/influxdb/cloud/admin/tokens/create-token/) da Documentação Oficial.

8. Configure o **Grafana**: crie um usuário.

9. *(Opcional)* No **Grafana**, importe o **_json_** disponível em `server/configs/grafana/dashboards/dht22-dashboard.json`.

## Contato

Se você tiver dúvidas, sugestões ou quiser contribuir com o projeto, entre em contato:

- **Autor**: Gustavo Henrique Aragão Silva
- **E-mail**: gustavo.aragao@dcomp.ufs.br
- **LinkedIn**: [linkedin.com/in/gustavoaragaoo](https://linkedin.com/in/gustavoaragaoo)
- **GitHub**: [github.com/gustaaragao](https://github.com/gustaaragao)

Fique à vontade para abrir issues ou pull requests no repositório do projeto!


## Agradecimentos Especiais

Gostaríamos de expressar nossos sinceros agradecimentos a [Paulo Aragão](https://github.com/pauloaragao) por sua contribuição e suporte a arquitetura da solução e mostrar o caminho.
