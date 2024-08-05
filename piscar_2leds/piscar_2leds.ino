 //programa que envia sinal auto para a saída 7, aguarda um segundo, e envia sinal baixo para a saída 7
  void setup (){
  // put your setup code here, to run once:
  pinMode (7,OUTPUT);
  pinMode (2,OUTPUT);
  //porta configurada com saída
}

void loop() {
  //put your main code here, to run repeatedly
  digitalWrite(7,HIGH);
  delay(1000);
  digitalWrite(2,LOW)
  delay(1000);
  digitalWrite (7,LOW);
  delay (1000)
  digitalWrite (2,HIGH)
  
}
