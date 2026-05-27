#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
// Definição de pinos para o ESP8266
#define SS_PIN    2   // D4 (Leitor RFID)
#define RST_PIN   0   // D3 (Leitor RFID)
#define LED_PIN   5   // D1 (Luzes da Casa Toda)
 
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Cria o servidor invisível na porta padrão 80
ESP8266WebServer server(80);
 
// CONFIGURAÇÃO DO WIFI
// O roteador precisa estar em uma frequência específica (2.4 GHz)
const char* ssid = "NOME_DO_SEU_WIFI";
const char* password = "SENHA_DO_SEU_WIFI";

// CONFIGURAÇÃO DOS CARTÕES
String uidAutorizado = "00 00 00 00"; // Cartão Mestre / Admin (Faz tudo)
String led           = "00 00 00 00"; // Cartão que liga/desliga as Luzes
 
void setup() {
  Serial.begin(115200); 
  SPI.begin();
  mfrc522.PCD_Init(); 
 
  // Configura as luzes
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 

  // INICIALIZAÇÃO DO WI-FI
  Serial.println();
  Serial.print("Conectando na rede: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado com sucesso!");
  Serial.print("O Endereço IP da sua placa é: ");
  Serial.println(WiFi.localIP());

  // ROTAS DO SERVIDOR (Comandos do App React Native)
  server.on("/", []() {
    server.send(200, "text/plain", "Servidor da Maquete Online");
  });

  // Rota que liga a luz via WiFi
  server.on("/luz/on", []() {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Comando Wi-Fi: Luz LIGADA");
    server.send(200, "text/plain", "Luz ligada!");
  });

  // Rota que apaga a luz via WiFi
  server.on("/luz/off", []() {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Comando Wi-Fi: Luz DESLIGADA");
    server.send(200, "text/plain", "Luz desligada!");
  });

  server.begin();
  Serial.println("Sistema de Automação das Luzes Iniciado.");
}
 
void loop() {
  
 // O WiFi precisa ser a primeira linha para rodar sem travar
  server.handleClient(); 
 
  // CONTROLE FÍSICO POR CARTÕES RFID
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;
 
  String uidLida = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidLida.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uidLida.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uidLida.toUpperCase();
  uidLida.trim();
 
  Serial.print("Tag lida: ");
  Serial.println(uidLida); 
 
  // FUNÇÃO: CONTROLE DOS LEDS (VIA CARTÃO)
  if (uidLida == led || uidLida == uidAutorizado) {
    Serial.println("Comando Cartão: Alternando Luzes da Casa");
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(1000); // Pausa de 1 segundo para evitar leitura dupla do cartão
  } 
  
  // AÇÃO: ACESSO NEGADO
  else {
    Serial.println("Acesso Negado!");
    for(int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  }
 
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
