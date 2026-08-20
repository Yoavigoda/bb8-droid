#include <WiFi.h>
#include <WebServer.h>
#include "html_content.h"
const char *SSID = "BB8-Droid";
const char *PASSWORD = "12345678";
const uint8_t PIN_STBY = D2;
const uint8_t PIN_PWMA = D3;
const uint8_t PIN_AIN1 = D4;
const uint8_t PIN_AIN2 = D5;
const uint8_t PIN_PWMB = D6;
const uint8_t PIN_BIN1 = D7;
const uint8_t PIN_BIN2 = D8;
const uint8_t PIN_ENC_A_CH_A = D9; 
const uint8_t PIN_ENC_A_CH_B = D10;
const uint8_t PIN_ENC_B_CH_A = D11; 
const uint8_t PIN_ENC_B_CH_B = D12;
const unsigned long CONTROL_PERIOD_MS = 50;
unsigned long last_control_ms = 0; 
int base_speed = 0;
volatile long enc_A_count = 0;
volatile long enc_B_count = 0;
long last_enc_A = 0;
long last_enc_B = 0;
const float KP = 0.5;

void drive_f(uint8_t eng_num, int speed);
void controlUpdate();
void IRAM_ATTR encoderA_ISR();
void IRAM_ATTR encoderB_ISR();
void handleRoot();
void handleForward();
void handleBack();
void handleStop();
void handleCount();
void handleNotFound();
enum MOTOR{
  MOTOR_A = 0,
  MOTOR_B = 1
};
WebServer server(80);

void setup() {
  pinMode(PIN_STBY,OUTPUT);//1
  pinMode(PIN_PWMA,OUTPUT);//2
  pinMode(PIN_AIN1,OUTPUT);//3
  pinMode(PIN_AIN2,OUTPUT);//4
  pinMode(PIN_PWMB,OUTPUT);//5
  pinMode(PIN_BIN1,OUTPUT);//6
  pinMode(PIN_BIN2,OUTPUT);//7
  pinMode(PIN_ENC_A_CH_A, INPUT);
  pinMode(PIN_ENC_A_CH_B, INPUT);
  pinMode(PIN_ENC_B_CH_A, INPUT);
  pinMode(PIN_ENC_B_CH_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A_CH_A), encoderA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_B_CH_A), encoderB_ISR, CHANGE);
  analogWrite(PIN_PWMA, 0);
  analogWrite(PIN_PWMB, 0);
  digitalWrite(PIN_AIN1,LOW);//3
  digitalWrite(PIN_AIN2,LOW);//4
  digitalWrite(PIN_BIN1,LOW);//6
  digitalWrite(PIN_BIN2,LOW);//7
  digitalWrite(PIN_STBY,HIGH);//7
  Serial.begin(115200);
  delay(100);
  WiFi.mode(WIFI_AP);
  bool start_ap = WiFi.softAP(SSID, PASSWORD);
  if(start_ap)
  {
    Serial.println("start");
  }
  else
  {
    Serial.println("didnt start");
  }
  IPAddress ip = WiFi.softAPIP();
  Serial.println(ip);
  server.on("/",HTTP_GET, handleRoot);
  server.on("/forward", HTTP_GET, handleForward);
  server.on("/stop", HTTP_GET, handleStop);
  server.on("/backword", HTTP_GET, handleBack);
  server.on("/count", HTTP_GET, handleCount);
  server.onNotFound(handleNotFound);
  server.begin();

}

void loop() {
  server.handleClient();
  unsigned long cur_millis = millis();
  if ((cur_millis - last_control_ms) >= CONTROL_PERIOD_MS)
  {
    last_control_ms = cur_millis;
    controlUpdate();
  }
}


