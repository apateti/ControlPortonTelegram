//#pragma once
#include "utils_eeprom.h"
#include <EEPROM.h>
#ifndef model_h
    #include "model.h"
#endif
#ifndef header_h
  #include "header.h"
#endif

bool EEPROMinit(){
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    #if DEBUG_MODE > 0
      Serial.println("failed to initialise EEPROM");
    #endif
    return (false);
  }else{
    #if DEBUG_MODE > 0
      Serial.println("OK to initialise EEPROM");
    #endif
    return (true);
  }
}

void erraseALL(void){
    for (int i = 0; i < EEPROM_SIZE; i++){
        EEPROM.write(i, 0xFF);
    }
    EEPROM.commit();
}

String readSSID(){
  String ssidSTA;
  char in[32];
  char curIn;
  curIn = EEPROM.read(SSID_ADDR);
  if(curIn == 0xFF || curIn == 0x00){
    #if DEBUG_MODE > 0
      Serial.println("SSID[0] es 0 o FF");
    #endif
    ssidSTA = "";
    return (ssidSTA);
  }
  #if DEBUG_MODE > 0
    Serial.println("read SSID: ");
  #endif
  for (int i = 0; i < SSID_SIZE; i++){
    curIn = EEPROM.read(i);
    in[i] = curIn;
    if(curIn == 0x00)
      break;
    #if DEBUG_MODE > 0
      Serial.print(in[i]);
    #endif
  }
  ssidSTA = String(in);
  return (ssidSTA);
}
String readPASS(){
  String passSTA;
  char in[PASS_SIZE];
  char curIn;
  curIn = EEPROM.read(PASS_ADDR);
  if(curIn == 0xFF || curIn == 0x00){
    #if DEBUG_MODE > 0
      Serial.println("PASS[0] es 0 o FF");
    #endif
    passSTA = "";
    return (passSTA);
  }
  #if DEBUG_MODE > 0
    Serial.println("\nread PASS: ");
  #endif
  for (int i = 0; i < PASS_SIZE; i++){
    curIn = EEPROM.read(i + PASS_ADDR);
    in[i] = curIn;
    if(curIn == 0x00)
      break;
    #if DEBUG_MODE > 0
      Serial.print(curIn);
    #endif
  }
  passSTA = String(in);
  return (passSTA);
}

void writeSSID(String ssidSTA){
  if(ssidSTA == ""){
    return;
  }
  #if DEBUG_MODE > 0
    Serial.print("write SSID: ");
  #endif
  for (int i = 0; i < SSID_SIZE; i++){
    EEPROM.write(i, ssidSTA[i]);
    if(ssidSTA[i] == 0x00)
      break;
    #if DEBUG_MODE > 0
      Serial.print(ssidSTA[i]);
    #endif
  }
  EEPROM.commit();
}

void writePASS(String passSTA){
  if(passSTA == ""){
    return;
  }
  #if DEBUG_MODE > 0
    Serial.print("\nwrite PASS: ");
  #endif
  for (int i = 0; i < PASS_SIZE; i++){
    EEPROM.write(i + PASS_ADDR, passSTA[i]);
    if(passSTA[i] == 0x00)
      break;
    #if DEBUG_MODE > 0
      Serial.print(passSTA[i]);
    #endif
  }
  EEPROM.commit();
}

IPAddress readIP(void){
  IPAddress ipEEPROM(0,0,0,0);
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = EEPROM.read(IP_ADDR);
  ip1 = EEPROM.read(IP_ADDR + 1);
  ip2 = EEPROM.read(IP_ADDR + 2);
  ip3 = EEPROM.read(IP_ADDR + 3);
  ipEEPROM = {ip0, ip1, ip2, ip3};
  return ipEEPROM;
}

IPAddress readGATEWAY(void){
  IPAddress ipGateway;
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = EEPROM.read(IP_GATEWAY);
  ip1 = EEPROM.read(IP_GATEWAY + 1);
  ip2 = EEPROM.read(IP_GATEWAY + 2);
  ip3 = EEPROM.read(IP_GATEWAY + 3);
  ipGateway = {ip0, ip1, ip2, ip3};
  return ipGateway;
}

