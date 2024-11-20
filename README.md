# StormForce

<p align="center">
  <img src="https://i.imgur.com/H9rz6mx.png" alt="Imagem logo" />
</p>

O Storm Force é um projeto de IoT desenvolvido para monitorar e registrar dados de umidade e temperatura de forma eficiente e em tempo real. Utilizando o microcontrolador ESP32, um sensor DHT11 e o protocolo MQTT (via broker Mosquitto), este sistema permite a coleta e publicação de dados em uma rede local ou na nuvem, facilitando o monitoramento remoto e a automação de processos.



## Requisitos de Hardware

- ESP32
- Sensor de Temperatura e Umidade DHT11
- Jumpers (fios de conexão)

## Requisitos de Software

- PlatformIO (preferido) ou Arduino IDE
- Mosquitto MQTT Broker
- Node-RED

## Configuração e Instalação

1. **Configuração de Hardware**: Conecte o sensor DHT11 ao ESP32. As conexões são as seguintes:

   - DHT11 VCC ao ESP32 3V3
   - DHT11 GND ao ESP32 GND
   - DHT11 DATA ao ESP32 GPIO 26

2. **Configuração de Software**:
   - Instale e configure o VSCode + PlatformIO (preferido) ou Arduino IDE.
   - Instale o Mosquitto MQTT Broker.
   - Instale o Node-RED.
   - Instale o Node-RED Dashboard.

3. **Envio do Código**: Faça o upload do código fornecido para o ESP32.

## Executando o Projeto

1. Inicie o broker Mosquitto.
2. Inicie o Node-RED e abra o dashboard fornecido.
3. Ligue o ESP32.

**Atenção**: Se estiver usando um microcontrolador diferente (como outro modelo de ESP ou Arduino), ajuste os pinos conforme necessário.

4. **Configuração do Código**: Antes de fazer o upload do código fornecido para o ESP32:
- Edite o código para configurar sua rede Wi-Fi. Substitua os placeholders pelo nome da sua rede Wi-Fi (SSID) e senha:
 ```cpp
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
```
   - Verifique e ajuste os pinos do microcontrolador para garantir que correspondem às conexões do hardware.

## MQTT Broker

Baixe e instale o Mosquitto MQTT Broker [aqui](https://mosquitto.org/download/).  
Você pode iniciar o broker com a configuração padrão executando `mosquitto` no terminal.

### Instalação do Mosquitto MQTT Broker

#### Debian e Ubuntu
Execute os seguintes comandos no terminal:
```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients -y
sudo systemctl enable mosquitto
sudo systemctl start mosquitto
```

### Fedora

```bash
sudo dnf install mosquitto mosquitto-clients -y
sudo systemctl enable mosquitto
sudo systemctl start mosquitto
```

### Arch Linux

```bash
sudo pacman -S mosquitto
sudo systemctl enable mosquitto
sudo systemctl start mosquitto
```




## Node-RED

Baixe e instale o Node-RED [aqui](https://nodered.org/docs/getting-started/local).  
Você pode iniciar o Node-RED executando `node-red` no terminal.

Criar um fluxo no Node-RED é muito fácil. Você pode encontrar mais informações sobre isso [aqui](https://nodered.org/docs/getting-started/first-flow).

### Importando o JSON dos Flows no Node-RED

1. Abra a interface do Node-RED acessando `http://localhost:1880` no navegador.
2. No canto superior direito, clique no botão de menu (três linhas horizontais) e selecione **Import**.
3. Cole o conteúdo do arquivo JSON do fluxo na janela que aparecer ou clique em **Upload** para enviar o arquivo JSON.
4. Clique em **Import** para adicionar o fluxo à sua área de trabalho.
5. Certifique-se de conectar os nós corretamente, se necessário.
6. Clique no botão **Deploy** no canto superior direito para ativar o fluxo.



## Node-RED Dashboard

Instale o Node-RED Dashboard [aqui](https://flows.nodered.org/node/node-red-dashboard).  
Você pode acessar o dashboard indo para `http://localhost:1880/ui` no seu navegador.

<p align="center">
  <img src="https://i.imgur.com/pIKp3qd.png" alt="Imagem logo" />
</p>



## Solução de Problemas

Certifique-se de seguir corretamente a configuração do hardware. Se estiver usando uma placa ESP32 diferente, os pinos GPIO podem variar.

Preste atenção à saída serial do ESP32. Ela é muito útil para depuração.

No arquivo `mosquitto.conf`, certifique-se de configurar a opção `allow_anonymous` como `true`.  
Além disso, configure o listener para `1883 0.0.0.0`, como mostrado abaixo.
<p align="center">
  <img src="https://i.imgur.com/eGgCjk2.png" alt="Imagem logo" />
</p>

