void controlUpdate()
{
   
  if(base_speed == 0)
  {
    drive_f(MOTOR_A, 0);
    drive_f(MOTOR_B, 0);
    last_enc_A = 0;
    last_enc_B = 0;
    return;
  }

  long cur_enc_A_count = enc_A_count; 
  long cur_enc_B_count = enc_B_count;
  long delta_A = cur_enc_A_count - last_enc_A; 
  long delta_B = cur_enc_B_count - last_enc_B; 
  last_enc_A = cur_enc_A_count;
  last_enc_B = cur_enc_B_count;
  long err = delta_A -delta_B;
  int correct = (int)(KP * err);
  drive_f(MOTOR_A, base_speed - correct);
  drive_f(MOTOR_B, base_speed + correct); 
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