IPAddress readSUBNET(void){
  IPAddress ipSubnet;
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = EEPROM.read(IP_SUBNET);
  ip1 = EEPROM.read(IP_SUBNET + 1);
  ip2 = EEPROM.read(IP_SUBNET + 2);
  ip3 = EEPROM.read(IP_SUBNET + 3);
  ipSubnet = {ip0, ip1, ip2, ip3};
  return ipSubnet;
}

IPAddress readdns1(void){
  IPAddress ipSubnet;
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = EEPROM.read(IP_DNS1);
  ip1 = EEPROM.read(IP_DNS1 + 1);
  ip2 = EEPROM.read(IP_DNS1 + 2);
  ip3 = EEPROM.read(IP_DNS1 + 3);
  ipSubnet = {ip0, ip1, ip2, ip3};
  return ipSubnet;
}

IPAddress readdns2(void){
  IPAddress ipSubnet;
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = EEPROM.read(IP_DNS2);
  ip1 = EEPROM.read(IP_DNS2 + 1);
  ip2 = EEPROM.read(IP_DNS2 + 2);
  ip3 = EEPROM.read(IP_DNS2 + 3);
  ipSubnet = {ip0, ip1, ip2, ip3};
  return ipSubnet;
}

void writeIP(IPAddress ipEEPROM){
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = ipEEPROM[0];
  ip1 = ipEEPROM[1];
  ip2 = ipEEPROM[2];
  ip3 = ipEEPROM[3];
  #if DEBUG_MODE > 0
    Serial.print("IP a grabar: "); 
    Serial.print(ip0); Serial.print("."); 
    Serial.print(ip1); Serial.print("."); 
    Serial.print(ip2); Serial.print("."); 
    Serial.print(ip3);
  #endif
  EEPROM.write(IP_ADDR, ip0);
  EEPROM.write(IP_ADDR + 1, ip1);
  EEPROM.write(IP_ADDR + 2, ip2);
  EEPROM.write(IP_ADDR + 3, ip3);
  EEPROM.commit();
}

void writeGATEWAY(IPAddress ipGateway){
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = ipGateway[0];
  ip1 = ipGateway[1];
  ip2 = ipGateway[2];
  ip3 = ipGateway[3];
  #if DEBUG_MODE > 0
    Serial.print("IPGateway a grabar: "); 
    Serial.print(ip0); Serial.print("."); 
    Serial.print(ip1); Serial.print("."); 
    Serial.print(ip2); Serial.print("."); 
    Serial.print(ip3);
  #endif
  EEPROM.write(IP_GATEWAY, ip0);
  EEPROM.write(IP_GATEWAY + 1, ip1);
  EEPROM.write(IP_GATEWAY + 2, ip2);
  EEPROM.write(IP_GATEWAY + 3, ip3);
  EEPROM.commit();
}

void writeSUBNET(IPAddress ipSubnet){
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = ipSubnet[0];
  ip1 = ipSubnet[1];
  ip2 = ipSubnet[2];
  ip3 = ipSubnet[3];
  #if DEBUG_MODE > 0
    Serial.print("SubNet a grabar: "); 
    Serial.print(ip0); Serial.print("."); 
    Serial.print(ip1); Serial.print("."); 
    Serial.print(ip2); Serial.print("."); 
    Serial.print(ip3);
  #endif
  EEPROM.write(IP_SUBNET, ip0);
  EEPROM.write(IP_SUBNET + 1, ip1);
  EEPROM.write(IP_SUBNET + 2, ip2);
  EEPROM.write(IP_SUBNET + 3, ip3);
  EEPROM.commit();
}

