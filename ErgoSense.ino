/*
 * ErgoSense - GS 2025
 * Versão que funciona SEM WiFi
 */

#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// Pinagem
const int trigPin = 5;    // GPIO 5 → TRIG
const int echoPin = 18;   // GPIO 18 → ECHO
const int ldrPin = 34;    // GPIO 34 → LDR
const int buttonPin = 2;  // GPIO 2 → Botão
const int buzzerPin = 4;  // GPIO 4 → Buzzer
const int ledR = 12;      // GPIO 12 → LED Vermelho
const int ledG = 13;      // GPIO 13 → LED Verde
const int ledB = 14;      // GPIO 14 → LED Azul

// LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variáveis de tempo
unsigned long workStartTime = 0;
bool isWorking = false;
bool isOnBreak = false;
const long workInterval = 15000;  // 15s para teste (25min real)
const long breakInterval = 5000;  // 5s para teste (5min real)

// --- DECLARAÇÃO DE FUNÇÕES ---
void setRGB(int r, int g, int b);
long readUltrasonic();
void startWorkSession();
void startBreak();
void endBreak();
void postureAlert();
void clearPostureAlert();
void lightAlert();

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
  
  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ErgoSense GS25");
  
  setRGB(0, 255, 0); // LED Verde
  
  // *** CONEXÃO WiFi NÃO-BLOQUEANTE ***
  Serial.println("Iniciando WiFi...");
  WiFi.begin("Wokwi-GUEST", "");
  
  // Não espera pela conexão - continua o programa
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Ativo!");
  lcd.setCursor(0, 1);
  lcd.print("Pressione BTN");
  
  Serial.println("Sistema ErgoSense Pronto!");
  Serial.println("WiFi em segundo plano...");
}

void loop() {
  // Verifica WiFi apenas uma vez (não bloqueia)
  static bool wifiChecked = false;
  if (!wifiChecked && WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Conectado!");
    wifiChecked = true;
  }
  
  // Lê o botão de presença
  if (digitalRead(buttonPin) == LOW) {
    if (!isWorking) {
      startWorkSession();
    }
    isWorking = true;
    workStartTime = millis();
    delay(300); // Debounce
  }

  // Lógica de Pausa (Pomodoro)
  if (isWorking && !isOnBreak) {
    if (millis() - workStartTime > workInterval) {
      startBreak();
    }
  }
  if (isOnBreak) {
    if (millis() - workStartTime > (workInterval + breakInterval)) {
      endBreak();
    }
  }

  // Monitoramento de Postura
  long distance = readUltrasonic();
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if (distance < 40 || distance > 80) {
    postureAlert();
  } else {
    clearPostureAlert();
  }

  // Monitoramento de Luminosidade
  int light = analogRead(ldrPin);
  Serial.print("Luminosidade: ");
  Serial.println(light);
  
  if (light < 1000) {
    lightAlert();
  }

  delay(1000);
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---
void setRGB(int r, int g, int b) {
  analogWrite(ledR, r);
  analogWrite(ledG, g);
  analogWrite(ledB, b);
}

long readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void startWorkSession() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bom Trabalho!   ");
  lcd.setCursor(0, 1);
  lcd.print("Postura: OK     ");
  setRGB(0, 255, 0); // Verde
  Serial.println("STATUS: Sessao Iniciada");
}

void startBreak() {
  isOnBreak = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PAUSA! Alongue-se");
  setRGB(255, 255, 0); // Amarelo
  tone(buzzerPin, 1000, 500);
  Serial.println("STATUS: Em Pausa");
}

void endBreak() {
  isOnBreak = false;
  isWorking = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pausa Finalizada!");
  setRGB(0, 255, 0); // Verde
  tone(buzzerPin, 2000, 500);
  Serial.println("STATUS: Pronto para voltar");
}

void postureAlert() {
  lcd.setCursor(0, 1);
  lcd.print("MA POSTURA!    ");
  setRGB(255, 0, 0); // Vermelho
  tone(buzzerPin, 500, 300);
  Serial.println("ALERTA: Postura Ruim!");
}

void clearPostureAlert() {
  lcd.setCursor(0, 1);
  lcd.print("Postura: OK     ");
}

void lightAlert() {
  lcd.setCursor(0, 1);
  lcd.print("POUCA LUZ!     ");
  Serial.println("ALERTA: Pouca Luz!");
}