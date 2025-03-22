#!/bin/bash
# filepath: c:\Users\gugao\Desktop\projeto-embarcados\server\scripts\setup.sh

echo "Configurando diretórios e permissões..."

# Criar diretórios necessários
mkdir -p ../configs/mqtt/config ../configs/mqtt/data ../configs/mqtt/log
mkdir -p ../configs/node-red
mkdir -p ../configs/influxdb
mkdir -p ../configs/grafana

# Ajustar permissões
chmod -R 777 ../configs/mqtt ../configs/node-red ../configs/influxdb ../configs/grafana

# Configurar Mosquitto
./server/scripts/setup-mosquitto.sh

echo "Configuração concluída!"