void writeDNS1(IPAddress ipDNS1){
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = ipDNS1[0];
  ip1 = ipDNS1[1];
  ip2 = ipDNS1[2];
  ip3 = ipDNS1[3];
  #if DEBUG_MODE > 0
    Serial.print("DNS1 a grabar: "); 
    Serial.print(ip0); Serial.print("."); 
    Serial.print(ip1); Serial.print("."); 
    Serial.print(ip2); Serial.print("."); 
    Serial.print(ip3);
  #endif
  EEPROM.write(IP_DNS1, ip0);
  EEPROM.write(IP_DNS1 + 1, ip1);
  EEPROM.write(IP_DNS1 + 2, ip2);
  EEPROM.write(IP_DNS1 + 3, ip3);
  EEPROM.commit();
}

void writeDNS2(IPAddress ipDNS2){
  uint8_t ip0, ip1, ip2, ip3;
  ip0 = ipDNS2[0];
  ip1 = ipDNS2[1];
  ip2 = ipDNS2[2];
  ip3 = ipDNS2[3];
  #if DEBUG_MODE > 0
    Serial.print("DNS2 a grabar: "); 
    Serial.print(ip0); Serial.print("."); 
    Serial.print(ip1); Serial.print("."); 
    Serial.print(ip2); Serial.print("."); 
    Serial.print(ip3);
  #endif
  EEPROM.write(IP_DNS2, ip0);
  EEPROM.write(IP_DNS2 + 1, ip1);
  EEPROM.write(IP_DNS2 + 2, ip2);
  EEPROM.write(IP_DNS2 + 3, ip3);
  EEPROM.commit();
}

String readTOKEN(){
  String tokenBot;
  char in[TOKEN_SIZE+1];
  char curIn;
  curIn = EEPROM.read(TOKEN_ADDR);
  if(curIn == 0xFF || curIn == 0x00){
    Serial.println("tokenBot[0] es 0 o FF");
    tokenBot = "";
    return (tokenBot);
  }
  #if DEBUG_MODE > 0
    Serial.println("read TOKEN: ");
  #endif
  for (int i = 0; i < TOKEN_SIZE; i++){
    curIn = EEPROM.read(i + TOKEN_ADDR);
    in[i] = curIn;
    if(curIn == 0x00)
      break;
    #if DEBUG_MODE > 0
      Serial.print(curIn);
    #endif
  }
  in[TOKEN_SIZE] = '\0';
  tokenBot = String(in);
  return (tokenBot);
}

void writeTOKEN(String tokenBot){
  if(tokenBot == ""){
    return;
  }
  Serial.print("\nwrite TOKEN: ");
  for (int i = 0; i < TOKEN_SIZE; i++){
    EEPROM.write(i + TOKEN_ADDR, tokenBot[i]);
    if(tokenBot[i] == 0x00)
      break;
    Serial.print(tokenBot[i]);
  }
  EEPROM.commit();
}

void erraseToken(void){
  #if DEBUG_MODE > 0
    Serial.print("\nBorrando TOKEN: ");
  #endif
  for (int i = 0; i < TOKEN_SIZE; i++){
    EEPROM.write(i + TOKEN_ADDR, 0xFF);
  }
  EEPROM.commit();
}

void readAllUser(){
    uint32_t valor;
    char caract[5];
    #if DEBUG_MODE > 0
      Serial.print("Addr Inicial USER ID: "); Serial.println(USER_0);
    #endif
    for (int i = 0; i < USER_ALL; i++)
    {
        caract[0] = EEPROM.read(USER_0+(i*4));
        caract[1] = EEPROM.read(USER_0+(i*4)+1);
        caract[2] = EEPROM.read(USER_0+(i*4)+2);
        caract[3] = EEPROM.read(USER_0+(i*4)+3);
        caract[4] = '\0';
        valor = uint8_t(caract[3])*0x1000000 + uint8_t(caract[2])*0x0010000 + uint8_t(caract[1])*0x0000100 + uint8_t(caract[0]);
        #if DEBUG_MODE > 0
          Serial.print("User ID: "); Serial.print(i); Serial.print(": ");
          Serial.println(valor);
        #endif
        cuentaUser.account[i] = valor;
    }
}

