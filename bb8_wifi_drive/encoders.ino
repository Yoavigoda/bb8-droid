
void IRAM_ATTR encoderA_ISR()
{
  if(digitalRead(PIN_ENC_A_CH_A) != digitalRead(PIN_ENC_A_CH_B))
  {
    enc_A_count++;
  }
  else 
  {
    enc_A_count--;
  }
}
void IRAM_ATTR encoderB_ISR()
{
  if(digitalRead(PIN_ENC_B_CH_A) != digitalRead(PIN_ENC_B_CH_B))
  {
    enc_B_count++;
  }
  else 
  {
    enc_B_count--;
  }
}