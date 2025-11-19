# ErgoSense - Sistema Inteligente de Ergonomia
## GS - (TEMA)
###  Nome e RM 

## Problema
Trabalho remoto e híbrido levam a problemas de saúde como má postura, falta de pausas e ambiente inadequado, impactando bem-estar e produtividade.

## Solução
Uma estação inteligente com ESP32 que monitora postura, luminosidade e ambiente, alerta o usuário e promove pausas regulares, integrando-se a um dashboard via MQTT.

## Funcionalidades
- Monitoramento de postura com sensor ultrassônico.
- Controle de luminosidade ambiental.
- Alertas para pausas seguindo a técnica Pomodoro.
- Dashboard em tempo real via MQTT.

## Instruções de Uso (Wokwi)
1. Acesse o link do projeto: [LINK DO WOKWI AQUI]
2. Compile e execute a simulação.
3. Para ver os dados MQTT, abra a serial ou use um cliente MQTT externo conectado ao broker `test.mosquitto.org` e inscreva-se no tópico `ErgoSense/#`.

## Tópicos MQTT Utilizados
- `ErgoSense/Usuario1/distancia`: Valor da distância do sensor ultrassônico.
- `ErgoSense/Usuario1/luminosidade`: Valor do sensor de luz (LDR).
- `ErgoSense/Usuario1/temperatura`: Valor da temperatura (DHT).
- `ErgoSense/Usuario1/status`: Status geral ("Trabalhando", "Em Pausa", "Postura Ruim").

## Circuito
![Diagrama do Circuito](docs/diagrama_schematic.png)

## Código Fonte
O código principal está em `src/ErgoSense.ino`.