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
volatile long encAcount = 0;
void drive_f(uint8_t eng_num, int speed);
enum MOTOR{
  MOTOR_A = 0,
  MOTOR_B = 1
};
WebServer server(80);
void handleCount()
{
  String str_encAcount = String(encAcount);
  server.send(200, "text/plain", str_encAcount);
}
void IRAM_ATTR encoderA_ISR()
{
  if(digitalRead(PIN_ENC_A_CH_A) == digitalRead(PIN_ENC_A_CH_B))
  {
    encAcount++;
  }
  else 
  {
    encAcount--;
  }
}
void drive_f(uint8_t eng_num, int speed)
{
  uint8_t in_pinpwmc = 0;
  uint8_t in_pin1 = 0;
  uint8_t in_pin2 = 0;
  if(eng_num == MOTOR_A)
  {
    in_pinpwmc = PIN_PWMA;
    in_pin1 = PIN_AIN1;
    in_pin2 = PIN_AIN2;
  }
  else if(eng_num == MOTOR_B)
  {
    in_pinpwmc = PIN_PWMB;
    in_pin1 = PIN_BIN1;
    in_pin2 = PIN_BIN2;
  }
  else
  {
    return;
  }

  speed = constrain(speed, -255,255);
  if(speed  >0)
  {
    digitalWrite(in_pin1, HIGH);
    digitalWrite(in_pin2, LOW);

  }
  else if(speed < 0)
  {
    digitalWrite(in_pin2, HIGH);
    digitalWrite(in_pin1, LOW);
  }
  else
  {
    digitalWrite(in_pin1, LOW);
    digitalWrite(in_pin2, LOW);
  }
  analogWrite(in_pinpwmc, abs(speed)); 
}

void handleForward()
{

  server.send(200, "text/plain", "forward");
  Serial.println("motor A forward");
  drive_f(MOTOR_A, 150);
  Serial.println("motor B forward");
  drive_f(MOTOR_B, 150);  
}

void handleBack()
{
  server.send(200, "text/plain", "backword");
  Serial.println("motor A backword");
  drive_f(MOTOR_A, -150);
  Serial.println("motor B backword");
  drive_f(MOTOR_B, -150);  
}

void handleStop()
{  
  server.send(200, "text/plain", "stop");
  Serial.println("motor A stop");
  drive_f(MOTOR_A, 0);
  Serial.println("motor B stop");
  drive_f(MOTOR_B, 0);  
}

void handleRoot()
{
  server.send(200, "text/html",String(INDEX_HTML));
}

void handleNotFound()
{
  server.send(404, "text/plain","code 404");
} 

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
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A_CH_A), encoderA_ISR, CHANGE);
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
}