void writeAllUser(){
    
  uint32_t valor;
  
  for (int i = 0; i < USER_ALL; i++)
  {
    valor = cuentaUser.account[i];
    EEPROM.write((i * 4) + USER_0, uint8_t(valor));
    EEPROM.write((i * 4)+1 + USER_0, uint8_t(valor >> 8));
    EEPROM.write((i * 4)+2 + USER_0, uint8_t(valor >> 16));
    EEPROM.write((i * 4)+3 + USER_0, uint8_t(valor >> 24));
  }
  EEPROM.commit();
}

void readUser_i(int index){
  Serial.println("Rutina readUser_i");
    uint32_t valor;
    char caract[5];
    caract[0] = EEPROM.read(USER_0+(index*4));
    #if DEBUG_MODE > 0
      Serial.print(uint8_t(caract[0]));
      Serial.print(',');
    #endif
    caract[0+1] = EEPROM.read(USER_0+(index*4)+1);
    #if DEBUG_MODE > 0
      Serial.print(uint8_t(caract[1]));
      Serial.print(',');
    #endif
    caract[0+2] = EEPROM.read(USER_0+(index*4)+2);
    #if DEBUG_MODE > 0
      Serial.print(uint8_t(caract[2]));
      Serial.print(',');
    #endif
    caract[0+3] = EEPROM.read(USER_0+(index*4)+3);
    #if DEBUG_MODE > 0
      Serial.print(uint8_t(caract[3]));
      Serial.println(',');
      caract[0+4] = '\0';
    #endif
    valor = uint8_t(caract[3])*0x1000000 + uint8_t(caract[2])*0x0010000 + uint8_t(caract[1])*0x0000100 + uint8_t(caract[0]);
    cuentaUser.account[index] = valor;
    #if DEBUG_MODE > 0
      Serial.print("Valor Leido de la EEPROM: ");
      Serial.println(valor);
    #endif
}

void writeUser_i(int index){
    #if DEBUG_MODE > 0
      Serial.println("Rutina writeUser_i");
    #endif
    uint32_t valor;
    valor = cuentaUser.account[index];
    #if DEBUG_MODE > 0
      Serial.println(valor);
    #endif
    EEPROM.write((index * 4) + USER_0, uint8_t(valor));
    #if DEBUG_MODE > 0
      Serial.print("Dir_i: "); Serial.println(uint8_t((index * 4) + USER_0));
      Serial.print("Dat_i: "); Serial.println(uint8_t(valor));
    #endif
    EEPROM.write((index * 4)+1 + USER_0, uint8_t(valor >> 8));
    #if DEBUG_MODE > 0
      Serial.print("Dir_i: "); Serial.println(uint8_t((index * 4)+1 + USER_0));
      Serial.print("Dat_i: "); Serial.println(uint8_t(valor >> 8));
    #endif
    EEPROM.write((index * 4)+2 + USER_0, uint8_t(valor >> 16));
    #if DEBUG_MODE > 0
      Serial.print("Dir_i: "); Serial.println(uint8_t((index * 4)+2 + USER_0));
      Serial.print("Dat_i: "); Serial.println(uint8_t(valor >> 16));
    #endif
    EEPROM.write((index * 4)+3 + USER_0, uint8_t(valor >> 24));
    #if DEBUG_MODE > 0
      Serial.print("Dir_i: "); Serial.println(uint8_t((index * 4)+3 + USER_0));
      Serial.print("Dat_i: "); Serial.println(uint8_t(valor >> 24));
    #endif
    EEPROM.commit();
}

