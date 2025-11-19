/*
 * ErgoSense - GS 2025
 * Conexões ESP32 no Wokwi - LCD I2C
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Configurações WiFi e MQTT
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

// Pinagem
const int trigPin = 5;    // GPIO 5 → TRIG
const int echoPin = 18;   // GPIO 18 → ECHO
const int ldrPin = 34;    // GPIO 34 → LDR
const int buttonPin = 2;  // GPIO 2 → Botão
const int buzzerPin = 4;  // GPIO 4 → Buzzer
const int ledR = 12;      // GPIO 12 → LED Vermelho
const int ledG = 13;      // GPIO 13 → LED Verde
const int ledB = 14;      // GPIO 14 → LED Azul

// *** LCD I2C - Endereço comum 0x27 ***
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  
  // Configurar pinos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT); 
  pinMode(ledB, OUTPUT);
  
  // *** INICIALIZAR LCD I2C ***
  lcd.init();          // Inicializa o LCD
  lcd.backlight();     // Liga a luz de fundo
  lcd.clear();         // Limpa o display
  lcd.setCursor(0, 0); // Posiciona cursor (coluna, linha)
  lcd.print("ErgoSense GS25");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  
  // Conectar WiFi e MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
  setRGB(0, 255, 0); // LED Verde
  
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pressione BTN");
  lcd.setCursor(0, 1);
  lcd.print("para comecar");
}

// ... (as outras funções permanecem iguais)

void startWorkSession() {
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Bom Trabalho!   ");
  lcd.setCursor(0, 1);
  lcd.print("Postura: OK     ");
  setRGB(0, 255, 0); // Verde
  client.publish("ErgoSense/Usuario1/status", "Sessao Iniciada");
}

void postureAlert() {
  lcd.setCursor(0, 1);
  lcd.print("MA POSTURA!    ");
  setRGB(255, 0, 0); // Vermelho
  client.publish("ErgoSense/Usuario1/status", "Alerta de Postura");
}

void clearPostureAlert() {
  lcd.setCursor(0, 1);
  lcd.print("Postura: OK     ");
}