#pragma once
#ifndef header_h
    #define header_h

#define DEBUG_MODE          1        //Si es >0 se Activa Debug, se imprime por  Terminalls Logs
#define BUFFER_BIG          2000     //Tamaño del Buffer para Json
//#define EEPROM_SIZE 1024      //Tamaño del Buffer para la EEPROM
#define statusLED           2
#define statusLED_Ext       16
//#define bottonProg          0        //Led de la Tarjeta ESP32 DEV
#define bottonProg          4       //Led de la Tarjeta ESP32 DEV
//#define bottonProg          18       //Led de la Tarjeta ESP32 DEV
//#define LED 15
#define powerPorton         5
/////////////////////////////////////////////////////////////////////
//  Señales de Entradas del Porton
/////////////////////////////////////////////////////////////////////
#define SensorPC        14       //Sensor Porton Clouse (SPC)
#define SensorPO        13      //Sensor Porton Open (SPO)
#define SenPorAct       27      //Sensor si se Activo Porton Local
#define SenPorPower     26      //Sensor si Porton esta Encendido
/////////////////////////////////////////////////////////////////////
//  Señal de Salida del Porton
/////////////////////////////////////////////////////////////////////
#define ActPortonOutput 15      //Salida para Activar Motor Porton (APO)
/////////////////////////////////////////////////////////////////////
//  Se definen las Lecturas de las Señales del Porton
/////////////////////////////////////////////////////////////////////
#define isPortonOpen    !digitalRead(SensorPO)      //Si SPO == 0, esta Abierto
#define isPortonClouse  !digitalRead(SensorPC)      //Si SPC == 0, esta Cerrado

#define noPortonOpen    digitalRead(SensorPO)       //Si SPO == 1, no esta Abierto
#define noPortonClouse  digitalRead(SensorPC)       //Si SPC == 1, no esta Cerrado

#define isActManualPor  !digitalRead(SenPorAct)
#define isPortonON      !digitalRead(SenPorPower)
/////////////////////////////////////////////////////////////////////
//  Se define la Activacion de Señal del Porton
/////////////////////////////////////////////////////////////////////
#define noActPorton     digitalWrite(ActPortonOutput, LOW)      //Activa Motor Inactiva
#define siActPorton     digitalWrite(ActPortonOutput, HIGH)     //Se Activa Motor

extern String ssid;
extern String pass;

extern String userName;
extern int32_t userID;
extern String adminUser;
extern int32_t admidId;
extern bool    userBlock;
extern uint8_t regID;
extern uint8_t countFSM;
//extern uint8_t countTime;
extern bool    ActPorton;


extern TaskHandle_t TaskWifi;

// uint16_t countStatusLed;
// bool flagConfig = false;
// bool flag1Vez = true;
//uint16_t countProg = 0;

// void readAllUser(void);

#endif