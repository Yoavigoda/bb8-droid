const uint8_t PIN_STBY = D2;
const uint8_t PIN_PWMA = D3;
const uint8_t PIN_AIN1 = D4;
const uint8_t PIN_AIN2 = D5;
const uint8_t PIN_PWMB = D6;
const uint8_t PIN_BIN1 = D7;
const uint8_t PIN_BIN2 = D8; 
enum MOTOR{
  MOTOR_A = 0,
  MOTOR_B = 1
};

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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIN_STBY,OUTPUT);//1
  pinMode(PIN_PWMA,OUTPUT);//2
  pinMode(PIN_AIN1,OUTPUT);//3
  pinMode(PIN_AIN2,OUTPUT);//4
  pinMode(PIN_PWMB,OUTPUT);//5
  pinMode(PIN_BIN1,OUTPUT);//6
  pinMode(PIN_BIN2,OUTPUT);//7
  analogWrite(PIN_PWMA, 0);
  analogWrite(PIN_PWMB, 0);
  digitalWrite(PIN_AIN1,LOW);//3
  digitalWrite(PIN_AIN2,LOW);//4
  digitalWrite(PIN_BIN1,LOW);//6
  digitalWrite(PIN_BIN2,LOW);//7
  digitalWrite(PIN_STBY,HIGH);//7

  Serial.println("driver awake, motors stopped");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("motor A foward");
  drive_f(MOTOR_A, 128);
  delay(5000);
  Serial.println("motor A stop");
  drive_f(MOTOR_A, 0);
  delay(5000);
  Serial.println("motor A backwards");
  drive_f(MOTOR_A, -128);
  delay(5000);
  Serial.println("motor A stop");
  drive_f(MOTOR_A, 0);
  delay(5000);
  
  Serial.println("motor B foward");
  drive_f(MOTOR_B, 128);
  delay(5000);
  Serial.println("motor B stop");
  drive_f(MOTOR_B, 0);
  delay(5000);
  Serial.println("motor B backwards");
  drive_f(MOTOR_B, -128);
  delay(5000);
  Serial.println("motor B stop");
  drive_f(MOTOR_B, 0);
  delay(5000);

}
