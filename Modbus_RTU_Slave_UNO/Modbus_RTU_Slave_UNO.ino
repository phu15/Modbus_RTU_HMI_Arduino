/**
 *  Modbus slave example 2:
 *  The purpose of this example is to link the Arduino digital and analog
 *	pins to an external device.
 *
 *  Recommended Modbus Master: QModbus
 *  http://qmodbus.sourceforge.net/
 *
 *  Editado al español por LuxARTS
 */

//Incluye la librería del protocolo Modbus
#include <ModbusRtu.h>
#define ID   1

//Crear instancia
Modbus slave(ID, Serial, 8); //ID del nodo. 0 para el master, 1-247 para esclavo
                        //Puerto serie (0 = TX: 1 - RX: 0)
                        //Serial protocol. 0 for RS-232 + USB (default), any pin greater than 1 for RS-485
boolean led;
int8_t state = 0;
unsigned long tempus;

uint16_t au16data[9]; //La tabla de registros que se desea compartir por la red

/*********************************************************
 Configuración del programa
*********************************************************/
void setup() {
  io_setup(); //configura las entradas y salidas

  Serial.begin(9600); //Open communication as a slave
  slave.start();
  tempus = millis() + 100; //Guarda el tiempo actual + 100ms
  digitalWrite(13, HIGH ); //Prende el led del pin 13 (el de la placa)
}

/*********************************************************
 Program start
*********************************************************/
void loop() {
  //Check the input buffer
  int i;
  state = slave.poll( au16data, 9 ); //Parameters: Table of records for the exchange of info
                                     //            Log table size
                                     //Returns 0 if there is no data request
                                     //Returns 1 to 4 if there was a communication error
                                     //Returns more than 4 if the order was processed correctly

  if (state > 4) { //If it is greater than 4 = the order was correct
    tempus = millis() + 50; //Tiempo actual + 50ms
   
  }
  
  
  //Actualiza los pines de Arduino con la tabla de Modbus
  io_poll();


} 

/**
 * pin maping:
 * 2 - digital input
 * 3 - digital input
 * 4 - digital input
 * 5 - digital input
 * 6 - digital output
 * 7 - digital output
 * 8 - digital output
 * 9 - digital output
 * 10 - analog output
 * 11 - analog output
 * 14 - analog input
 * 15 - analog input
 *
 * pin 13 reservado para ver el estado de la comunicación
 */
void io_setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(6, LOW );
  digitalWrite(7, LOW );
  digitalWrite(8, LOW );
  digitalWrite(9, LOW );
 // digitalWrite(13, HIGH ); //Led del pin 13 de la placa
  analogWrite(10, 0 ); //PWM 0%
  analogWrite(11, 0 ); //PWM 0%
}

/*********************************************************
Enlaza la tabla de registros con los pines
*********************************************************/
void io_poll() {
  // digital inputs -> au16data[0]
  // Reads the digital inputs and saves them in bits of the first variable of the vector
  // it's the same as making a mask
 // bitWrite( au16data[0], 0, digitalRead( 2 )); //Read Arduino pin 13 and save it to bit 0 of variable au16data [0] 
 // bitWrite( au16data[0], 1, digitalRead( 3 ));
 // bitWrite( au16data[0], 2, digitalRead( 4 ));
 // bitWrite( au16data[0], 3, digitalRead( 5 ));

  // digital outputs -> au16data[1]
  // Reads the bits of the second variable and puts them on the digital outputs
  digitalWrite( 13, bitRead( au16data[1], 0 )); //Read bit 0 of variable au16data [1] and put it on pin 6 of Arduino
  digitalWrite( 7, bitRead( au16data[6], 1 ));
  digitalWrite( 8, bitRead( au16data[6], 2 ));
  digitalWrite( 9, bitRead( au16data[6], 3 ));

  // Change the PWM value
  analogWrite( 10, au16data[2] ); //The value of au16data [2] is written to the PWM output of pin 10 of the Arduino. (being 0 = 0% and 255 = 100%)
  analogWrite( 11, au16data[3] );

  // Reads the analog inputs (ADC)
  au16data[4] = analogRead( 2 ); //The analog value read at pin A0 is saved in au16data [4]. (where 0 = 0v and 1023 = 5v)
  au16data[5] = analogRead( 1 );

  // Communication diagnosis (for debug)
  //au16data[6] = slave.getInCnt();  //Returns how many messages were received
 // au16data[7] = slave.getOutCnt(); //Returns how many messages were transmitted
  //au16data[8] = slave.getErrCnt(); //Returns how many errors there were
}
