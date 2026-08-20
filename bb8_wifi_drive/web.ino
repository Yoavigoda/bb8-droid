    
void handleCount()
{
  String str_enc_A_count = String(enc_A_count);
  String str_enc_B_count = String(enc_B_count);
  String msg = str_enc_A_count + "," + str_enc_B_count; 
  server.send(200, "text/plain", msg);
}

void handleForward()
{

  server.send(200, "text/plain", "forward");
  base_speed = 150;  
}

void handleBack()
{
  server.send(200, "text/plain", "backword");
  base_speed = -150;  
}

void handleStop()
{  
  server.send(200, "text/plain", "stop");
  base_speed = 0;
}

void handleRoot()
{
  server.send(200, "text/html",String(INDEX_HTML));
}

void handleNotFound()
{
  server.send(404, "text/plain","code 404");
} 
