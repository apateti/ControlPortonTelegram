#pragma once
#ifndef utils_eeprom_h
    #define utils_eeprom_h

#ifndef Arduino_h
  #include <Arduino.h>
#endif

#define EEPROM_SIZE   2048
#define SSID_SIZE     32
#define PASS_SIZE     64
#define SIZE_IP       4
#define SIZE_GATEWAY  4
#define SIZE_SUBNET   4
#define SIZE_DNS1     4
#define SIZE_DNS2     4
#define TOKEN_SIZE    46
#ifndef USER_ALL
//Hay que copiar en el Archivo TelegramESP32.h
  #define USER_ALL    40
  #define USER_SIZE   4 
  #define USER_ALL_SIZE (USER_ALL * USER_SIZE)

  #define USER_Name_SIZE 32
  #define USER_Name_ALL_SIZE (USER_Name_SIZE * USER_ALL)

  #define USER_Blocked_SIZE 01
#endif
#define SSID_ADDR   0
#define PASS_ADDR   SSID_SIZE
#define IP_ADDR     SSID_SIZE + PASS_SIZE
#define IP_GATEWAY  SSID_SIZE + PASS_SIZE + SIZE_IP
#define IP_SUBNET   SSID_SIZE + PASS_SIZE + SIZE_IP + SIZE_GATEWAY
#define IP_DNS1     SSID_SIZE + PASS_SIZE + SIZE_IP + SIZE_GATEWAY + SIZE_SUBNET
#define IP_DNS2     SSID_SIZE + PASS_SIZE + SIZE_IP + SIZE_GATEWAY + SIZE_SUBNET + SIZE_DNS1
#define TOKEN_ADDR  SSID_SIZE + PASS_SIZE + SIZE_IP + SIZE_GATEWAY + SIZE_SUBNET + SIZE_DNS1 + SIZE_DNS2
//
#define USER_0      TOKEN_ADDR + TOKEN_SIZE
#define USER_1      USER_0 + USER_SIZE
#define USER_2      USER_0 + (2 * USER_SIZE)
#define USER_3      USER_0 + (3 * USER_SIZE)
#define USER_4      USER_0 + (4 * USER_SIZE)
#define USER_5      USER_0 + (5 * USER_SIZE)
#define USER_6      USER_0 + (6 * USER_SIZE)
#define USER_7      USER_0 + (7 * USER_SIZE)
#define USER_8      USER_0 + (8 * USER_SIZE)
#define USER_9      USER_0 + (9 * USER_SIZE)
#define USER_10      USER_0 + (10 * USER_SIZE)
#define USER_11      USER_0 + (11 * USER_SIZE)
#define USER_12      USER_0 + (12 * USER_SIZE)
#define USER_13      USER_0 + (13 * USER_SIZE)
#define USER_14      USER_0 + (14 * USER_SIZE)
#define USER_15      USER_0 + (15 * USER_SIZE)
#define USER_16      USER_0 + (16 * USER_SIZE)
#define USER_17      USER_0 + (17 * USER_SIZE)
#define USER_18      USER_0 + (18 * USER_SIZE)
#define USER_19      USER_0 + (19 * USER_SIZE)
//Ultimo Usuario
#define USER_END    USER_0 + ((USER_ALL) * USER_SIZE)
//
#define USER_Name_0  USER_END + (0 * USER_Name_SIZE)
#define USER_Name_1  USER_END + (1 * USER_Name_SIZE)
#define USER_Name_2  USER_END + (2 * USER_Name_SIZE)
#define USER_Name_3  USER_END + (3 * USER_Name_SIZE)
#define USER_Name_4  USER_END + (4 * USER_Name_SIZE)
#define USER_Name_5  USER_END + (5 * USER_Name_SIZE)
#define USER_Name_6  USER_END + (6 * USER_Name_SIZE)
#define USER_Name_7  USER_END + (7 * USER_Name_SIZE)
#define USER_Name_8  USER_END + (8 * USER_Name_SIZE)
#define USER_Name_9  USER_END + (9 * USER_Name_SIZE)
#define USER_Name_10  USER_END + (10 * USER_Name_SIZE)
#define USER_Name_11  USER_END + (11 * USER_Name_SIZE)
#define USER_Name_12  USER_END + (12 * USER_Name_SIZE)
#define USER_Name_13  USER_END + (13 * USER_Name_SIZE)
#define USER_Name_14  USER_END + (14 * USER_Name_SIZE)
#define USER_Name_15  USER_END + (15 * USER_Name_SIZE)
#define USER_Name_16  USER_END + (16 * USER_Name_SIZE)
#define USER_Name_17  USER_END + (17 * USER_Name_SIZE)
#define USER_Name_18  USER_END + (18 * USER_Name_SIZE)
#define USER_Name_19  USER_END + (19 * USER_Name_SIZE)
//Ultimo
#define USER_Name_END  USER_END + ((USER_ALL) * USER_Name_SIZE)
//
#define USER_Bloc_0  USER_Name_END + (0 * USER_Blocked_SIZE)
#define USER_Bloc_1  USER_Name_END + (1 * USER_Blocked_SIZE)
#define USER_Bloc_2  USER_Name_END + (2 * USER_Blocked_SIZE)
#define USER_Bloc_3  USER_Name_END + (3 * USER_Blocked_SIZE)
#define USER_Bloc_4  USER_Name_END + (4 * USER_Blocked_SIZE)
#define USER_Bloc_5  USER_Name_END + (5 * USER_Blocked_SIZE)
#define USER_Bloc_6  USER_Name_END + (6 * USER_Blocked_SIZE)
#define USER_Bloc_7  USER_Name_END + (7 * USER_Blocked_SIZE)
#define USER_Bloc_8  USER_Name_END + (8 * USER_Blocked_SIZE)
#define USER_Bloc_9  USER_Name_END + (9 * USER_Blocked_SIZE)
#define USER_Bloc_10  USER_Name_END + (10 * USER_Blocked_SIZE)
#define USER_Bloc_11  USER_Name_END + (11 * USER_Blocked_SIZE)
#define USER_Bloc_12  USER_Name_END + (12 * USER_Blocked_SIZE)
#define USER_Bloc_13  USER_Name_END + (13 * USER_Blocked_SIZE)
#define USER_Bloc_14  USER_Name_END + (14 * USER_Blocked_SIZE)
#define USER_Bloc_15  USER_Name_END + (15 * USER_Blocked_SIZE)
#define USER_Bloc_16  USER_Name_END + (16 * USER_Blocked_SIZE)
#define USER_Bloc_17  USER_Name_END + (17 * USER_Blocked_SIZE)
#define USER_Bloc_18  USER_Name_END + (18 * USER_Blocked_SIZE)
#define USER_Bloc_19  USER_Name_END + (19 * USER_Blocked_SIZE)
//ulyimo
#define USER_Bloc_END  USER_Name_END  + ((USER_ALL) * USER_Blocked_SIZE)

bool EEPROMinit();
void erraseALL(void);
String readSSID();
String readPASS();
void writeSSID(String ssidSTA);
void writePASS(String passSTA);
String readTOKEN();
void writeTOKEN(String tokenBot);
void readAllUser();
void writeUser_i(int index);
void readUser_i(int index);
void writeUserName_i(int index, String userName);
void readAllUserName(void);
void readAllUserRegPen(void);
void erraseToken(void);
void writeUserBlock_i(uint8_t index, bool valor);
void readAllUserBlock(void);
IPAddress readIP(void);
IPAddress readGATEWAY(void);
IPAddress readSUBNET(void);
IPAddress readdns1(void);
IPAddress readdns2(void);
void writeIP(IPAddress ipEEPROM);
void writeGATEWAY(IPAddress ipGateway);
void writeSUBNET(IPAddress ipSubnet);
void writeDNS1(IPAddress ipDNS1);
void writeDNS2(IPAddress ipDNS2);


#endif