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
#define ABRIENDOENTRADA flag1.bit.b2
#define CERRANDOENTRADA flag1.bit.b3
#define ABRIENDOSALIDA flag1.bit.b4
#define CERRANDOSALIDA flag1.bit.b5

//Definincion de los distintos estados de Entrada/Salida
//Se utilizan los mismos nombres para ahorrar espacio pero en el caso de la Salida
// VEHICULOENTRANDO y VEHICULOINGRESADO se deben interpretar como SALIENDO y SALIDO

#define ESPERANDO 1
#define VEHICULODETECTADO 2
#define BARRERAABIERTA 3
#define VEHICULOENTRANDO 4
#define VEHICULOINGRESADO 5

// Definicion de Variables

uint8_t cantidadVehiculos,ultimoBoton, estadoEntrada, estadoSalida;
unsigned long time, ultimoDebounce, accionamientoMotorEntrada, accionamientoMotorSalida;

_flag flag1;

//Prototipado de funciones

void ChequearDebounce(int botonActual);
void PararEstacionamiento();
void HabilitarEstacionamiento();
void ResetarEstacionamiento();
void AbrirBarreraEntrada();
void CerrarBarreraEntrada();
void AbrirBarreraSalida();
void CerrarBarreraSalida();

//Esta serie de funciones es para salvar el efecto de debounce de los botones mecanicos
// e implementar al logica ante cada boton presionado

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
  digitalWrite(LUZROJA, HIGH);
  digitalWrite(LUZVERDE, LOW);
}

void HabilitarEstacionamiento(){
  ESTACIONAMIENTOHABILITADO = 0x01;
  digitalWrite(LUZROJA, LOW);
  digitalWrite(LUZVERDE, HIGH);
}

void ResetearEstaccionamineto(){
  cantidadVehiculos = 0;
  ESTACIONAMIENTOHABILITADO = 0x01;
}

//Para abrir y cerrar las barreras se supone que el tiempo de activacion de los motores es de 3 segundos
// para completar el recorrido, asi como que se utilizaran salidas distintas para poder invertir el sentido
// de gito de los motores

void AbrirBarreraEntrada(){
  time = millis();
  if (!ABRIENDOENTRADA){
    digitalWrite(APERTURAENTRADA, HIGH);
    ABRIENDOENTRADA = 0x01;
    accionamientoMotorEntrada = millis();
  } else if ((time - accionamientoMotorEntrada) >= 3000){
      digitalWrite(APERTURAENTRADA, LOW);
      ABRIENDOENTRADA = 0x00;
    }
}

void CerrarBarreraEntrada(){
  time = millis();
  if(!CERRANDOENTRADA){
    digitalWrite(CIERREENTRADA, HIGH);
    CERRANDOENTRADA = 0x01;
    accionamientoMotorEntrada = millis();
  } else if ((time - accionamientoMotorEntrada) >= 3000){
    digitalWrite(CIERREENTRADA, LOW);
    CERRANDOENTRADA = 0x00;
  }
}

void AbrirBarreraSalida(){
  time = millis();
  if (!ABRIENDOSALIDA){
    digitalWrite(APERTURASALIDA, HIGH);
    ABRIENDOSALIDA = 0x01;
    accionamientoMotorSalida = millis();
  } else if ((time - accionamientoMotorSalida) >= 3000){
      digitalWrite(APERTURASALIDA, LOW);
      ABRIENDOSALIDA = 0x00;
    }
}

void CerrarBarreraSalida(){
  time = millis();
  if(!CERRANDOSALIDA){
    digitalWrite(CIERRESALIDA, HIGH);
    CERRANDOSALIDA = 0x01;
    accionamientoMotorSalida = millis();
  } else if ((time - accionamientoMotorSalida) >= 3000){
    digitalWrite(CIERRESALIDA, LOW);
    CERRANDOSALIDA = 0x00;
  }
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

  // Cuando se inicia el micro se supone el estacionamiento vacio y deshabilitado
  // hasta que se accione el boton. Los estados basicos de los sensores son ESPERANDO

  ESTACIONAMIENTOHABILITADO = 0x00;
  digitalWrite(LUZROJA, HIGH);
  digitalWrite(LUZVERDE, LOW);
  estadoEntrada = ESPERANDO;
  estadoSalida = ESPERANDO;
  cantidadVehiculos = 0;
}

void loop() {
  // Todo el tiempo se esta chequeando si los botones son presionados o no
  LeerBotones();
  //Los sensores de entrada solamente leen cuando el estacionamiento esta habilitado
  // y tiene lugar. Los sensores solamente se leen si el estado actual de la entrada lo amerita.
  if (ESTACIONAMIENTOHABILITADO){
    if(!ESTACIONAMIENTOLLENO){
      switch (estadoEntrada) {
        case ESPERANDO:
          if (digitalRead(SENSORENTRADA1)){
            estadoEntrada = VEHICULODETECTADO;
          }
        break;
        case VEHICULODETECTADO:
          AbrirBarreraEntrada();
          if (!ABRIENDOENTRADA){
            estadoEntrada = BARRERAABIERTA;
          }
        break;
        case BARRERAABIERTA:
          if (digitalRead(SENSORENTRADA2)){
            estadoEntrada = VEHICULOENTRANDO;
          }
        break;
        case VEHICULOENTRANDO:
          if (!digitalRead(SENSORENTRADA2)){
            estadoEntrada = VEHICULOINGRESADO;
          }
        break;
        case VEHICULOINGRESADO:
          CerrarBarreraEntrada();
          if (++cantidadVehiculos >= 10){
            ESTACIONAMIENTOHABILITADO = 0x00;
            digitalWrite(LUZROJA, HIGH);
            digitalWrite(LUZVERDE, LOW);
          }
          if(!CERRANDOENTRADA){
            estadoEntrada = ESPERANDO;
          }
        break;
      }
    }
    // Los sensores de salida son leidos siempre que el estacionamiento este habilitado,
    // a diferencia de los sensores de entrada ya que los autos siempre salen.
    switch (estadoSalida) {
      case ESPERANDO:
        if (digitalRead(SENSORSALIDA1) && digitalRead(SENSORTARJETA)){
          estadoSalida = VEHICULODETECTADO;
        }
      break;
      case VEHICULODETECTADO:
        AbrirBarreraSalida();
        if (!ABRIENDOSALIDA){
          estadoSalida = BARRERAABIERTA;
        }
      break;
      case BARRERAABIERTA:
        if (digitalRead(SENSORSALIDA2)){
          estadoSalida = VEHICULOENTRANDO;
        }
      break;
      case VEHICULOENTRANDO:
        if (!digitalRead(SENSORSALIDA2)){
          estadoSalida = VEHICULOINGRESADO;
        }
      break;
      case VEHICULOINGRESADO:
        CerrarBarreraEntrada();
        if (++cantidadVehiculos <= 10){
          ESTACIONAMIENTOHABILITADO = 0x01;
          digitalWrite(LUZROJA, LOW);
          digitalWrite(LUZVERDE, HIGH);
        }
        if(!CERRANDOENTRADA){
          estadoSalida = ESPERANDO;
        }
      break;
    }
  }
}
