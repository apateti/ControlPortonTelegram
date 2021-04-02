#pragma once
#ifndef commandWifi_h
    #define commandWifi_h

#ifndef Arduino_h
  #include <Arduino.h>
#endif
#include <WebServer.h>

#define NoComd      0
//#define ToggleLED 1
#define OpenP       1
#define ClouseP     2
#define StatusP     3
#define AccPoint    4
#define ConectAcPo  5
#define tokenJson   6
#define verUser     7
#define erraseAll   8
#define grabUser    9
#define registUser  10
#define verRegistro 11
#define erraseUser  12
#define blockUser   13
#define credenUser  14
#define StatusBot   15
#define powerPort   16
#define temperature 17
#define modeSTA     18

bool conectAP(String ssid, String pass);
void configServer();
bool serverJsonIsOK();
void requestError();
int verCmdo(String& ssidConfig, String& passConfig, String& tokenBot);
void sendRequest(int codeRequest, String strJson);
void verClient();




#endif