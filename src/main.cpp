#include <Arduino.h>

typedef union{
  struct{
    uint8_t b0:1;
    uint8_t b1:1;
    uint8_t b2:1;
    uint8_t b3:1;
    uint8_t b4:1;
    uint8_t b5:1;
    uint8_t b6:1;
    uint8_t b7:1;
  }bit;
  uint8_t byte;
}_flag;

// Definicion Variables de Entrada

#define SENSORENTRADA1 2
#define SENSORENTRADA2 3

#define SENSORSALIDA1 4
#define SENSORSALIDA2 5
#define SENSORTARJETA 6

#define BOTONM 7
#define BOTONR 8
#define BOTONP 9

//Definicion Variables de Salida

#define LUZROJA 10
#define LUZVERDE 11

#define APERTURAENTRADA 12
#define CIERREENTRADA 13
#define APERTURASALIDA 14
#define CIERRESALIDA 15

//Definicion de Banderas para los distintos Estados

#define ESTACIONAMIENTOLLENO flag1.bit.b0
#define ESTACIONAMIENTOHABILITADO flag1.bit.b1
#define ABRIENDO flag1.bit.b2

uint8_t cantidadVehiculos,ultimoBoton;
unsigned long time, ultimoDebounce, accionamientoMotorEntrada, accionamientoMotorSalida;

_flag flag1;

void ChequearDebounce(int botonActual);
void PararEstacionamiento();
void HabilitarEstacionamiento();
void ResetarEstacionamiento();

void CambiarLuz(){
  digitalWrite(LUZROJA, ESTACIONAMIENTOLLENO);
  digitalWrite(LUZVERDE, !ESTACIONAMIENTOLLENO);
}

void LeerBotones(){
  if(digitalRead(BOTONM) || digitalRead(BOTONR) || digitalRead(BOTONP)){
    if (digitalRead(BOTONP)){
      ChequearDebounce(BOTONP);
    }
    if (digitalRead(BOTONM)) {
      ChequearDebounce(BOTONM);
    }
    if (digitalRead(BOTONR)){
      ChequearDebounce(BOTONR);
    }
  } else {
    ultimoBoton = 0x00;
    ultimoDebounce = millis();
  }
}

void ChequearDebounce(int botonActual){
  time = millis();
  if ((time - ultimoDebounce) >= 30){
    if(botonActual ^ ultimoBoton){
      ultimoBoton = botonActual;
      switch (botonActual) {
        case BOTONP:
          PararEstacionamiento();
          break;
        case BOTONM:
          HabilitarEstacionamiento();
          break;
        case BOTONR:
          ResetarEstacionamiento();
          break;
        default:
          break;
      }
      ultimoDebounce = millis();
    } else {
      ultimoDebounce = millis();
    }
  }
}

void PararEstacionamiento(){
  ESTACIONAMIENTOHABILITADO = 0x00;
}

void HabilitarEstacionamiento(){
  ESTACIONAMIENTOHABILITADO = 0x01;
}

void ResetearEstaccionamineto(){
  cantidadVehiculos = 10;
  ESTACIONAMIENTOHABILITADO = 0x01;
}

void setup() {
  pinMode(SENSORENTRADA1, INPUT);
  pinMode(SENSORENTRADA2, INPUT);
  pinMode(SENSORSALIDA1, INPUT);
  pinMode(SENSORSALIDA2, INPUT);
  pinMode(SENSORTARJETA, INPUT);
  pinMode(BOTONM, INPUT);
  pinMode(BOTONR, INPUT);
  pinMode(BOTONP, INPUT);

  pinMode(LUZVERDE, OUTPUT);
  pinMode(LUZROJA, OUTPUT);
  pinMode(APERTURAENTRADA, OUTPUT);
  pinMode(CIERREENTRADA, OUTPUT);
  pinMode(APERTURASALIDA, OUTPUT);
  pinMode(CIERRESALIDA, OUTPUT);
}

void loop() {
  LeerBotones();
}