void writeUserName_i(int index, String userName){
    #if DEBUG_MODE > 0
      Serial.println("Rutina writeUserName_i");
    #endif
    char valor[USER_Name_SIZE+1];
    //valor = userName;
    userName.toCharArray(valor, sizeof(valor));
    #if DEBUG_MODE > 0
      Serial.print("\nuserName a Grabar EEPROM:");
      Serial.println(valor);
    #endif
    //valor = cuentaUser.account[index];
    #if DEBUG_MODE > 0
      Serial.print("Direcciones a Grabar: ");
    #endif
    for (uint8_t i = 0; i < USER_Name_SIZE; i++)
    {
      EEPROM.write(i + (USER_Name_0 + (USER_Name_SIZE*index)), char(valor[i]));
      #if DEBUG_MODE > 0
        Serial.print(i + (USER_Name_0 + (USER_Name_SIZE*index))); Serial.print(",");
      #endif
      if(valor[i] == '\0'){
        break;
      }
    }
    #if DEBUG_MODE > 0
      Serial.println("");
      // EEPROM.write((index * 4) + USER_Name_0, char(valor[0]));
      // Serial.print(char(valor[0]));
      // Serial.print(',');
      // EEPROM.write((index * 4)+1 + USER_Name_0, char(valor[1]));
      // Serial.print(char(valor[1]));
      // Serial.print(',');
      // EEPROM.write((index * 4)+2 + USER_Name_0, char(valor[1]));
      // Serial.print(char(valor[1]));
      // Serial.print(',');
      // EEPROM.write((index * 4)+3 + USER_Name_0, char(valor[1]));
      // Serial.print(char(valor[1]));
      // Serial.println(',');
    #endif
    EEPROM.commit();
}

// void writePASS(String passSTA){
//   if(passSTA == ""){
//     return;
//   }
//   Serial.print("\nwrite PASS: ");
//   for (int i = 0; i < PASS_SIZE; i++){
//     EEPROM.write(i + PASS_ADDR, passSTA[i]);
//     if(passSTA[i] == 0x00)
//       break;
//     Serial.print(passSTA[i]);
//   }
//   EEPROM.commit();
// }

void readAllUserName(void){
    String ssidSTA;
    char in[USER_Name_SIZE+1];
    char curIn;
    #if DEBUG_MODE > 0
      Serial.print("Addr Inicial USER_Name: "); Serial.println(USER_Name_0);
    #endif
    for (int i = 0; i < USER_ALL; i++)
    {
      for (uint8_t j = 0; j < USER_Name_SIZE; j++){
        if(j == 0){
          curIn = EEPROM.read(j + (USER_Name_0 + (USER_Name_SIZE*i)));
          if(curIn == 0xFF || curIn == 0x0){
            in[j] = '\0';
            break;
          }else{
            in[j] = curIn;
          }
        }
        curIn = EEPROM.read(j + (USER_Name_0 + (USER_Name_SIZE*i)));
        in[j] = curIn;
        if(curIn == 0)
          break;
      }
      cuentaUserName.account[i] = String(in);
    }
    #if DEBUG_MODE > 0
      for (int i = 0; i < USER_ALL; i++){
        Serial.print("User Name "); Serial.print(i); Serial.print(": ");
        Serial.println(cuentaUserName.account[i]);
      }
    #endif
}

void readAllUserBlock(void){
  uint8_t curIn;
  #if DEBUG_MODE > 0
    Serial.print("Addr Inicial USER_Block: "); Serial.println(USER_Bloc_0);
  #endif
  for (int i = 0; i < USER_ALL; i++){
    curIn = EEPROM.read((USER_Bloc_0 + (USER_Blocked_SIZE*i)));
    if(curIn == 0xFF)
      cuentaUserBlock.account[i] = true;
    else
      cuentaUserBlock.account[i] = false;
  }
}

void writeUserBlock_i(uint8_t index, bool valor){
  uint8_t curIn;
  curIn = EEPROM.read((USER_Bloc_0 + (USER_Blocked_SIZE*index)));
  if((valor == true && curIn == 0xFF) || (valor == false && curIn == 0x0)){
    return;
  }
  if(valor == true){
    curIn = 0xFF;
    cuentaUserBlock.account[index] = true;
    #if DEBUG_MODE > 0
      Serial.print("B: true: "); Serial.println((USER_Bloc_0 + (USER_Blocked_SIZE*index)));
    #endif
  }else{
    curIn = 0x0;
    cuentaUserBlock.account[index] = false;
    #if DEBUG_MODE > 0
      Serial.print("B: true: "); Serial.println((USER_Bloc_0 + (USER_Blocked_SIZE*index)));
    #endif
  }
  EEPROM.write((USER_Bloc_0 + (USER_Blocked_SIZE*index)), curIn);
  EEPROM.commit();
}

void readAllUserRegPen(void){

}
