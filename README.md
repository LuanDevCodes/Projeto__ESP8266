# Sistema de Automação Residencial com ESP8266 e RFID (Leitor de tags)

Este projeto consiste no desenvolvimento de um sistema de automação residencial para uma maquete acadêmica, utilizando o microcontrolador ESP8266 (NodeMCU). O objetivo principal é isolar e controlar funcionalidades distintas (iluminação e controle de acesso) de forma independente utilizando tags RFID (cartões e chaveiros).

## 🔧 Funcionalidades

- **Controle de Acesso (Porta Automática):** Utiliza um micro servo motor SG90 que atua como interruptor. Ao passar o cartão correto, a porta abre (ou fecha) e permanece na posição até que o cartão seja aproximado novamente.
- **Controle de Iluminação (Casa Toda):** Permite ligar e desligar um barramento de LEDs em paralelo simulando a iluminação interna da residência.
- **Segurança de Tensão:** O código implementa pausas cirúrgicas (`delay`) para estabilização da corrente antes do acionamento do motor, evitando quedas de tensão e reinicializações involuntárias da placa.
- **Feedback de Erro:** Caso um cartão não cadastrado seja aproximado, o sistema emite um alerta visual piscando os LEDs rapidamente.

## 🛠️ Componentes Utilizados

- 1x Placa ESP8266 NodeMCU v3
- 1x Leitor RFID RC522 (13.56MHz)
- 1x Micro Servo Motor SG90 9g
- Múltiplos LEDs
- 1x Protoboard e Jumpers para conexões

## 📌 Mapeamento de Pinos (Hardware)

Para evitar conflitos elétricos e de comunicação de hardware, as conexões foram distribuídas da seguinte forma:

| Componente | Pino no Componente | Pino no ESP8266 | Função |
| :--- | :--- | :--- | :--- |
| * | * | * | * |
| **SG90** | Fio (Laranja) do Servo | **D2** (GPIO 4) | Controle do Servo da Porta |
| | Fio (Vermelho) do Servo | **VIN/VU** | Fornece 5V do USB |
| | GND | **GND** | Linha de Terra Comum |
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

1. Instalar as bibliotecas `MFRC522` e `Servo` através do Gerenciador de Bibliotecas da IDE.
2. Configurar a velocidade do Monitor Serial para **115200 baud** para monitorar a leitura das tags sem problemas de caracteres corrompidos.
3. Mapear as strings de UID no topo do código com os respectivos IDs hexadecimais dos seus cartões:

```cpp
String uidAutorizado = "00 00 00 00"; // Cartão Master
String led           = "00 00 00 00"; // Cartão das Luzes
String porta         = "00 00 00 00"; // Cartão da Porta
