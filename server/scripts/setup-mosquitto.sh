#!/bin/bash
# filepath: c:\Users\gugao\Desktop\projeto-embarcados\server\scripts\setup-mosquitto.sh

echo "Configurando Mosquitto..."

# Caminho para o arquivo de configuração desejado
CONFIG_SOURCE="./server/configs/mosquitto/mosquitto.conf"

# Caminho para o volume mapeado do Mosquitto
CONFIG_DEST="./server/configs/mosquitto/config/mosquitto.conf"

# Verifica se o arquivo de configuração existe
if [ -f "$CONFIG_SOURCE" ]; then
    echo "Copiando arquivo de configuração para o volume mapeado..."
    mkdir -p "$(dirname "$CONFIG_DEST")" # Cria o diretório, se necessário
    cp "$CONFIG_SOURCE" "$CONFIG_DEST"
    echo "Configuração copiada com sucesso!"
else
    echo "Erro: Arquivo de configuração '$CONFIG_SOURCE' não encontrado!"
    exit 1
fi