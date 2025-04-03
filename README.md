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

*Este projeto foi desenvolvido durante a disciplina de **Fundamentos de Sistemas Embarcados**, ministrada pelo [Prof. Dr. Rodolfo Botto](https://buscatextual.cnpq.br/buscatextual/visualizacv.do;jsessionid=2708D44749FF71E24222D13D375F0255.buscatextual_0).*

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

Para iniciar os serviços necessários, como Mosquitto (broker MQTT), Node-RED e Grafana, utilize o comando abaixo:

```sh
docker-compose up --build -d
```

### Configuração do Servidor