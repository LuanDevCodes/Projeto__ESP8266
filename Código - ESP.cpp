#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
// Definição de pinos para o ESP8266
#define SS_PIN    2   // D4 (Leitor RFID)
#define RST_PIN   0   // D3 (Leitor RFID)
#define SERVO_PIN 4   // D2 (Micro Servo da Porta)
#define LED_PIN   5   // D1 (Luzes da Casa Toda)
 
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo meuServoPorta; // Objeto que controla o motor da porta
 
// CONFIGURAÇÃO DOS 3 CARTÕES
String uidAutorizado = "00 00 00 00"; // Cartão Mestre / Admin (Faz tudo)
String led           = "00 00 00 00"; // Cartão que liga/desliga as Luzes
String porta         = "00 00 00 00"; // Cartão que abre/fecha a Porta

// Variável que controla o estado atual da porta
bool portaAberta = false; // A porta começa fechada (falso)
 
void setup() {

  Serial.begin(115200); // É a velocidade de comunicação da placa ESP padrão, é preciso pois com isso evitamos os ícones quebrados no terminal
  SPI.begin();
  mfrc522.PCD_Init(); // Inicializa o leitor de cartões
 
  // Configura as luzes como uma saída de energia da placa
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // A maquete começa com as luzes apagadas 
 
  // configuração do motor: Valores 500 e 2400 calibram o SG90 e evitam tremedeira
  meuServoPorta.attach(SERVO_PIN, 500, 2400);
  meuServoPorta.write(90); // A porta começa trancada/centralizada em 90 graus
 
  Serial.println("Sistema de Automação Residencial Iniciado");
}
 
void loop() {

  // Verifica se há algum cartão se aproximando do leitor
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;
 
  // Bloco que faz a leitura do código único (UID) do cartão
  String uidLida = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidLida.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uidLida.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uidLida.toUpperCase();
  uidLida.trim();
 
  // Mostra no computador qual cartão foi lido
  Serial.print("Tag lida: ");
  Serial.println(uidLida); // curiosidade: o "println" é responsável por pular o fim da linha e não o começo, se eu tirar o código do serial vai colar com o comando,
                          // como um "\n" no fim de um print de String
 
  // ==========================================================
  // FUNÇÃO: CONTROLE DOS LEDS (CASA TODA)
  // ==========================================================

  if (uidLida == led || uidLida == uidAutorizado) {
    Serial.println("Comando: Alternando Luzes da Casa");
    
    // O comando '!' inverte o estado atual (se ligado, desliga; se desligado, liga)
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    
    delay(1000); // Pausa de 1 segundo para evitar que o leitor leia o mesmo cartão duas vezes seguidas
  } 
  
  // ==========================================================
  // FUNÇÃO: CONTROLE DA PORTA AUTOMÁTICA
  // ==========================================================

  else if (uidLida == porta || uidLida == uidAutorizado) {
    
    // segurança: Pausa de 300ms para a energia estabilizar
    delay(300); 
    
    if (portaAberta == false) { // Se a porta está fechada
      Serial.println("Abrindo a porta...");
      meuServoPorta.write(0);  // Move o motor para abrir a porta
      portaAberta = true;      // Atualiza o sistema dizendo que agora está aberta
    } 
    else {                      // Se a porta NÃO está fechada (ou seja, está aberta)
      Serial.println("Fechando a porta...");
      meuServoPorta.write(90); // Move o motor de volta para trancar
      portaAberta = false;     // Atualiza o sistema dizendo que agora fechou
    }
    
    delay(500); // Espera o motor terminar de se mover com segurança
    digitalWrite(LED_PIN, LOW); // Apaga o LED indicador de leitura (caso vocês usem algum)
    
    delay(1000); // Pausa de segurança contra leituras repetidas muito rápidas
  }
  
  // ==========================================================
  // AÇÃO: ACESSO NEGADO
  // ==========================================================

  else {
    
    Serial.println("Acesso Negado!");
    
    // Pisca os LEDs da casa 3 vezes rapidamente para avisar que o cartão é inválido
    for(int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  }
 
  // Comandos obrigatórios da biblioteca para preparar o leitor para o próximo cartão
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}