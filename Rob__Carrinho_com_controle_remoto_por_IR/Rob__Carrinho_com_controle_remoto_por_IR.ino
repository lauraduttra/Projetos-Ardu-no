#include <SoftwareSerial.h>
#include <Servo.h>
#include <IRremote.h>

// IR
IRrecv irrecv(7);
decode_results results;
unsigned long key_value = 0;

// servo motor
#define SERVO 2
Servo servo;
//
// sensor distância 1
int sensorFrenteE = 9;
int sensorFrenteD = 10;
int sensorRe = 11;
//
// 74HC595
int DSdata = 4;
int STCPshift = 5;
int SHCPclock = 3;
//
int luzFrente[2] = {128, false}; 
int setaEsquerda[2] = {64, false};
int setaDireita[2] = {32, false};
int luzRe[2] = {16, false};
int motorFrente[2] = {8, false};
int motorRe[2] = {4, false};
int q6[2] = {2, false};
int q7[2] = {1, false};
int totalCi595Ligados = 0;

const long IGNICAO_CARRO = 0xFFA25D;
const long LUZ_FRENTE = 0xFF22DD;
const long SETA_ESQUERDA = 0xFDB04F;
const long SETA_DIREITA = 0xFD708F;
const long LUZ_RE = 0xFFC23D;
const long PISCA_ALERTA = 0xFFE21D;
const long MOTOR_FRENTE = 0xFF02FD;
const long MOTOR_RE = 0xFF9867;
const long DIRECAO_ESQUERDA = 0xFFE01F;
const long DIRECAO_CENTRO = 0xFFA857;
const long DIRECAO_DIREITA = 0xFF906F;
const long VELOCIDADE_1 = 0xFF30CF;
const long VELOCIDADE_2 = 0xFF18E7;
const long VELOCIDADE_3 = 0xFF7A85;
const long VELOCIDADE_4 = 0xFF10EF;
//
int pinMotor = 6;
int velocidade = 255;
char dadosSerial;
String dadosSerialString = "";
int indiceSerial = 0;
int direcaoCentro = 91;
int emAtividade = true;

class ci595
 {
    public:
        // seta esquerda e direita e alerta
         int piscar (int seta) {
            for (int i = 0; i < 3; i++){
                totalCi595Ligados += seta;
                digitalWrite(STCPshift, LOW);
                shiftOut(DSdata, SHCPclock, LSBFIRST, totalCi595Ligados);
                digitalWrite(STCPshift, HIGH);
                delay(500);
                totalCi595Ligados -= seta;
                digitalWrite(STCPshift, LOW);
                shiftOut(DSdata, SHCPclock, LSBFIRST, totalCi595Ligados);
                digitalWrite(STCPshift, HIGH);
                delay(500);
            }
             emAtividade = true;
        }
        // luz farol e luz de ré
        int ligar (int led) {
                digitalWrite(STCPshift, LOW);
                shiftOut(DSdata, SHCPclock, LSBFIRST, led);
                digitalWrite(STCPshift, HIGH);
              emAtividade = true;
        }
        // motor
        int motorDc (int motor) {
                analogWrite(pinMotor, velocidade);
                digitalWrite(STCPshift, LOW);
                shiftOut(DSdata, SHCPclock, LSBFIRST, motor);
                digitalWrite(STCPshift, HIGH);
              emAtividade = true;
        }
        // configuração geral dos pinos do ci595 para controlar indiviualmente os canais.
        int configure(int recurso[]){
          if (recurso[1] == false){
              totalCi595Ligados += recurso[0];
              recurso[1] = true;
          }else{
              totalCi595Ligados -= recurso[0];
              recurso[1] = false;
          }
          emAtividade = true;
        }
 };
ci595 ci595;
class ci293
{
  public:
  int velocidadeMotor(int valor){
          velocidade = valor*64-1;
          if(velocidade > 255){
              velocidade = 255;
            }
        ci595.motorDc(totalCi595Ligados);
  }
};
ci293 ci293;

