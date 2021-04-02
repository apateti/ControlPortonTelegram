#pragma once
#ifndef commandTelegram_h
  #define commandTelegram_h

#ifndef Arduino_h
  #include <Arduino.h>
#endif
#ifndef ArduinoJson_h
    #include <ArduinoJson.h>
#endif
#ifndef HTTPClient_h
  #include <HTTPClient.h>
#endif
#ifndef model_h
  #include <model.h>
#endif
#ifndef header_h
  #include <header.h>
#endif
//
// String tokenBot         = "1163113539:AAEZfgENIoMnjdzN3kdBrqZiCGMjjC_ZcdU";
// String urlPing          = "api.telegram.org";
// String urlTelegram      = "https://api.telegram.org/bot";
// String comandGetUpdate  = "/getUpdates?offset=";
// String comandGet        = "/getUpdates";
// int32_t update_id       = 1;
// String comandGetMe      = "/getMe";
// String comandSendText   = "/sendMessage";
// String comandAnswQuery  = "answerInlineQuery";
// //
#define CommadSTART       "/start"
#define CommadREGISTRO    "/registrar"
#define CommadMENU        "/menu"
#define Commad_OpenP      "Abrir_Porton"
#define Commad_ClouseP    "Cerrar_Porton"
#define Commad_EstatusP   "Estatus_Porton"
//#define CommadLED_Status  "Estatus_LED"
#define Command_Temperatura "Temperatura"
#define Commad_Power      "Power_ON_OFF"
#define Commad_Ubicacion  "Ubicacion"
#define ComandTemp_Int    "Temp_Interna"
#define CommadOption      "/opciones"
// //query Keyboard
// String queryON          = "LedOn";
// String queryOFF         = "LedOff";
// String queryToggle      = "LedToggle";
// String queryStatus      = "LedStatus";

// #define BUFFER_BIG 2000
// DynamicJsonDocument root(BUFFER_BIG);

// int32_t  getUpdateID = 0;
// int32_t  updateID = 0;

extern String tokenBot;

extern int verBot(String botToken);
extern int requestGetUpdateClear(String token);
int requestGetUpdate(String token);
int sendComand(int32_t userId, String mensaje, String formato);
int sendMenu(int32_t userId);
int sendUbicacion(int32_t userId, float latit, float longit);

#endif