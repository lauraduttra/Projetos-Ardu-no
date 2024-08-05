/*******************************************************************************
*
*    Projeto 58: Sorteio de 0 a 9 - display 7 segmentos - Arduino
*                 http://squids.com.br/arduino                
*
*******************************************************************************/
byte displaySeteSeg[10][4] = { 
 { 0,0,0,0 },  //DIGITO 0
 { 1,0,0,0 },  //DIGITO 1
 { 0,1,0,0 },  //DIGITO 2
 { 1,1,0,0 },  //DIGITO 3
 { 0,0,1,0 },  //DIGITO 4
 { 1,0,1,0 },  //DIGITO 5
 { 0,1,1,0 },  //DIGITO 6
 { 1,1,1,0 },  //DIGITO 7
 { 0,0,0,1 },  //DIGITO 8
 { 1,0,0,1 }  //DIGITO 9
};

byte botao=12;
 
void setup(){
  for (int i = 4; i < 8; i++)
  {
    pinMode(i, OUTPUT);
  }

  pinMode(botao, INPUT_PULLUP);
}

void loop() {

  //Geração de numero aleatorios de 0 a 9
  long n = random(0, 10);
  numero_display(n);
  delay(50);

   //Se o botão foi pressionado…
  if (!digitalRead(botao)) {
    delay(500);
    //O processo do arduino é interrompido e o display mostra o numero sorteado.    
    while (digitalRead(botao)) {
      } // loop vazio faz com que o programa interropa o processo até sair da condição, ou seja, pressionar o botão
    delay(400); // Delays são usados para que de tempo para a informação do botão ser lida apenas uma vez. 
  }
}

// função para mostrar número
void numero_display(int m) {

  for (int j = 4; j < 8; j++) {
    digitalWrite(j, displaySeteSeg[m][j-4]);
  }
}
