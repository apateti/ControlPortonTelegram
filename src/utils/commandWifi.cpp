//#pragma once
#include <WiFi.h>
//#include <WebServer.h>
#include <ArduinoJson.h>
#ifndef header_h
  #include "header.h"
#endif
//#define  DEBUG_MODE 1
#ifndef commandWifi_h
    #include "commandWifi.h"
#endif
#ifndef model_h
    #include "model.h"
#endif
#ifndef utils_eeprom_h
  #include utils_eeprom.h
#endif
WebServer server(80);


bool conectAP(String ssid, String pass){
  uint8_t mac[6];
  char nameHost[22];
  WiFi.macAddress(mac);
  sprintf(nameHost, "Home_IoT_Port-%02X%02X%02X", mac[3], mac[4], mac[5]);
  #if DEBUG_MODE > 0
    Serial.print("\nHost Name: ");
    Serial.println(nameHost);
    Serial.println("\nRutina conectAP");
  #endif
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  #if DEBUG_MODE > 0
    Serial.println("Configurado WIFI_STA");
    //ssid = wifiEeprom.readSSID();
    //ssid = readSSID();
  #endif
    if(WiFi.setHostname(nameHost)){
      #if DEBUG_MODE > 0
      Serial.println("Host Name1 Exitoso: ");
      delay(1);
      Serial.println(WiFi.getHostname());
      #endif
    }else{
      #if DEBUG_MODE > 0
      Serial.println("Host Name1 Fallo: ");
      delay(1);
      Serial.println(WiFi.getHostname());
      #endif
    }
  //WiFi.setHostname(nameHost);
  if(ssid == ""){
    return false;
  }
  //pass = wifiEeprom.readPASS();
  //pass = readPASS();
  IPAddress addrIP;
  IPAddress addrGat;
  IPAddress addrSub;
  IPAddress primaryDNS(8, 8, 8, 8); //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional
  addrIP = readIP();
  addrGat = readGATEWAY();
  addrSub = readSUBNET();
  primaryDNS = readdns1();
  secondaryDNS = readdns2();
  #if DEBUG_MODE > 0
    Serial.print("IP de la EEPROM: "); Serial.println(addrIP);
    Serial.print("Gateway de la EEPROM: "); Serial.println(addrGat);
    Serial.print("SubNet de la EEPROM: "); Serial.println(addrSub);
    Serial.print("DNS1 de la EEPROM: "); Serial.println(primaryDNS);
    Serial.print("DNS2 de la EEPROM: "); Serial.println(secondaryDNS);
  #endif
  if(addrIP[0] != 0xFF && addrIP[0] != 0x0){
    addrGat = readGATEWAY();
    addrSub = readSUBNET();
    primaryDNS = readdns1();
    secondaryDNS = readdns2();
    if (!WiFi.config(addrIP, addrGat, addrSub, primaryDNS, secondaryDNS)){
      #if DEBUG_MODE > 0
        Serial.println("STA Fallo la configuracion de IP Fija");
      #endif
    }else{
      #if DEBUG_MODE > 0
        Serial.println("STA Exitosa la configuracion de IP Fija");
      #endif
    }
  }else{
    #if DEBUG_MODE > 0
      Serial.println("No hay en la EEPROM la Conf de IP Fija");
    #endif
  }
  #if DEBUG_MODE > 0
    Serial.println("Conectandose a:");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(pass);
  #endif
  if(pass == ""){
    WiFi.begin(ssid.c_str());
    delay(1);
    if(WiFi.setHostname(nameHost)){
      #if DEBUG_MODE > 0
      Serial.println("Host Name2 Exitoso");
      #endif
    }else{
      #if DEBUG_MODE > 0
      Serial.println("Host Name2 Fallo");
      #endif
    }
  }else{
    WiFi.begin(ssid.c_str(), pass.c_str());
    delay(1);
    if(WiFi.setHostname(nameHost)){
      #if DEBUG_MODE > 0
      Serial.print("Host Name2 Exitoso: ");
      delay(1);
      Serial.println(WiFi.getHostname());
      #endif
    }else{
      #if DEBUG_MODE > 0
      Serial.println("Host Name2 Fallo: ");
      delay(1);
      Serial.println(WiFi.getHostname());
      #endif
    }
  }
  #if DEBUG_MODE > 0
    Serial.println("Conectando...");
  #endif
  uint8_t c = 0;
  while(WiFi.status() != WL_CONNECTED && c < 30) {
    c++;
    digitalWrite(statusLED_Ext, HIGH);
    digitalWrite(statusLED, HIGH);
    delay(100);
    digitalWrite(statusLED_Ext, LOW);
    digitalWrite(statusLED, LOW);
    delay(500);
    #if DEBUG_MODE > 0
      Serial.print(".");
    #endif
    
  }
  if( c > (30-1)){
    return (false);
  }else{
    
    #if DEBUG_MODE > 0
      Serial.println("");
      Serial.print("Conectado a la Red Wifi con IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("ESP Mac Address: ");
      Serial.println(WiFi.macAddress());
      Serial.print("Subnet Mask: ");
      Serial.println(WiFi.subnetMask());
      Serial.print("Gateway IP: ");
      Serial.println(WiFi.gatewayIP());
      Serial.print("DNS: ");
      Serial.println(WiFi.dnsIP());
    #endif
    // if(addrIP[0] == 0xFF ){
    //   #if DEBUG_MODE > 0
    //     Serial.println("Grabando el la EEPROM la conf de IP Fija");
    //   #endif
    //   addrIP = WiFi.localIP();
    //   addrGat = WiFi.gatewayIP();
    //   addrSub = WiFi.subnetMask();
    //   writeIP(addrIP);
    //   writeGATEWAY(addrGat);
    //   writeSUBNET(addrSub);
    //   addrIP = readIP();
    //   addrGat = readGATEWAY();
    //   addrSub = readSUBNET();
    //   #if DEBUG_MODE > 0
    //     Serial.println("Leyendo de la EEPROM la conf de IP Fija");
    //     Serial.print("IP de la EEPROM Leida: "); Serial.println(addrIP);
    //     Serial.print("Gateway de la EEPROM Leida: "); Serial.println(addrGat);
    //     Serial.print("SubNet de la EEPROM Leida: "); Serial.println(addrSub);
    //   #endif
    // }else{
    //   #if DEBUG_MODE > 0
    //     Serial.println("Ya esta Grabada en la EEPROM la conf de IP Fija");
    //   #endif
    // }

    return (true);
  }
}

const void* verCmdoJson();
const void* handleNotFound();
 
void configServer(){
  server.on(F("/cmdoJson"), HTTP_POST, verCmdoJson);
  server.onNotFound(handleNotFound);
  server.begin();
  #if DEBUG_MODE > 0
    Serial.println("\nserver.on se Configuro...");
  #endif
}


bool serverJsonIsOK(){
  if (server.hasArg("plain") == false){
    String body = server.arg("plain");
    #if DEBUG_MODE > 0
    Serial.println("\nJson Rx:");
    Serial.println(body);
    #endif
    return false;
  }else{
    String body = server.arg("plain");
    #if DEBUG_MODE > 0
    Serial.println("\nJson Rx:");
    Serial.println(body);
    #endif
    return true;
  }
    
}

void requestError(){
  // String jsonString;
  // String ssidConfig;
  // String passConfig;
  if (server.hasArg("plain") == false){
    String strJson;
    strJson = "{\"error\": 1";
    strJson += "}";
    server.send(200,"application/json",strJson);
    return;
  }
}

int verCmdo(String& ssidConfig, String& passConfig, String& tokenBot){
    uint8_t resp = NoComd;
    StaticJsonDocument<250> jsonDocument;
    String body = server.arg("plain");
    #if DEBUG_MODE > 0
    Serial.println("\nJson Rx:");
    Serial.println(body);
    #endif
    deserializeJson(jsonDocument, body);
    if(jsonDocument["cmdo"] == "openP"){
      if(jsonDocument["data"]["userName"]){
        userName = jsonDocument["data"]["userName"].as<String>();
      }else{
        userName = "";
      }
      if(jsonDocument["data"]["userID"]){
        userID = jsonDocument["data"]["userID"];
      }else{
        userID = 0;
      }
      //return OpenP;
      resp = OpenP;
    }else if(jsonDocument["cmdo"] == "clouseP"){
      if(jsonDocument["data"]["userName"]){
        userName = jsonDocument["data"]["userName"].as<String>();
      }else{
        userName = "";
      }
      if(jsonDocument["data"]["userID"]){
        userID = jsonDocument["data"]["userID"];
      }else{
        userID = 0;
      }
      //return ClouseP;
      resp = ClouseP;
    }else if(jsonDocument["cmdo"] == "statusP"){
      if(jsonDocument["data"]["userName"]){
        userName = jsonDocument["data"]["userName"].as<String>();
      }else{
        userName = "";
      }
      if(jsonDocument["data"]["userID"]){
        userID = jsonDocument["data"]["userID"];
      }else{
        userID = 0;
      }
      //return StatusP;
      resp = StatusP;
    }else if(jsonDocument["cmdo"] == "powerPort"){
      if(jsonDocument["data"]["userName"]){
        userName = jsonDocument["data"]["userName"].as<String>();
      }else{
        userName = "";
      }
      if(jsonDocument["data"]["userID"]){
        userID = jsonDocument["data"]["userID"];
      }else{
        userID = 0;
      }
      //return StatusP;
      resp = powerPort;
    }else if(jsonDocument["cmdo"] == "statusBot"){
      if(jsonDocument["data"]["userName"]){
        adminUser = jsonDocument["data"]["userName"].as<String>();
      }else{
        adminUser = "";
      }
      if(jsonDocument["data"]["userID"]){
        admidId = jsonDocument["data"]["userID"];
      }else{
        admidId = 0;
      }
      //return credenUser;
      resp = StatusBot;
    }
    else if(jsonDocument["cmdo"] == "credenU"){
      if(jsonDocument["data"]["userName"]){
        userName = jsonDocument["data"]["userName"].as<String>();
      }else{
        userName = "";
      }
      if(jsonDocument["data"]["userID"]){
        userID = jsonDocument["data"]["userID"];
      }else{
        userID = 0;
      }
      //return credenUser;
      resp = credenUser;
    }else if(jsonDocument["cmdo"] == "accPoint"){
      if(jsonDocument["data"]["userName"]){
        userName = jsonDocument["data"]["userName"].as<String>();
      }else{
        userName = "";
      }
      if(jsonDocument["data"]["userID"]){
        userID = jsonDocument["data"]["userID"];
      }else{
        userID = 0;
      }
      //return AccPoint;
      resp = AccPoint;
    }else if(jsonDocument["cmdo"] == "conectAP"){
      if(jsonDocument["data"]["userName"]){
        userName = jsonDocument["data"]["userName"].as<String>();
      }else{
        userName = "";
      }
      if(jsonDocument["data"]["userID"]){
        userID = jsonDocument["data"]["userID"];
      }else{
        userID = 0;
      }
      #if DEBUG_MODE > 0
        Serial.print("\nSSID Recibido del Json: ");
        Serial.println(jsonDocument["data"]["ssid"].as<String>());
        Serial.print("PASS Recibido del Json: ");
        Serial.println(jsonDocument["data"]["pass"].as<String>());
      #endif
      if(jsonDocument["data"]["ssid"]){
        ssidConfig = jsonDocument["data"]["ssid"].as<String>();
        if(jsonDocument["data"]["pass"]){
          passConfig = jsonDocument["data"]["pass"].as<String>();
        }else{
          passConfig = "";
        }
        //return ConectAcPo;
        resp = ConectAcPo;
      }else{
          //return  NoComd;
          resp = NoComd;
      }
    }else if(jsonDocument["cmdo"] == "tokenBot"){
      // if(jsonDocument["data"]["userName"]){
      //   adminUser = jsonDocument["data"]["userName"].as<String>();
      // }else{
      //   adminUser = "";
      // }
      // if(jsonDocument["data"]["userID"]){
      //   admidId = jsonDocument["data"]["userID"];
      // }else{
      //   admidId = 0;
      // }
      tokenBot = jsonDocument["data"]["token"].as<String>();
      //return tokenJson;
      resp = tokenJson;
    }else if(jsonDocument["cmdo"] == "erraseEE"){
      if(jsonDocument["data"]["userName"]){
        adminUser = jsonDocument["data"]["userName"].as<String>();
      }else{
        adminUser = "";
      }
      if(jsonDocument["data"]["userID"]){
        admidId = jsonDocument["data"]["userID"];
      }else{
        admidId = 0;
      }
      //return erraseAll;
      resp = erraseAll;
    }else if(jsonDocument["cmdo"] == "verAllUser"){
      if(jsonDocument["data"]["userName"]){
        adminUser = jsonDocument["data"]["userName"].as<String>();
      }else{
        adminUser = "";
      }
      if(jsonDocument["data"]["userID"]){
        admidId = jsonDocument["data"]["userID"];
      }else{
        admidId = 0;
      }
      //return verUser;
      resp = verUser;
        // }
        // else if(jsonDocument["cmdo"] == "grabarUser"){
        //   if(jsonDocument["data"]["userName"]){
        //     adminUser = jsonDocument["data"]["userName"].as<String>();
        //   }else{
        //     adminUser = "";
        //   }
        //   if(jsonDocument["data"]["userID"]){
        //     admidId = jsonDocument["data"]["userID"];
        //   }else{
        //     admidId = 0;
        //   }
        //   userMsg.user.id = jsonDocument["data"]["user"];
        //   return grabUser;
    }else if(jsonDocument["cmdo"] == "registrar"){
      userName = jsonDocument["data"]["userName"].as<String>();
      userID = jsonDocument["data"]["userID"];
      adminUser = jsonDocument["data"]["adminName"].as<String>();
      admidId = jsonDocument["data"]["adminID"];
      userBlock = jsonDocument["data"]["blockUser"];
      //return registUser;
      resp = registUser;
    }else if(jsonDocument["cmdo"] == "verRegistro"){
      if(jsonDocument["data"]["userName"]){
        adminUser = jsonDocument["data"]["userName"].as<String>();
      }else{
        adminUser = "";
      }
      if(jsonDocument["data"]["userID"]){
        admidId = jsonDocument["data"]["userID"];
      }else{
        admidId = 0;
      }
      //return verRegistro;
      resp = verRegistro;
    }else if(jsonDocument["cmdo"] == "erraseUser"){
      userName = jsonDocument["data"]["userName"].as<String>();
      userID = jsonDocument["data"]["userID"];
      adminUser = jsonDocument["data"]["adminName"].as<String>();
      admidId = jsonDocument["data"]["adminID"];
      regID = jsonDocument["data"]["regID"];
      //return erraseUser;
      resp = erraseUser;
    }else if(jsonDocument["cmdo"] == "blockUser"){
      userName = jsonDocument["data"]["userName"].as<String>();
      userID = jsonDocument["data"]["userID"];
      adminUser = jsonDocument["data"]["adminName"].as<String>();
      admidId = jsonDocument["data"]["adminID"];
      regID = jsonDocument["data"]["regID"];
      userBlock = jsonDocument["data"]["userBlock"];
      //return blockUser;
      resp = blockUser;
    }else if(jsonDocument["cmdo"] == "temperatura"){
      userName = jsonDocument["data"]["userName"].as<String>();
      userID = jsonDocument["data"]["userID"];
      adminUser = jsonDocument["data"]["adminName"].as<String>();
      admidId = jsonDocument["data"]["adminID"];
      regID = jsonDocument["data"]["regID"];
      userBlock = jsonDocument["data"]["userBlock"];
      //return blockUser;
      resp = temperature;
    }else if(jsonDocument["cmdo"] == "modeSTA"){
      userName = jsonDocument["data"]["userName"].as<String>();
      userID = jsonDocument["data"]["userID"];
      adminUser = jsonDocument["data"]["adminName"].as<String>();
      admidId = jsonDocument["data"]["adminID"];
      regID = jsonDocument["data"]["regID"];
      userBlock = jsonDocument["data"]["userBlock"];
      //return blockUser;
      resp = modeSTA;
    }
    return resp;
}

void sendRequest(int codeRequest, String strJson){
    server.send(codeRequest,"application/json",strJson);
}

void verClient(){
  // #if DEBUG_MODE > 0
  //   Serial.print("\nVer Cliente....");
  // #endif
  // Serial.println(xPortGetCoreID());
  server.handleClient();
}