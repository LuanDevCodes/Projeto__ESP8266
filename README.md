# Sistema de Automação Residencial com ESP8266 e RFID (Leitor de tags) 💳

Este projeto consiste no desenvolvimento de um sistema de automação residencial para uma maquete acadêmica, utilizando o microcontrolador ESP8266 (NodeMCU). O objetivo principal é isolar e controlar funcionalidades de forma independente utilizando tags RFID (cartões e chaveiros).

## 🔧 Funcionalidades

- **Controle de Iluminação (Casa Toda):** Permite ligar e desligar um barramento de LEDs em paralelo simulando a iluminação interna da residência.
- **Feedback de Erro:** Caso um cartão não cadastrado seja aproximado, o sistema emite um alerta visual piscando os LEDs rapidamente.

## 🛠️ Componentes Utilizados

- 1x Placa ESP8266 NodeMCU v3
- 1x Leitor RFID RC522 (13.56MHz)
- Múltiplos LEDs
- 1x Protoboard e Jumpers para conexões

## 📌 Mapeamento de Pinos (Hardware)

Para evitar conflitos elétricos e de comunicação de hardware, as conexões foram distribuídas da seguinte forma:

| Componente | Pino no Componente | Pino no ESP8266 | Função |
| :--- | :--- | :--- | :--- |
| * | * | * | * |
| **Os LED's** | LED's | **D1** (GPIO 5) | Acionamento das Luzes |
| * | * | * | * |
| **Leitor RFID** | SDA | **D4** (GPIO 2) | Comunicação SPI |
| | SCK | **D5** (GPIO 14) | Comunicação SPI |
| | MOSI | **D7** (GPIO 13) | Comunicação SPI |
| | MISO | **D6** (GPIO 12) | Comunicação SPI |
| | GND | **GND** | Linha de Terra Comum |
| | RST | **D3** (GPIO 0) | Reset do Módulo |
| | 3.3V | **3V3** | Alimentação Logica (Nunca usar 5V) |

## 💻 Configuração do Software

O código foi desenvolvido na Arduino IDE. Para o funcionamento correto, certifique-se de:

1. Instalar a biblioteca `MFRC522` através do Gerenciador de Bibliotecas da IDE.
2. Configurar a velocidade do Monitor Serial para **115200 baud** para monitorar a leitura das tags sem problemas de caracteres corrompidos.
3. Mapear as strings de UID no topo do código com os respectivos IDs hexadecimais dos seus cartões:

```cpp
String uidAutorizado = "00 00 00 00"; // Cartão Master
String led           = "00 00 00 00"; // Cartão das Luzes
