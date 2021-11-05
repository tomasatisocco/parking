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
  // put your main code here, to run repeatedly:
}