void setup(){
    Serial.begin(9600);
    pinMode(STCPshift, OUTPUT);
    pinMode(SHCPclock, OUTPUT);
    pinMode(DSdata, OUTPUT);
    pinMode(pinMotor, OUTPUT);
    pinMode(sensorFrenteE, INPUT);
    irrecv.enableIRIn();
    //irrecv.blink13(true);
    servo.attach(SERVO);
    servo.write(91);
    Serial.println("Setup...");
}

void loop(){
  // Recebe sinal via infravermelho
  if (irrecv.decode(&results) && emAtividade){
      emAtividade = false;
        //Serial.println(results.value, HEX);
        key_value = results.value;
        disparaOpcao();
        delay(100);
        irrecv.resume(); 
  }
  // Recebe sinal via USB serial
    if (Serial.available() > 0) {
        // lê o dado recebido:
        dadosSerial = Serial.read();
        // responder o que foi recebido:
        Serial.println(dadosSerial);
        // envia caracter lido
        key_value = dadosSerial;
        disparaOpcao();
    }  
}

void disparaOpcao(){
  switch (key_value){
    case IGNICAO_CARRO: case 0xFD00FF: case '0':
    Serial.println("Ignicao carro...");
    ci595.configure(luzFrente);
    ci595.ligar(totalCi595Ligados);    
    break;
    case LUZ_FRENTE:  case 0xFD10EF: case 'q':
      Serial.println("Luz frente...");
        ci595.configure(luzFrente);
        ci595.ligar(totalCi595Ligados);
    break;
    case SETA_ESQUERDA: case 'w':
      Serial.println("Direcao esquerda...");
        ci595.piscar(setaEsquerda[0]);
    break;
    case SETA_DIREITA: case 'e':
      Serial.println("Seta direita...");
        ci595.piscar(setaDireita[0]);
    break;
    case LUZ_RE: case 0xFD50AF: case 'r':
    Serial.println("Luz re...");    
        ci595.configure(luzRe);
        ci595.ligar(totalCi595Ligados);
    break;
    case PISCA_ALERTA: case 0xFD40BF: case 't':
      Serial.println("Pisca alerta...");
        ci595.piscar(setaEsquerda[0] + setaDireita[0]);
    break;
    case MOTOR_FRENTE: case 0xFD807F: case 'y':
      Serial.println("Motor frente...");
        motorRe[1] ? ci595.configure(motorRe):
        ci595.configure(motorFrente);
        ci595.motorDc(totalCi595Ligados);
    break;
    case MOTOR_RE: case 0xFD906F: case 'u':
      Serial.println("Motor re...");
        motorFrente[1] ? ci595.configure(motorFrente):
        ci595.configure(motorRe);
        ci595.motorDc(totalCi595Ligados);
    break;
    case DIRECAO_ESQUERDA: case 0xFD609F: case 'i':
      Serial.println("Direcao esquerda...");
        servo.write(120);
      emAtividade = true;
    break;
    case DIRECAO_CENTRO: case 0xFDA05F: case 'o':
      Serial.println("Direcao centro...");
        servo.write(91);
      emAtividade = true;
    break;
    case DIRECAO_DIREITA: case 0xFD20DF: case 'p':
      Serial.println("Direcao direita...");
        servo.write(60);
      emAtividade = true;
    break;
    case VELOCIDADE_1: case 0xFD08F7: case '1':
      Serial.println("Velocidade 1...");
        ci293.velocidadeMotor(2);
    break;
  case VELOCIDADE_2: case 0xFD8877: case '2':
      Serial.println("Velocidade 2...");
        ci293.velocidadeMotor(3);
    break;
  case VELOCIDADE_3: case 0xFD48B7: case '3':
      Serial.println("Velocidade 3...");
        ci293.velocidadeMotor(4);
    break;
    default:
      Serial.print("Sem caso para: ");
        Serial.println(key_value, HEX);
      delay(100);
      emAtividade = true;
    break;
  }
}
