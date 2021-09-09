#include <Arduino.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
//#include <ESP32Ping.h>
#include <WiFiClientSecure.h>

#include <OneWire.h>
#include <DallasTemperature.h>

//#define ONE_WIRE_BUS 18
//#define ONE_WIRE_BUS 19
#define ONE_WIRE_BUS 17

OneWire oneWire_in(ONE_WIRE_BUS);
DallasTemperature sensor_temp(&oneWire_in);
DeviceAddress insideThermometer;


//Se incluyen mis Archivos
#include "header.h"
#include "utils_isr_timer.h"
#include "utils_eeprom.h"
#ifndef commandTelegram_h
  #include "commandTelegram.h"
#endif
#ifndef commandWifi_h
    #include "commandWifi.h"
#endif

//////////////////////////////////////////
// Codigo para sensar temperatura Interna
//////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
///////////////////////////////////////////
bool  isconectAP = false;
bool conct_1Vez = false;
bool falgAdmin = false;
bool flagConfig = false;
bool flag1Vez = true;
bool txDevice = false;
bool TxWebTel_AbP = false;
bool TxWebTel_AcP = false;
bool TxWebTel_CeP = false;
bool TxWebTel_PoP = false;
bool portonActLoc = false;
bool portonActConLoc = false;
bool portonActAbrLoc = false;
bool portonActCerLoc = false;
bool TxAdminTimeOpen = false;
bool TxAdminOpCl = false;
bool flagAct1Vez = true;
//bool flagActRF = false;
bool flagScanSensor = false;

uint8_t  countScanSensor;
uint16_t countStatusLed;
uint16_t countStatusWifi;
uint16_t countTemp;
String ssid;
String pass;
String tokenBot;
TBUser    userBot;
TBMessage userMsg;
UserAcount cuentaUser;
UserAccName cuentaUserName;
UserBlock cuentaUserBlock;
TaskHandle_t TaskWifi;
TaskHandle_t TaskRutinas;
String userName;
String userNameBot;
int32_t userID;
String adminUser;
int32_t admidId;
uint8_t regID;
bool  userBlock;
regUser regUserStore[USER_ALL];
FSM_Porton estados;
uint8_t countFSM = 0;
//uint8_t countTime = 0;
uint8_t timeOpenClousPot = 255;
bool    ActPorton = false;
bool    medirTimePot = false;
bool    infPorAbCe = false;

float longitude = 0.0;
float latitude = 0.0;

String sinFormat = "";
String conFormat = "Markdown";

float temperatureC;

void readEepromAllUser(void);
void imprimirAllUser(void);
void verTelegram(void);
void loop2(void * pvParameters);
//void rutinas(void * pvParameters);
bool userNoIsRegistred(int32_t verUserID);
void userNoRegister(void);
void enviarRegistrarTel(void);
bool adminRegistra(void);
bool adminRegistraUser(void);
void guardarREgistrarTel(void);
void limpiarRegUser(void);
void enviarRegPend(void);
void enviarUserRequest(void);
bool verRegisterActive(void);
void registoPendiente(void);
bool verCuenta(String scanUserName, int32_t scanUserId);
void envRegistroUser(void);
String paramToken(void);
bool adminBlockUser();
void enviarDeviceTelegram(void);
bool userIsBlock(int32_t idUser);
void userBloqueado(void);
void enviarWebAbPTelegram(void);
void enviarWebAcPTelegram(void);
void enviarWebCePTelegram(void);
void enviarWebPoPTelegram(void);
void porton_FSM(void);
String credenciales(int32_t credenID);
void enviarLocalTelegram(void);
void enviarContLocalTelegram(void);
void enviarAbrirLocalTelegram(void);
void enviarCerrarLocalTelegram(void);
void enviarFaltaUserName(void);
void verButton();
void statuLED();
void enviarTimeOpenAdm(void);
void enviarTimeClouseAdm(void);
void enviarClouseAdm(void);
void enviarOpenAdm(void);
void printAddress(DeviceAddress deviceAddress);
void readDS1820(void);
void _conectAccesPoint(void);
void _conectBot(void);
bool verAdminBlank(void);
String credencialesSinAdmin(void);

void setup() {
  conct_1Vez = false;
  pinMode(powerPorton, OUTPUT);
  digitalWrite(powerPorton, LOW);
  #if DEBUG_MODE > 0
    Serial.begin(115200); 
  #endif
  initISR_Timer();
  EEPROMinit();
  //pinMode(LED, OUTPUT);
  pinMode(statusLED, OUTPUT);
  pinMode(statusLED_Ext, OUTPUT);
  //digitalWrite(LED, LOW);
  digitalWrite(statusLED, LOW);
  digitalWrite(statusLED_Ext, LOW);
  pinMode(bottonProg, INPUT_PULLUP);
  pinMode(ActPortonOutput, OUTPUT);
  digitalWrite(ActPortonOutput, LOW);
  pinMode(SensorPC, INPUT_PULLUP);
  pinMode(SensorPO, INPUT_PULLUP);
  pinMode(SenPorAct, INPUT_PULLUP);
  pinMode(SenPorPower, INPUT_PULLUP);
  
  ////DS18B20 *******************************************************************************************************
  sensor_temp.begin();
  
  #if DEBUG_MODE > 0
    Serial.print("DS18b20 Found: ");
    Serial.print(sensor_temp.getDeviceCount(), DEC);
    Serial.print("Parasite power is: "); 
  
    if (sensor_temp.isParasitePowerMode()) Serial.println("ON");
    else Serial.println("OFF");
  
    if (!sensor_temp.getAddress(insideThermometer, 0)) Serial.println("Unable to find address DS18b20 for Device 0");
    Serial.print("Device 0 Address: ");
    printAddress(insideThermometer);
  #else
    sensor_temp.isParasitePowerMode();
    sensor_temp.getAddress(insideThermometer, 0);
  #endif
  sensor_temp.setResolution(insideThermometer, 9);
  #if DEBUG_MODE > 0
    Serial.println();
    Serial.print("Device 0 Resolution: ");
    Serial.print(sensor_temp.getResolution(insideThermometer), DEC); 
    Serial.println();
  #endif
  countTemp = 0;
  readDS1820();
  //////***************************************************************************************************************
  _conectAccesPoint();
  _conectBot();
  // ssid = readSSID();
  // pass = readPASS();
  // isconectAP = conectAP(ssid, pass);
  // if(isconectAP){
  //   #if DEBUG_MODE > 0
  //     Serial.println("\nConfigurando Server");
  //   #endif
  //   configServer();
  // }
  // String tok = readTOKEN();
  // if(tok == ""){
  //   #if DEBUG_MODE > 0
  //     Serial.println("\nToken leido de la EEPROM nulo");
  //   #endif
  // }
  // else if(tok[10] == ':'){
  //   #if DEBUG_MODE > 0
  //   Serial.println("\nToken de la EEPROM Valido, con token: ");
  //   Serial.println(tok);
  //   #endif
  //   tokenBot = tok;
  //   int request;
  //   bool conectBot = false;
  //   if(WiFi.status() == WL_CONNECTED)
  //   //if(WiFi.isConnected())
  //   {
  //     request = verBot(tok);
  //     if(request == 200){
  //       conectBot = true;
  //       #if DEBUG_MODE > 0
  //         Serial.println("Bot Responde y en Linea");
  //         Serial.print("\nId del Bot: ");
  //         Serial.println(userBot.id);
  //         Serial.print("Nombre del Bot: ");
  //         Serial.println(userBot.firstName);
  //         Serial.print("UserName del Bot: ");
  //         Serial.println(userBot.username);
  //       #endif
  //       userNameBot = userBot.firstName;
  //       tokenBot = tok;
  //       #if DEBUG_MODE > 0
  //         Serial.print("Token Leido EEPROM: ");
  //         Serial.println(tokenBot);
  //       #endif
  //       //writeTOKEN(tok);
  //       requestGetUpdate(tokenBot);
  //     }
  //   }
  //   if(!conectBot){
  //     #if DEBUG_MODE > 0
  //       Serial.println("\nBot no Responde");
  //     #endif
  //   }
  // }else{
  //   #if DEBUG_MODE > 0
  //     Serial.println("\nToken leido de la EEPROM no valido");
  //   #endif
  // }

  estados = PortonAbriendo;


  limpiarRegUser();
  readEepromAllUser();
  readAllUserName();
  readAllUserBlock();
  //imprimirAllUser();
  // if(noPortonClouse && noPortonOpen){
  //   estados = PortonAbriendo;
  // }
  xTaskCreatePinnedToCore(
    loop2, /* Funcion de la tarea1 */
    "Tarea1", /* Nombre de la tarea */
    10000,  /* Tamaño de la pila */
    NULL,  /* Parametros de entrada */
    0,  /* Prioridad de la tarea */
    &TaskWifi,  /* objeto TaskHandle_t. */
    0);
  // xTaskCreatePinnedToCore(
  //   rutinas, /* Funcion de la tarea1 */
  //   "Tarea2", /* Nombre de la tarea */
  //   1000,  /* Tamaño de la pila */
  //   NULL,  /* Parametros de entrada */
  //   0,  /* Prioridad de la tarea */
  //   &TaskRutinas,  /* objeto TaskHandle_t. */
  //   1);  /* Nucleo donde se correra */
}



void loop() {
  // verClient();
  // statuLED();
  // verButton();
  if(WiFi.status() == WL_CONNECTED)
  //if(WiFi.isConnected())
  {
    if(tokenBot[10] == ':'){
      verTelegram();
    }
  }
  if(tokenBot[10] == ':'){
    if(txDevice){
      txDevice = false;
      enviarDeviceTelegram();
    }
    if(TxWebTel_AbP){
      TxWebTel_AbP = false;
      enviarWebAbPTelegram();
    }
    if(TxWebTel_AcP){
      TxWebTel_AcP = false;
      enviarWebAcPTelegram();
    }
    if(TxWebTel_CeP){
      TxWebTel_CeP = false;
      enviarWebCePTelegram();
    }
    if(TxWebTel_PoP){
      TxWebTel_PoP = false;
      enviarWebPoPTelegram();
    }
    if(portonActLoc){
      portonActLoc = false;
      if(isPortonON){
        enviarLocalTelegram();
      }
    }
    if(portonActConLoc){
      portonActConLoc = false;
      enviarContLocalTelegram();
    }
    if(portonActAbrLoc){
      portonActAbrLoc = false;
      enviarAbrirLocalTelegram();
    }
    if(portonActCerLoc){
      portonActCerLoc = false;
      enviarCerrarLocalTelegram();
    }
    if(TxAdminTimeOpen){
      TxAdminTimeOpen = false;
      if(estados == PortonClouse){
        enviarTimeClouseAdm();
      }else if(estados == PortonOpen){
        enviarTimeOpenAdm();
      }
      
    }
    if(TxAdminOpCl){
      TxAdminOpCl = false;
      if(estados == PortonClouse){
        enviarClouseAdm();
      }else if(estados == PortonOpen){
        enviarOpenAdm();
      }
    }
  }
  
  vTaskDelay(10);
  // put your main code here, to run repeatedly:
}

void loop2(void * pvParameters){
  #if DEBUG_MODE > 0
    Serial.print("loop2 nucleo: ");
    Serial.println(xPortGetCoreID());
  #endif
  while (true)
  {
    if(WiFi.status() == WL_CONNECTED)
    //if(WiFi.isConnected())
    {
      verClient();
      //  #if DEBUG_MODE > 0
      //   Serial.println("Revisando Cliente Wifi por Wifi");
      //   //Serial.println(xPortGetCoreID());
      // #endif
    }
    if(flagConfig){
      verClient();
      // #if DEBUG_MODE > 0
      //   Serial.println("Revisando Cliente Wifi por Config");
      //   //Serial.println(xPortGetCoreID());
      // #endif
    }
    porton_FSM();
    statuLED();
    verButton();
  }
  //vTaskDelay(10);
}

// void rutinas(void * pvParameters){
//   enviarDeviceTelegram();
// }

String scanWifi(void);
bool configWiFi(String ssidSTA, String passSTA);
String respuestaConfig(int code);
bool verToken(void);




void handleNotFound(){
  DynamicJsonDocument doc(512);
  doc["code"] = 404;
  

  //Serial.print(F("Stream..."));
  String buf;
  serializeJson(doc, buf);
  sendRequest(400, buf);
  //server.send(400, F("application/json"), buf);
  //Serial.print(F("done."));
  #if DEBUG_MODE > 0
    Serial.print("Error de Request del cliente");
  #endif
}

void _conectAccesPoint(void){
  ssid = readSSID();
  pass = readPASS();
  isconectAP = conectAP(ssid, pass);
  if(isconectAP){
    #if DEBUG_MODE > 0
      Serial.println("\nConfigurando Server");
    #endif
    configServer();
    conct_1Vez = true;
  }
}
void _conectBot(void){
  String tok = readTOKEN();
  if(tok == ""){
    #if DEBUG_MODE > 0
      Serial.println("\nToken leido de la EEPROM nulo");
    #endif
  }
  else if(tok[10] == ':'){
    #if DEBUG_MODE > 0
    Serial.println("\nToken de la EEPROM Valido, con token: ");
    Serial.println(tok);
    #endif
    tokenBot = tok;
    int request;
    bool conectBot = false;
    if(WiFi.status() == WL_CONNECTED)
    //if(WiFi.isConnected())
    {
      request = verBot(tok);
      if(request == 200){
        conectBot = true;
        #if DEBUG_MODE > 0
          Serial.println("Bot Responde y en Linea");
          Serial.print("\nId del Bot: ");
          Serial.println(userBot.id);
          Serial.print("Nombre del Bot: ");
          Serial.println(userBot.firstName);
          Serial.print("UserName del Bot: ");
          Serial.println(userBot.username);
        #endif
        userNameBot = userBot.firstName;
        tokenBot = tok;
        #if DEBUG_MODE > 0
          Serial.print("Token Leido EEPROM: ");
          Serial.println(tokenBot);
        #endif
        //writeTOKEN(tok);
        requestGetUpdate(tokenBot);
      }
    }
    if(!conectBot){
      #if DEBUG_MODE > 0
        Serial.println("\nBot no Responde");
      #endif
    }
  }else{
    #if DEBUG_MODE > 0
      Serial.println("\nToken leido de la EEPROM no valido");
    #endif
  }
}



//Si se Recibe Peticiones WEB alServidor se ejecuta
//esta Funcion
void verCmdoJson(){
  #if DEBUG_MODE > 0
    Serial.print("verCmdo nucleo: ");
    Serial.println(xPortGetCoreID());
  #endif
  String strJson;
  String  mensaje;
  bool envMsjAdmTel_AbP = false;
  bool envMsjAdmTel_AcP = false;
  bool envMsjAdmTel_CeP = false;
  bool envMsjAdmTel_PoP = false;
  
  if(!serverJsonIsOK()){
    #if DEBUG_MODE > 0
    Serial.println("Se Rx con ERROR");
    #endif
    requestError();
  }else{
    #if DEBUG_MODE > 0
    Serial.println("Se Rx SIN ERROR");
    #endif
    int requeest = verCmdo(ssid, pass, tokenBot);
    if(requeest == OpenP){
      if(verCuenta(userName, userID)){
        if(!userIsBlock(userID)){
          if(estados == PortonOpen){
            strJson = "{\"code\": 550}";
          }else if(estados == PortonClouse){
            ActPorton = true;
            strJson = "{\"code\": 0}";
            envMsjAdmTel_AbP = true;
          }else if(estados == PortonAbriendo || estados == PortonCerrando){
            ActPorton = true;
            strJson = "{\"code\": 551}";
            envMsjAdmTel_AcP = true;
          }
          
        }else{
          strJson = "{\"code\": 501}";
        }
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
      if(envMsjAdmTel_AbP){
        envMsjAdmTel_AbP = false;
        //if(userID != cuentaUser.account[0]){
          TxWebTel_AbP = true;
        //}
      }else if(envMsjAdmTel_AcP){
        envMsjAdmTel_AcP = false;
        //if(userID != cuentaUser.account[0]){
          TxWebTel_AcP = true;
        //}
      }
    }else if(requeest == ClouseP){
      if(verCuenta(userName, userID)){
        if(!userIsBlock(userID)){
          if(estados == PortonClouse){
            strJson = "{\"code\": 550}";
          }else if(estados == PortonOpen){
            ActPorton = true;
            strJson = "{\"code\": 0}";
            envMsjAdmTel_CeP = true;
          }else if(estados == PortonAbriendo || estados == PortonCerrando){
            ActPorton = true;
            strJson = "{\"code\": 551}";
            envMsjAdmTel_AcP = true;
          }
          
        }else{
          strJson = "{\"code\": 501}";
        }
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
      if(envMsjAdmTel_CeP){
        envMsjAdmTel_CeP = false;
        //if(userID != cuentaUser.account[0]){
          TxWebTel_CeP = true;
        //}
      }else if(envMsjAdmTel_AcP){
        envMsjAdmTel_AcP = false;
        //if(userID != cuentaUser.account[0]){
          TxWebTel_AcP = true;
        //}
      }
    }else if(requeest == StatusP){
      if(verCuenta(userName, userID)){
        if(!userIsBlock(userID)){
          if(estados == PortonClouse){
            strJson = "{\"code\": 560}";
          }else if(estados == PortonOpen){
            strJson = "{\"code\": 561}";
          }else if(estados == PortonAbriendo){
            strJson = "{\"code\": 562}";
          }else if(estados == PortonCerrando){
            strJson = "{\"code\": 563}";
          }else{
            strJson = "{\"code\": 564}";
          }
          //envMsjAdmTel = true;
        }else{
          strJson = "{\"code\": 501}";
        }
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
      // if(envMsjAdmTel){
      //   envMsjAdmTel = false;
      //   if(userID != cuentaUser.account[0]){
      //     TxWebTel = true;
      //   }
      // }
    }else if(requeest == powerPort){
      if(verCuenta(userName, userID)){
        if(!userIsBlock(userID)){
          digitalWrite(powerPorton, !digitalRead(powerPorton));
          if(digitalRead(powerPorton)){
            strJson = "{\"code\": 551}";
          }else{
            strJson = "{\"code\": 550}";
          }
          envMsjAdmTel_PoP = true;
        }else{
          strJson = "{\"code\": 501}";
        }
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
      if(envMsjAdmTel_PoP){
        envMsjAdmTel_PoP = false;
        TxWebTel_PoP = true;
      }
    }else if(requeest == credenUser){
      if(verAdminBlank()){
        strJson = credencialesSinAdmin();
      }else{
        if(verCuenta(userName, userID)){
          strJson = credenciales(userID);
        }else{
          strJson = "{\"code\": 401}";
        }
      }
      sendRequest(200, strJson);
    }
    else if(requeest == AccPoint){
      strJson = "";
      strJson = scanWifi();
      sendRequest(200, strJson);
    }else if(requeest == ConectAcPo){
      if(configWiFi(ssid, pass)){
        strJson = ""; 
        strJson = respuestaConfig(0);
        sendRequest(200, strJson);
        writeSSID(ssid);
        writePASS(pass);
      }else{
        strJson = ""; 
        strJson = respuestaConfig(1);
        sendRequest(200, strJson);
      }
    }else if(requeest == tokenJson){
      if(verToken()){
        //strJson = "{\"code\": 0}";
        strJson = paramToken();
        writeTOKEN(tokenBot);
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
    }else if(requeest == StatusBot){
      if(admidId == cuentaUser.account[0]){
        if(verToken()){
          strJson = paramToken();
        }
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
    }
    else if(requeest == erraseAll){
      if(admidId == cuentaUser.account[0]){
        strJson = "{\"code\": 0}";
        erraseALL();
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
    }else if(requeest == verUser){
      // strJson = "{\"code\": 0}";
      // sendRequest(strJson);
      if(admidId == cuentaUser.account[0]){
        enviarUserRequest();
        imprimirAllUser();
      }else{
        strJson = "{\"code\": 401}";
        sendRequest(200, strJson);
      }
    // }
    // else if(requeest == grabUser){
    //   strJson = "{\"code\": 0}";
    //   sendRequest(strJson);
    //   for (int8_t i = 0; i < USER_ALL; i++){
    //     if(cuentaUser.account[i] != 0xFFFFFFFF){
    //       cuentaUser.account[i] = userMsg.user.id;
    //       Serial.print("User Rx: "); Serial.println(cuentaUser.account[i]);
    //       writeUser_i(i);
    //       readUser_i (i);
    //       Serial.print("Usuario Leido de la EEPROM: ");
    //       Serial.println(cuentaUser.account[i]);
    //       break;
    //     }
    //   }
      
    }else if(requeest == registUser){
      bool envRegUser = false;
      if(verAdminBlank()){
        if(adminRegistraUser()){
          //strJson = "{\"code\": 0}";
          strJson = credenciales(cuentaUser.account[0]);
        }else{
          strJson = "{\"code\": 400}";
        }
      }else{
        if(cuentaUser.account[0] == admidId){
          if(userNoIsRegistred(userID)){
            if(adminRegistraUser()){
            
            strJson = "{\"code\": 0}";
            envRegUser = true;
            //sendRequest(strJson);
            }else{
              strJson = "{\"code\": 400}";
            }
          }else{
            strJson = "{\"code\": 400}";
          //sendRequest(strJson);
          }
        }else{
          strJson = "{\"code\": 401}";
          //sendRequest(strJson);
        }
      }
      sendRequest(200, strJson);
      if(envRegUser){
        envRegistroUser();
      }
      #if DEBUG_MODE > 0
        Serial.println("Rutina de Registrar Usuario, se envia el JSON:");
        Serial.println(strJson);
      #endif
    }else if(requeest == blockUser){
      if(cuentaUser.account[0] == admidId){
        if(cuentaUser.account[regID] == userID){
          if(adminBlockUser()){
          
          strJson = "{\"code\": 0}";
          //sendRequest(strJson);
          }else{
            strJson = "{\"code\": 400}";
          }
        }else{
          strJson = "{\"code\": 400}";
        //sendRequest(strJson);
        }
      }else{
        strJson = "{\"code\": 401}";
        //sendRequest(strJson);
      }
      sendRequest(200, strJson);
    }else if(requeest == verRegistro){
      if(cuentaUser.account[0] == admidId){
        enviarRegPend();
      }else{
        strJson = "{\"code\": 401}";
        sendRequest(200, strJson);
      }
      
    }else if(requeest == erraseUser){
      readAllUser();
      if(cuentaUser.account[0] == admidId){
        if(userID == cuentaUser.account[regID]){
          cuentaUser.account[regID] = 0xFFFFFFFF;
          writeUser_i(regID);
          writeUserName_i(regID, "");
          strJson = "{\"code\": 0}";
        }else{
          strJson = "{\"code\": 400}";
        }
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
    }else if(requeest == temperature){
      if(verCuenta(userName, userID)){
        if(!userIsBlock(userID)){
          readDS1820();
          strJson = "{\"code\": 0, \"temperature\":";
          strJson += temperatureC;
          strJson += "}";
        }else{
          strJson = "{\"code\": 501}";
        }
      }else{
        strJson = "{\"code\": 401}";
      }
      sendRequest(200, strJson);
      if(envMsjAdmTel_PoP){
        envMsjAdmTel_PoP = false;
        TxWebTel_PoP = true;
      }
    }else if(requeest == modeSTA){
      bool actvSTA = false;
      actvSTA = true;
      strJson = "{\"code\": 0}";
      // if(verCuenta(userName, userID)){
      //   if(!userIsBlock(userID)){
      //     actvSTA = true;
      //     strJson = "{\"code\": 0}";

      //   }else{
      //     strJson = "{\"code\": 501}";
      //   }
      // }else{
      //   strJson = "{\"code\": 401}";
      // }
      sendRequest(200, strJson);
      if(actvSTA){
        flagConfig = false;
        delay(1000);
        _conectAccesPoint();
        _conectBot();
      }
    }
    else{
      strJson = "{\"code\": 401}";
      sendRequest(401, strJson);
    }
    
  }
}

String paramToken(void){
  String jsonString;
  StaticJsonDocument<2048> root;
  root.clear();
  DynamicJsonDocument doc(1500); 
  doc.clear();
  //JsonObject s1 = doc.createNestedObject();
  doc["idBot"] = userBot.id;
  doc["firstName"] = userBot.firstName;
  doc["username"] = userBot.username;
  root["code"] = 0;
  root["data"] = doc;
  serializeJson(root, jsonString);
  return (jsonString);
}

bool verToken(void){
  if(tokenBot[10] == ':'){
    #if DEBUG_MODE > 0
    Serial.println("Token de la Json Valido");
    #endif
    int request = verBot(tokenBot);
    if(request != 200){
      #if DEBUG_MODE > 0
        Serial.println("\nBot no Responde");
      #endif
      return (false);
    }else{
      #if DEBUG_MODE > 0
        Serial.println("Bot Responde y en Linea");
        Serial.print("\nId del Bot: ");
        Serial.println(userBot.id);
        Serial.print("Nombre del Bot: ");
        Serial.println(userBot.firstName);
        Serial.print("UserName del Bot: ");
        Serial.println(userBot.username);
      #endif
      //writeTOKEN(tokenBot);
      requestGetUpdate(tokenBot);
      return (true);
    }

  }else{
    #if DEBUG_MODE > 0
      Serial.println("\nToken leido de la EEPROM no valido");
    #endif
    return (true);
  }
}

String scanWifi(void){
  //DynamicJsonDocument jsonBuffer(2048);
  int n = WiFi.scanNetworks();
  String jsonString;
  #if DEBUG_MODE > 0
    Serial.print("\nComando Json Buscar Wifi");
    Serial.println("\nEscaneando Redes Wifi...");
    Serial.print("Redes Encontradas: ");
    Serial.println(n);
  
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found   
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
    Serial.println("\n\n");
  #endif
  if (n == 0){
    String strJson;
    strJson = "{\"code\": 0, \"wifi\":[]}";
    //server.send(200,"application/json",strJson);
    return (strJson);
  }else{
    StaticJsonDocument<2048> root;
    root.clear();
    DynamicJsonDocument doc(1500); 
    doc.clear();
    JsonArray arrayWifi = doc.to<JsonArray>();
    //arrayWifi.clear();
    //JsonObject s1 = doc.as<JsonObject>();
    JsonObject s1 = arrayWifi.createNestedObject();
    s1.clear();
    root["code"] = 0;
    arrayWifi.remove(0);
    for (int i = 0; i < n; ++i){
      s1["ssid"] = WiFi.SSID(i);
      String ssid = s1["wifi"];
      int qos = WiFi.RSSI(i);
      if(qos < -85)
        s1["qos"] = 1;
      else if(qos < -75)
        s1["qos"] = 2;
      else if(qos < -60)
        s1["qos"] = 3;
      else
        s1["qos"] = 4;
      s1["encryption"] = WiFi.encryptionType(i);
      arrayWifi.add(s1);
      Serial.print("Tamaño Arreglo para i= ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(arrayWifi.size());
    }
    #if DEBUG_MODE > 0
      Serial.print("Tamaño Arreglo Total: ");
      Serial.println(arrayWifi.size());
    #endif
    if(arrayWifi.size() > n){
      arrayWifi.remove(0);
    }
    //Serial.println("");
    //Serial.println(arrayWifi);
    root["wifi"] = arrayWifi;
    serializeJson(root, jsonString);
    #if DEBUG_MODE > 0
      Serial.print("\n");
      Serial.println(jsonString);
    #endif
    //server.send(200,"application/json",jsonString);
    return (jsonString);
  }
}

bool configWiFi(String ssidSTA, String passSTA){
//  WiFi.disconnect();
//  WiFi.mode(WIFI_OFF);
//  delay(1000);
//  WiFi.mode(WIFI_STA);
//  WiFi.setAutoReconnect(true);
  uint8_t mac[6];
  char nameHost[22];
  WiFi.macAddress(mac);
  sprintf(nameHost, "Home_IoT_Port-%02X%02X%02X", mac[3], mac[4], mac[5]);
  #if DEBUG_MODE > 0
    Serial.print("\nHost Name: ");
    Serial.println(nameHost);
    Serial.println("\nRutina conectAP");
  #endif
  IPAddress addrIP(0,0,0,0);
  int Intentos = 0;
  WiFi.persistent(true);
  WiFi.config(addrIP, addrIP, addrIP);
  WiFi.begin("", "");
  while (WiFi.status() != WL_CONNECTED && Intentos < 5){
    delay(100);
    Intentos++;
  }
  //ESP.eraseConfig();
  //ESP32.eraseConfig();
  WiFi.persistent(false);
  delay(500);
  IPAddress addrGat;
  IPAddress addrSub;
  IPAddress primaryDNS(8, 8, 8, 8); //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional
  WiFi.setAutoReconnect(true);
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
  if(passSTA  == ""){
    #if DEBUG_MODE > 0
      Serial.println("Conectando Sin PASS...");
    #endif
    WiFi.begin(ssidSTA.c_str());
  }else{
    #if DEBUG_MODE > 0
    Serial.println("Conectando Con PASS...");
    #endif
    WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
  }
  #if DEBUG_MODE > 0
    Serial.println("Conectando...");
    Serial.print("SSID: ");
    Serial.println(ssidSTA);
    Serial.print("PASS: ");
    Serial.println(passSTA);
  #endif
  uint8_t c = 0;
  while(WiFi.status() != WL_CONNECTED && c < 20) {
    c++;
    delay(500);
    #if DEBUG_MODE > 0
      Serial.print(".");
    #endif
  }
  if(WiFi.status() != WL_CONNECTED){
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
      Serial.print("DNS 1: ");
      Serial.println(WiFi.dnsIP(0));
      Serial.print("DNS 2: ");
      Serial.println(WiFi.dnsIP(1));
    #endif
    writeIP(WiFi.localIP());
    writeGATEWAY(WiFi.gatewayIP());
    writeSUBNET(WiFi.subnetMask());
    writeDNS1(WiFi.dnsIP(0));
    writeDNS2(WiFi.dnsIP(1));
//    wifiEeprom.writeSSID(ssidSTA);
//    wifiEeprom.writePASS(passSTA);
//      writeSSID(ssidSTA);
//      writePASS(passSTA);
    return (true);
  }
  
}

String respuestaConfig(int code){
  StaticJsonDocument<500> root;
  DynamicJsonDocument doc(200); 
  String jsonString;
  root["code"] = code;
  if(code == 0){
    IPAddress broadCast =  WiFi.localIP();
    String ipSTA = String(broadCast[0])+ String(".") + String(broadCast[1])+ String(".") + String(broadCast[2])+ String(".") + String(broadCast[3]);
    doc["dirIP"] = ipSTA;
    root["data"] = doc;
  }else{
    doc["dirIP"] = "";
    root["data"] = doc;
  }
  serializeJson(root, jsonString);
  //server.send(200,"application/json",jsonString);
  #if DEBUG_MODE > 0
  Serial.print("\n");
  Serial.println(jsonString);
  #endif
  return (jsonString);
}

void statuLED(){
  portENTER_CRITICAL(&timerMux);
  bool flagTime = flagTimer;
  portEXIT_CRITICAL(&timerMux);
  if(flagTime){
    portENTER_CRITICAL(&timerMux);
    flagTimer = false;
    portEXIT_CRITICAL(&timerMux);
    countStatusLed++;
    countStatusWifi++;
    if(countStatusLed > TIME_2Seg){
      countStatusLed = 0;
      #if DEBUG_MODE > 0
      Serial.print("LED nucleo: ");
      Serial.println(xPortGetCoreID());
      #endif
      
    }
    if(countStatusWifi > TIME_60Seg){
      countStatusWifi = 0;
        if(!flagConfig){
          if(WiFi.status() != WL_CONNECTED){
            _conectAccesPoint();
            _conectBot();
          }
          // if(WiFi.status() != WL_CONNECTED)
          // //if(!WiFi.isConnected())
          // {
          //   #if DEBUG_MODE > 0
          //   Serial.println("Wifi Desconectado.....");
          //   #endif
          //   // if(pass == ""){
          //   //   WiFi.begin(ssid.c_str());
          //   // }else{
          //   //   WiFi.begin(ssid.c_str(), pass.c_str());
          //   // }
          //   // delay(500);

          //   // uint8_t c = 0;
          //   // #if DEBUG_MODE > 0
          //   //     Serial.print("\nReconectando Wifi");
          //   //   #endif
          //   // while(WiFi.status() != WL_CONNECTED && c < 20) {
          //   //   c++;
          //   //   delay(500);
          //   //   #if DEBUG_MODE > 0
          //   //     Serial.print(".");
          //   //   #endif
              
          //   // }
          //   // if(WiFi.status() == WL_CONNECTED)
          //   // //if(WiFi.isConnected())
          //   // {
          //   //   configServer();
          //   // }
          // }else
          // {
          //   #if DEBUG_MODE > 0
          //   Serial.println("Conec. Wifi");
          //   #endif
          // }
      }
      
    }
    if(flagConfig){
      if(countStatusLed == 0 || countStatusLed == 3 || countStatusLed == 6){
        digitalWrite(statusLED, HIGH);
        if(digitalRead(bottonProg))
          digitalWrite(statusLED_Ext, HIGH);
      }else{
        digitalWrite(statusLED, LOW);
        if(digitalRead(bottonProg))
          digitalWrite(statusLED_Ext, LOW);
      }
    }
    else if(WiFi.status() == WL_CONNECTED)
    //if(WiFi.isConnected())
    {
      //else if(WiFi.isConnected()){
      if(countStatusLed == 0){
        digitalWrite(statusLED, HIGH);
        if(digitalRead(bottonProg))
          digitalWrite(statusLED_Ext, HIGH);
      }else{
        digitalWrite(statusLED, LOW);
        if(digitalRead(bottonProg))
          digitalWrite(statusLED_Ext, LOW);
      }
    }else{
      if(countStatusLed == 0 || countStatusLed == 3){
        digitalWrite(statusLED, HIGH);
        if(digitalRead(bottonProg))
          digitalWrite(statusLED_Ext, HIGH);
      }else{
        digitalWrite(statusLED, LOW);
        if(digitalRead(bottonProg))
          digitalWrite(statusLED_Ext, LOW);
      }
    }
    //verButton();

  }
}
//const char* ssidap     = "AP_Home_IoT_";
const char* passwordAP = "123456789";
void modeConfig(){ 
  uint8_t mac[6];
  char ssidap[20];
  WiFi.macAddress(mac);
  sprintf(ssidap, "AP_Home_IoT_%02X%02X%02X", mac[3], mac[4], mac[5]);
  // uint64_t chipid=ESP.getEfuseMac();
  // uint16_t chip = (uint16_t)(chipid >> 32);
  // String chipStrint = (String)chip;
  String ssidAP = ssidap;
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssidAP.c_str(), passwordAP);
  configServer();
}

void verButton(){
  portENTER_CRITICAL(&timerMux);
  bool flag = flagButton;
  portEXIT_CRITICAL(&timerMux);
  if(flag){
    countTemp++;
    portENTER_CRITICAL(&timerMux);
    flagButton = false;
    portEXIT_CRITICAL(&timerMux);
    if(!digitalRead(bottonProg)){
      if(flag1Vez){
        flag1Vez = false;
        //digitalWrite(LED, !digitalRead(LED));
        //enviarDeviceTelegram();
        digitalWrite(statusLED_Ext, !digitalRead(statusLED_Ext));
        //txDevice = true;
      }
      countProg++;
      if(countProg > TIME_4Seg){
        countProg = 0;
        //digitalWrite(LED, !digitalRead(LED));
        digitalWrite(statusLED_Ext, !digitalRead(statusLED_Ext));
        flagConfig = !flagConfig;
        if(flagConfig){
          modeConfig();
          //configServer();
          #if DEBUG_MODE > 0
            Serial.println("\nToggle Modo Configuracion");
          #endif
        }else{
          //Deja al equipo con configuracion de fabrica
          #if DEBUG_MODE > 0
            Serial.println("\nRESET de Fabrica");
          #endif
          erraseALL();
          delay(500);
          ESP.restart();
        }
       
      }
    }else{
      flag1Vez = true;
      countProg = 0;
      
    }
    // if(countTemp > (TIME_4Seg*10)){
    //   countTemp = 0;
    //   //sensor_temp.setWaitForConversion(false);
    //   Serial.println("\nRequesting temperatures...");
    //   //sensor_temp.requestTemperatures();
    //   //pinMode(ONE_WIRE_BUS, OUTPUT);
    //   //digitalWrite(ONE_WIRE_BUS, HIGH);
    //   //delay(100);
    //   //Serial.println("CMD Request OK");
    //   //temperatureC = sensor_temp.getTempCByIndex(0);
    //   //Serial.print("Temperatura: ");
    //   //Serial.println(temperatureC);
    //   ///
    //   //delay(100);
    //   //disableISR_Timer();
    //   sensor_temp.requestTemperatures();
    //   Serial.println("DONE");
    //   //float tempC = sensor_temp.getTempC(insideThermometer);
    //   temperatureC = sensor_temp.getTempC(insideThermometer);
    //   //initISR_Timer();
    //   if(temperatureC == DEVICE_DISCONNECTED_C) 
    //   {
    //     Serial.println("Error: Could not read temperature data");
    //     return;
    //   }
    //   Serial.print("Temp C: ");
    //   Serial.println(temperatureC);

    // }
  }
}

void readEepromAllUser(void){
  readAllUser();
  if(cuentaUser.account[0] == 0xFFFFFFFF || cuentaUser.account[0] == 0x0){
    falgAdmin = false;
  }
  else{
    falgAdmin = true;
  }
}

void imprimirAllUser(void){
  //readAllUser();
  readEepromAllUser();
  #if DEBUG_MODE > 0
    Serial.println("Usuarios Registrados en la EEPROM:");
    for (size_t i = 0; i < USER_ALL; i++)
    {
      Serial.print("Usuario "); Serial.print(i); Serial.print(": ");
      if(cuentaUser.account[i] == 0xFFFFFFFF)
        Serial.println("Sin Usuasrio");
      else
        Serial.println(cuentaUser.account[i]);
    }
  #endif
}

void verTelegram(void){
  bool envMsjAdm = false;
  portENTER_CRITICAL(&timerMux);
  bool flag = flagTelegram;
  portEXIT_CRITICAL(&timerMux);
  if(flag){
    portENTER_CRITICAL(&timerMux);
    flagTelegram = false;
    portEXIT_CRITICAL(&timerMux);
    countTelegram++;
    if(countTelegram > TIME_2Seg){
      countTelegram = 0;
      int requestGet = requestGetUpdateClear(tokenBot);
      if(requestGet == 200){
        #if DEBUG_MODE > 0
        Serial.print("\nver Telegram nucleo: ");
        Serial.println(xPortGetCoreID());
        #endif
        if(userMsg.messageType == MessageText){
          // #if DEBUG_MODE > 0
          //   Serial.println("\nRespuesta de requestGetUpdateClear Exitosa");
          //   Serial.print("Mensaje Tipo: "); Serial.println(userMsg.text);
          //   Serial.println(userMsg.user.id);
          //   Serial.println(userMsg.user.username);
          //   Serial.println(userMsg.user.firstName);
          //   Serial.println(userMsg.user.lastName);
          //   Serial.println(userMsg.text);
          // #endif
          if(userMsg.text == CommadSTART){
            #if DEBUG_MODE > 0
              Serial.println("\nSe Rx CommadSTART");
            #endif
            String msgStart = "Bienvenid@ al BOT que Automatiza el Porton.\n/registrar - Comando para Registrarse en el BOT.\nSu ID de Usuari@ es: *";
            msgStart += userMsg.user.id;
            msgStart += "*\nSu Nombre de Usuari@: *";
            msgStart += userMsg.user.username;
            msgStart += "*";
            
            int requestCommand = sendComand(userMsg.user.id, msgStart, conFormat);
            if(requestCommand == 200){
              #if DEBUG_MODE > 0
                Serial.println("Respuesta a la Tx del BOT Exitosa");
              #endif
            }else{
              requestCommand = sendComand(userMsg.user.id, msgStart, sinFormat);
              if(requestCommand == 200){
                #if DEBUG_MODE > 0
                  Serial.println("Reenvio a la Tx del BOT Exitosa");
                #endif
              }else{
                #if DEBUG_MODE > 0
                  Serial.println("Reenvio a la Tx del BOT Fallida");
                #endif
              }
            }
          }else if(userMsg.text == CommadREGISTRO){
            if(!userNoIsRegistred(userMsg.user.id)){
              String cmdRegistro = "Usuari@ ya Registrado, puede acceder mediante:\n/menu - Comando para ver opciones en el BOT.\nSu ID de Usuari@ es: `";
              cmdRegistro += userMsg.user.id;
              cmdRegistro +="`\nSu Nombre de Usuari@: `";
              cmdRegistro += userMsg.user.username;
              cmdRegistro += "`";
              int requestCommand = sendComand(userMsg.user.id, cmdRegistro, conFormat);
              if(requestCommand == 200){
                #if DEBUG_MODE > 0
                  Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                #endif
              }else{
                #if DEBUG_MODE > 0
                  Serial.println("\nReenviando Rspuesta");
                #endif
                requestCommand = sendComand(userMsg.user.id, cmdRegistro, conFormat);
                if(requestCommand == 200){
                  #if DEBUG_MODE > 0
                    Serial.println("\nReenvio a la Tx del BOT Exitosa");
                  #endif
                }else{
                  #if DEBUG_MODE > 0
                    Serial.println("\nReenvio a la Tx del BOT Fallida");
                  #endif
                }
              }
            }else{
              readAllUser();
              if(cuentaUser.account[0] == 0xFFFFFFFF || cuentaUser.account[0] == 0x0){
                String txtMsg = userMsg.user.username;
                uint8_t lastStringLength = txtMsg.length();
                if(lastStringLength < 5 || userMsg.user.username == '\0'){
                  enviarFaltaUserName();
                }else{
                  cuentaUser.account[0] = userMsg.user.id;
                  writeUser_i(0);
                  writeUserName_i(0, userMsg.user.username);
                  writeUserBlock_i(0, false);
                  String cmdRegistro = "Usuari@ Registrado como *Administrador* del Sistema, puede acceder mediante:\n/menu - Comando para ver opciones en el BOT.\nSu ID de Usuari@ es: `";
                  cmdRegistro += userMsg.user.id;
                  cmdRegistro +="`\nSu Nombre de Usuari@: `";
                  cmdRegistro += userMsg.user.username;
                  cmdRegistro += "`";
                  int requestCommand = sendComand(userMsg.user.id, cmdRegistro, conFormat);
                  if(requestCommand == 200){
                    #if DEBUG_MODE > 0
                      Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                    #endif
                  }
                }

              }else{
                //bool statusReg = 
                if(verRegisterActive()){
                  String txtMsg = userMsg.user.username;
                  uint8_t lastStringLength = txtMsg.length();
                  if(lastStringLength == 0 || userMsg.user.username == '\0'){
                    enviarFaltaUserName();
                  }else{
                    enviarRegistrarTel();
                    guardarREgistrarTel();
                  }
                  
                }else{
                  registoPendiente();
                }
                
              }
            }
          }else if(userMsg.text == CommadMENU){
            if(!userNoIsRegistred(userMsg.user.id)){
              if(!userIsBlock(userMsg.user.id)){
                int requestCommand = sendMenu(userMsg.user.id);
                if(requestCommand == 200){
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                  #endif
                }else{
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Fallida");
                  #endif
                }
              }else{
                userBloqueado();
              }
            }else{
              userNoRegister();
            }
            
          // }
          // else if(userMsg.text == CommadOption){
          //   int requestCommand = sendOption(userMsg.user.id);
          }
          else if(userMsg.text == Commad_OpenP){
            if(!userNoIsRegistred(userMsg.user.id)){
              if(!userIsBlock(userMsg.user.id)){
                String mensaje = "Usuari@: *";
                mensaje += userMsg.user.username;
                if(estados == PortonOpen){
                  mensaje += "*\nPorton ya esta Abierto";
                }else if(estados == PortonClouse){
                  ActPorton = true;
                  envMsjAdm = true;
                  mensaje += "*\nPorton se esta *Abriendo...*";
                  userID = userMsg.user.id;
                }else if(estados == PortonAbriendo || estados == PortonCerrando){
                  ActPorton = true;
                  envMsjAdm = true;
                  mensaje += "*\nComando *Abrir/Cerrar* Porton";
                  userID = userMsg.user.id;
                }
                int requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
                if(requestCommand == 200){
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                  #endif
                }else{
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Fallida");
                  #endif
                }
                if(envMsjAdm){
                  envMsjAdm = false;
                  if(cuentaUser.account[0] != userMsg.user.id){
                    mensaje = "El Usuari@: *";
                    mensaje += userMsg.user.username;
                    mensaje += "*\nActivo *Abrir* el Porton por el Bot: *";
                    mensaje += userNameBot;
                    mensaje += "*";
                    requestCommand = sendComand(cuentaUser.account[0], mensaje, conFormat);
                  }
                }
              }else{
                userBloqueado();
              }
            }else{
              userNoRegister();
            }
          }
          else if(userMsg.text == Commad_ClouseP){
            if(!userNoIsRegistred(userMsg.user.id)){
              if(!userIsBlock(userMsg.user.id)){
                String mensaje = "Usuari@: *";
                mensaje += userMsg.user.username;
                if(estados == PortonClouse){
                  mensaje += "*\nPorton ya esta *Cerrado*";
                }else if(estados == PortonOpen){
                  ActPorton = true;
                  envMsjAdm = true;
                  mensaje += "*\nPorton se esta *Cerrando...*";
                  userID = userMsg.user.id;
                }else if(estados == PortonAbriendo || estados == PortonCerrando){
                  ActPorton = true;
                  envMsjAdm = true;
                  mensaje += "*\nComando *Abrir/Cerrar* Porton";
                  userID = userMsg.user.id;
                }
                int requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
                if(requestCommand == 200){
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                  #endif
                }else{
                  #if DEBUG_MODE > 0
                    Serial.println("\nReenviando Rspuesta");
                  #endif
                  requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
                  if(requestCommand == 200){
                    #if DEBUG_MODE > 0
                      Serial.println("\nReenvio a la Tx del BOT Exitosa");
                    #endif
                  }else{
                    #if DEBUG_MODE > 0
                      Serial.println("\nReenvio a la Tx del BOT Fallida");
                    #endif
                  }
                }
                if(envMsjAdm){
                  envMsjAdm = false;
                  if(cuentaUser.account[0] != userMsg.user.id){
                    mensaje = "Usuari@: *";
                    mensaje += userMsg.user.username;
                    mensaje += "*\nActivo *Cierre* de Porton por el Bot: *";
                    mensaje += userNameBot;
                    mensaje += "*";
                    requestCommand = sendComand(cuentaUser.account[0], mensaje, conFormat);
                    #if DEBUG_MODE > 0
                    Serial.print("\nName Bot: "); Serial.println(userNameBot);
                    #endif
                  }
                }
              }else{
                userBloqueado();
              }
            }else{
              userNoRegister();
            }
          }
          else if(userMsg.text == Commad_EstatusP){
            if(!userNoIsRegistred(userMsg.user.id)){
              if(!userIsBlock(userMsg.user.id)){
                String mensaje = "Usuari@: *";
                mensaje += userMsg.user.username;
                if(estados == PortonClouse){
                  mensaje += "*\nPorton: *Cerrado*";
                }else if(estados == PortonOpen){
                  mensaje += "*\nPorton: *Abierto*";
                }else if(estados == PortonAbriendo || estados == PortonCerrando){
                  mensaje += "*\nPorton: *entre Abierto...*";
                }
                if(isPortonON){
                  mensaje += "\nPorton: *ON*";
                }else{
                  mensaje += "\nPorton: *OFF*";
                }
                int requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
                if(requestCommand == 200){
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                  #endif
                }else{
                  #if DEBUG_MODE > 0
                    Serial.println("\nReenviando Rspuesta");
                  #endif
                  requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
                  if(requestCommand == 200){
                    #if DEBUG_MODE > 0
                      Serial.println("\nReenvio a la Tx del BOT Exitosa");
                    #endif
                  }else{
                    #if DEBUG_MODE > 0
                      Serial.println("\nReenvio a la Tx del BOT Fallida");
                    #endif
                  }
                }
                // if(cuentaUser.account[0] != userMsg.user.id){
                //   mensaje = "El Usuario: ";
                //   mensaje += userMsg.user.username;
                //   mensaje += "\nSolicito Estatus del Porton";
                //   requestCommand = sendComand(cuentaUser.account[0], mensaje, sinFormat);
                // }
              }else{
                userBloqueado();
              }
              
            }else{
              userNoRegister();
            }
          }
          else if(userMsg.text == Command_Temperatura){
            if(!userNoIsRegistred(userMsg.user.id)){
              if(!userIsBlock(userMsg.user.id)){
                readDS1820();
                String mensaje = "Usuari@: *";
                mensaje += userMsg.user.username;
                mensaje += "*\nTemperatura Del Equipo: *";
                //mensaje += temperatureC;
                mensaje += String(temperatureC, 1);
                mensaje += " ºC*";
                int requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
                if(requestCommand == 200){
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                  #endif
                }
                // if(userMsg.user.id != cuentaUser.account[0]){
                //   sendComand(cuentaUser.account[0], mensaje, conFormat);
                // }
              }
            }else{
              userNoRegister();
            }
          }
          else if(userMsg.text == Commad_Power){
            if(!userNoIsRegistred(userMsg.user.id)){
              if(!userIsBlock(userMsg.user.id)){
                digitalWrite(powerPorton, !digitalRead(powerPorton));
                String mensaje = "Usuari@: *";
                mensaje += userMsg.user.username;
                if(digitalRead(powerPorton)){
                  mensaje += "*\nActivo Porton en: *ON*";
                }else{
                  mensaje += "*\nActivo Porton en: *OFF*";
                }
                int requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
                if(requestCommand == 200){
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                  #endif
                }
                if(userMsg.user.id != cuentaUser.account[0]){
                  sendComand(cuentaUser.account[0], mensaje, conFormat);
                }
              }
            }else{
              userNoRegister();
            }
          }
          else if(userMsg.text == ComandTemp_Int){
            if(!userNoIsRegistred(userMsg.user.id)){
              if(!userIsBlock(userMsg.user.id)){
                float tempInt = (temprature_sens_read() - 32) / 1.8;
                String mensaje = "Temperatura Interna: ";
                mensaje += String(tempInt, 1);
                mensaje += " ºC";
                int requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
                if(requestCommand == 200){
                  #if DEBUG_MODE > 0
                    Serial.println("\nRespuesta a la Tx del BOT Exitosa");
                  #endif
                }
              }else{
                userBloqueado();
              }
            }else{
              userNoRegister();
            }
          }
          else if(userMsg.text == Commad_Ubicacion){
            String message;
            int requestCommand;
            if(latitude == 0.0 && longitude == 0.0){
              message = "Ubicacion no Registrada";
              requestCommand = sendComand(userMsg.user.id, message, conFormat);
            }else{
              // message = "Latitude: " + String(longitude, 6) + "\n";
              // message += "Longitude: " + String(latitude, 6);
              requestCommand = sendUbicacion(userMsg.user.id, latitude, longitude);
            }
            
            if(requestCommand == 200){
              #if DEBUG_MODE > 0
                Serial.println("Respuesta a la Tx del BOT Exitosa");
              #endif
            }
          }
          else{
            String reply;
            if(!userNoIsRegistred(userMsg.user.id)){
              reply = "Usuari@: *" ;
              reply += userMsg.user.username;
              reply += "*.\nYa usted esta Registrado";
              reply += ".\nComando: /menu";
              reply += ".\nPara opciones del sistema";
              int requestCommand = sendComand(userMsg.user.id, reply, conFormat);
              if(requestCommand == 200){
                #if DEBUG_MODE > 0
                  Serial.println("Respuesta a la Tx del BOT Exitosa");
                #endif
              }else{
                #if DEBUG_MODE > 0
                  Serial.println("Respuesta a la Tx del BOT Fallida");
                #endif
              }
            }else{
              reply = "Bienvenid@ *" ;
              reply += userMsg.user.username;
              reply += "*.\nComando: /start";
              int requestCommand = sendComand(userMsg.user.id, reply, conFormat);
              if(requestCommand == 200){
                #if DEBUG_MODE > 0
                  Serial.println("Respuesta a la Tx del BOT Exitosa");
                #endif
              }else{
                #if DEBUG_MODE > 0
                  Serial.println("Respuesta a la Tx del BOT Fallida");
                #endif
              }
            }
          }
        }else if(userMsg.messageType == MessageLocation){
          String mensaje;
          if(cuentaUser.account[0] == userMsg.user.id){
            mensaje = "Ubicacion Recibida y Almacenada:\n";
            mensaje += "Latitud: `";
            mensaje += userMsg.location.latitude;
            mensaje += "`\nLongitud: `";
            mensaje += userMsg.location.longitude;
            mensaje += "`";
            latitude = userMsg.location.latitude;
            longitude = userMsg.location.longitude;
          }else{
            mensaje = "No tiene credenciales para almacenar la Ubicacion\n";
          }
          int requestCommand = sendComand(userMsg.user.id, mensaje, conFormat);
          if(requestCommand == 200){
            #if DEBUG_MODE > 0
              Serial.println("Respuesta a la Tx del BOT Exitosa");
            #endif
          }
        }
      }
        // else if (userMsg.messageType == MessageQuery){
        //   if(userMsg.callbackQueryData){
        //     //if(userMsg.callbackQueryData == queryON){
        //     if(strcmp(userMsg.callbackQueryData, queryON.c_str())==0){
        //       digitalWrite(LED, HIGH);
        //       //sendanswerCallbackQuery();
        //       String mensaje = "Usuario: ";
        //       mensaje += userMsg.user.username;
        //       mensaje += " LED en ON";
        //       int requestCommand = sendComand(userMsg.user.id, mensaje);
        //       if(requestCommand == 200){
        //         #if DEBUG_MODE > 0
        //           Serial.println("\nRespuesta a la Tx del BOT Exitosa");
        //         #endif
        //       }else{
        //         #if DEBUG_MODE > 0
        //           Serial.println("\nRespuesta a la Tx del BOT NO Realizada");
        //         #endif
        //       }
        //     //}else if(userMsg.callbackQueryData == queryOFF){
        //     }else if(strcmp(userMsg.callbackQueryData, queryOFF.c_str())==0){
        //       digitalWrite(LED, LOW);
        //       //sendanswerCallbackQuery();
        //       String mensaje = "Usuario: ";
        //       mensaje += userMsg.user.username;
        //       mensaje += " LED en OFF";
        //       int requestCommand = sendComand(userMsg.user.id, mensaje);
        //       if(requestCommand == 200){
        //         #if DEBUG_MODE > 0
        //           Serial.println("\nRespuesta a la Tx del BOT Exitosa");
        //         #endif
        //       }else{
        //         #if DEBUG_MODE > 0
        //           Serial.println("\nRespuesta a la Tx del BOT NO Realizada");
        //         #endif
        //       }
        //     //}else if(userMsg.callbackQueryData == queryToggle){
        //     }else if(strcmp(userMsg.callbackQueryData, queryToggle.c_str())==0){
        //       digitalWrite(LED, !digitalRead(LED));
        //       //sendanswerCallbackQuery();
        //       String mensaje = "Usuario: ";
        //       mensaje += userMsg.user.username;
        //       if(digitalRead(LED))
        //         mensaje += " LED en ON";
        //       else
        //         mensaje += " LED en OFF";
        //       int requestCommand = sendComand(userMsg.user.id, mensaje);
        //       if(requestCommand == 200){
        //         #if DEBUG_MODE > 0
        //           Serial.println("\nRespuesta a la Tx del BOT Exitosa");
        //         #endif
        //       }else{
        //         #if DEBUG_MODE > 0
        //           Serial.println("\nRespuesta a la Tx del BOT NO Realizada");
        //         #endif
        //       }
        //     //}else if(userMsg.callbackQueryData == queryStatus){
        //     }else if(strcmp(userMsg.callbackQueryData, queryStatus.c_str())==0){
        //       //sendanswerCallbackQuery();
        //       String mensaje = "Usuario: ";
        //       mensaje += userMsg.user.username;
        //       if(digitalRead(LED))
        //         mensaje += " LED en ON";
        //       else
        //         mensaje += " LED en OFF";
        //       int requestCommand = sendComand(userMsg.user.id, mensaje);
        //       if(requestCommand == 200){
        //         #if DEBUG_MODE > 0
        //           Serial.println("\nRespuesta a la Tx del BOT Exitosa");
        //         #endif
        //       }else{
        //         #if DEBUG_MODE > 0
        //           Serial.println("\nRespuesta a la Tx del BOT NO Realizada");
        //         #endif
        //       }
        //     }
        //   }
        // }

    }
  }
}

bool userNoIsRegistred(int32_t verUserID){
 bool  resp = true;
 for (uint8_t i = 0; i < USER_ALL; i++)
 {
   //userMsg.user.id
   if(verUserID == cuentaUser.account[i]){
     resp = false;
     break;
   }
 }
 return (resp);
}

void userNoRegister(void){
  String msgReg = "Usuari@ no Registrado envíe:.\n/registrar - Para Registrarse en el BOT.\nSu ID de Usuari@ es: *";
  msgReg += userMsg.user.id;
  msgReg += "*\nNombre Usuari@: *";
  msgReg += userMsg.user.username;
  msgReg += "*";
  int requestCommand = sendComand(userMsg.user.id, msgReg, conFormat);
  if(requestCommand == 200){
    #if DEBUG_MODE > 0
      Serial.println("\nEnvío a la Tx del BOT Exitosa");
    #endif
  }else{
    #if DEBUG_MODE > 0
      Serial.println("\nEnvio de comando Fallida");
    #endif
  }
}

void enviarRegistrarTel(void){
  String cmdoRegistro = "Su Registro fue enviado al Administrador.\nCuando este Responda se Actualizara su Estatus.";
  cmdoRegistro +="\nNombre de Usuari@: *";
  cmdoRegistro += userMsg.user.username;
  cmdoRegistro += "*\nSu ID de Usuari@ es: *";
  cmdoRegistro += userMsg.user.id;
  cmdoRegistro += "*";
  int requestCommand = sendComand(userMsg.user.id, cmdoRegistro, conFormat);
  if(requestCommand == 200){
    #if DEBUG_MODE > 0
      Serial.println("enviarRegistrarTel BOT Exitosa");
    #endif
  }else{
    #if DEBUG_MODE > 0
      Serial.println("enviarRegistrarTel BOT Fallida");
    #endif
  }
  cmdoRegistro = "";
  cmdoRegistro = "Administrador, el siguiente:.\n";
  cmdoRegistro += "Usuari@: `";
  cmdoRegistro += userMsg.user.username;
  cmdoRegistro += "`\nCon ID de Usuari@: `";
  cmdoRegistro += userMsg.user.id;
  cmdoRegistro += "`\nRealizó una Solicitud de registro en el sistema";
  requestCommand = sendComand(cuentaUser.account[0], cmdoRegistro, conFormat);
  if(requestCommand == 200){
    #if DEBUG_MODE > 0
      Serial.println("enviarRegistrarTel BOT Exitosa");
    #endif
  }else{
    #if DEBUG_MODE > 0
      Serial.println("enviarRegistrarTel BOT Fallida");
    #endif
  }
}

void enviarFaltaUserName(void){
  String cmdoRegistro = "Su Registro no puede ser procesado por no poseer *Nombre de Usuario* en el perfil de Telegram.\nPara esto debe ir al menu *Ajuste* en la App de Telegram y colocar en su cuenta el *Nombre de Usuario*";
  cmdoRegistro +="\nLuego de esto realice un nuevo Registro";
  int requestCommand = sendComand(userMsg.user.id, cmdoRegistro, conFormat);
  if(requestCommand == 200){
    #if DEBUG_MODE > 0
      Serial.println("enviarRegistrarTel BOT Exitosa");
    #endif
  }else{
    #if DEBUG_MODE > 0
      Serial.println("enviarRegistrarTel BOT Fallida");
    #endif
  }
}

void limpiarRegUser(void){
  #if DEBUG_MODE > 0
  Serial.println("Clear Registros");
  #endif
  for (uint8_t i = 0; i < USER_ALL; i++)
  {
    regUserStore[i].userIdReg = 0;
    regUserStore[i].userNameReg = "";
  }
}

void guardarREgistrarTel(void){
  #if DEBUG_MODE > 0
  Serial.println("Almac. Reg");
  #endif
  for (uint8_t i = 0; i < USER_ALL; i++)
  {
    if (regUserStore[i].userIdReg == 0)
    {
      regUserStore[i].userIdReg = userMsg.user.id;
      regUserStore[i].userNameReg = userMsg.user.username;
      break;
    }
  }
}
//Retorna TRUE si no Tiene Registro Pendiente
bool verRegisterActive(void){
  //readAllUserRegPen();
  #if DEBUG_MODE > 0
  Serial.println("\nReg Pendiente???");
  #endif
  // for (uint8_t i = 0; i < USER_ALL; i++){
  //   Serial.print("User Name "); Serial.print(i); Serial.print(": "); Serial.println(regUserStore[i].userNameReg);
  //   Serial.print("User ID "); Serial.print(i); Serial.print(": "); Serial.println(regUserStore[i].userIdReg);
  // }
  bool resp = true;
  for (uint8_t i = 0; i < USER_ALL; i++){
    if(regUserStore[i].userIdReg == userMsg.user.id){
      resp = false;
      #if DEBUG_MODE > 0
      Serial.println("Hay Registro Pendiente");
      #endif
      break;
    }
  }
  return (resp);
}

void registoPendiente(void){
   String cmdoRegistro = "Ya posee un Registro pendiente.\nPongase en contacto con el Administrador para que responda su solicitud.";
  cmdoRegistro +="\nNombre de Usuari@: *";
  cmdoRegistro += userMsg.user.username;
  cmdoRegistro += "*\nSu ID de Usuari@ es: *";
  cmdoRegistro += userMsg.user.id;
  cmdoRegistro += "*";
  int requestCommand = sendComand(userMsg.user.id, cmdoRegistro, conFormat);
  if(requestCommand == 200){
    #if DEBUG_MODE > 0
      Serial.println("enviarRegistrarTel BOT Exitosa");
    #endif
  }else{
    #if DEBUG_MODE > 0
      Serial.println("enviarRegistrarTel BOT Fallida");
    #endif
  }
}

bool adminRegistra(void){
  bool resp = false;
  readAllUser();
  for (uint8_t i = 0; i < USER_ALL; i++)
  {
    if(cuentaUser.account[i] == 0xFFFFFFFF || cuentaUser.account[i] == 0x0){
      cuentaUser.account[i] = admidId;
      writeUser_i(i);
      writeUserName_i(i, adminUser);
      resp = true;
      break;
    }
  }
  return resp;
}

bool adminRegistraUser(void){
  #if DEBUG_MODE > 0
  Serial.println("Grabando Usuario...");
  #endif
  bool resp = false;
  readAllUser();
  //readEepromAllUser();
  readAllUserName();
  readAllUserBlock();
  for (uint8_t i = 0; i < USER_ALL; i++)
  {
    if(cuentaUser.account[i] == 0xFFFFFFFF || cuentaUser.account[i] == 0x0){
      cuentaUser.account[i] = userID;
      writeUser_i(i);
      writeUserName_i(i, userName);
      writeUserBlock_i(i, userBlock);
      resp = true;
      break;
    }
  }
  return resp;
}

bool adminBlockUser(void){
  bool resp = false;
  writeUserBlock_i(regID, userBlock);
  resp = true;
return (resp);
}

void enviarRegPend(void){
  String jsonString;
  StaticJsonDocument<2048> root;
  root.clear();
  DynamicJsonDocument doc(1500);
  doc.clear();
  JsonArray arrayWifi = doc.to<JsonArray>();
  JsonObject s1 = arrayWifi.createNestedObject();
  s1.clear();
  root["code"] = 0;
  uint8_t countReg = 0;
  for (int i = 0; i < USER_ALL; ++i){
    if (regUserStore[i].userIdReg != 0){
      countReg++;
    }
  }
  if(countReg == 0){
    //arrayWifi.add();
  }else{
    for (int i = 0; i < countReg; ++i){
      s1["userId"] = regUserStore[i].userIdReg;
      s1["userName"] = regUserStore[i].userNameReg;
      arrayWifi.add(s1);
    }
  }
  arrayWifi.remove(0);
  root["data"] = arrayWifi;
  serializeJson(root, jsonString);
  #if DEBUG_MODE > 0
  Serial.println("\nRegistros Pendientes:");
  Serial.println(jsonString);
  #endif
  sendRequest(200, jsonString);
  limpiarRegUser();
}

void enviarUserRequest(void){
  readAllUserName();
  readAllUser();
  readAllUserBlock();
  String jsonString;
  StaticJsonDocument<2048> root;
  root.clear();
  DynamicJsonDocument doc(1500);
  doc.clear();
  JsonArray arrayWifi = doc.to<JsonArray>();
  JsonObject s1 = arrayWifi.createNestedObject();
  s1.clear();
  root["code"] = 0;

  for (int i = 0; i < USER_ALL; ++i){
    if(cuentaUser.account[i] != 0xFFFFFFFF && cuentaUser.account[i] != 0x0){
      s1["regID"] = i;
      s1["userId"] = cuentaUser.account[i];
      s1["userName"] = cuentaUserName.account[i];
      s1["userBlock"] = cuentaUserBlock.account[i];
      arrayWifi.add(s1);
    }
  }
  arrayWifi.remove(0);
  root["data"] = arrayWifi;
  serializeJson(root, jsonString);
  #if DEBUG_MODE > 0
  Serial.println("\nVer All User:");
  Serial.println(jsonString);
  #endif
  sendRequest(200, jsonString);
}

bool verCuenta(String scanUserName, int32_t scanUserId){
  bool resp = false;
  readAllUser();
  readAllUserName();
  for(int i = 0; i < USER_ALL; ++i){
    if(scanUserId == cuentaUser.account[i] && scanUserName == cuentaUserName.account[i]){
      resp = true;
      break;
    }
  }
  return resp;
}

bool verAdminBlank(){
  bool resp = false;
  readAllUser();
  if(cuentaUser.account[0] == 0xFFFFFFFF){
    resp = true;
  }
  return resp;
}

void envRegistroUser(void){
  String msgStart;
  msgStart = "Su solicitud fue aceptada y registrada por el Administrador.";
  msgStart += "\nEnvíe el comando:";
  msgStart += "\n/menu -> para ver las opciones del Sistema";
  msgStart += "\nSus credenciales para registrarse en la App son:";
  msgStart += "\nNombre de Usuari@: `";
  msgStart += userName;
  msgStart += "`\nID de Usuari@: `";
  msgStart += userID;
  msgStart += "`";
  int requestCommand = sendComand(userID, msgStart, conFormat);
  if(requestCommand == 200){
    #if DEBUG_MODE > 0
      Serial.println("Respuesta a la Tx del BOT Exitosa");
    #endif
  }
}

void enviarDeviceTelegram(void){
  #if DEBUG_MODE > 0
  Serial.print("EnvDev nucleo: ");
  Serial.println(xPortGetCoreID());
  #endif
  String mensaje = "Por Dispositivo se Activo el :";
    // if(digitalRead(LED))
    //   mensaje += " LED en ON";
    // else
    //   mensaje += " LED en OFF";
    int requestCommand = sendComand(cuentaUser.account[0], mensaje, sinFormat);
    if(requestCommand == 200){
      #if DEBUG_MODE > 0
        Serial.println("\nRespuesta a la Tx del BOT Exitosa");
      #endif
    }
}

void enviarWebAbPTelegram(void){
  String mensaje = "Usuari@: *";
  mensaje += userName;
  mensaje += "*\nActivo *Abrir* el Porton por *App*";
  sendComand(userID, mensaje, conFormat);
  if(userID != cuentaUser.account[0]){
    sendComand(cuentaUser.account[0], mensaje, conFormat);
  }
}

void enviarWebAcPTelegram(void){
  String mensaje = "Usuari@: *";
  mensaje += userName;
  mensaje += "*\nActivo *Abrir/Cerrar* el Porton por *App*";
  sendComand(userID, mensaje, conFormat);
  if(userID != cuentaUser.account[0]){
    sendComand(cuentaUser.account[0], mensaje, conFormat);
  }
}

void enviarWebCePTelegram(void){
  String mensaje = "Usuari@: *";
  mensaje += userName;
  mensaje += "*\nActivo *Cerrar* el Porton por *App*";
  sendComand(userID, mensaje, conFormat);
  if(userID != cuentaUser.account[0]){
    sendComand(cuentaUser.account[0], mensaje, conFormat);
  }
}

void enviarWebPoPTelegram(void){
  String mensaje = "Usuari@: *";
  mensaje += userName;
  if(digitalRead(powerPorton)){
    mensaje += "*\nActivo *Power ON* el Porton por *App*";
  }else{
     mensaje += "*\nActivo *Power OFF* el Porton por *App*";
  }
  sendComand(userID, mensaje, conFormat);
  if(userID != cuentaUser.account[0]){
    sendComand(cuentaUser.account[0], mensaje, conFormat);
  }
}

void enviarLocalTelegram(void){
  String mensaje = "Se *Activo* el Porton de manera *Local*";
  sendComand(cuentaUser.account[0], mensaje, conFormat);
}

void enviarContLocalTelegram(void){
  String mensaje = "Se *Activo* el Porton por *Control Local*";
  sendComand(cuentaUser.account[0], mensaje, conFormat);
}

void enviarAbrirLocalTelegram(void){
  String mensaje = "Se *Activo Abrir* el Porton por *Control Local*";
  sendComand(cuentaUser.account[0], mensaje, conFormat);
}

void enviarCerrarLocalTelegram(void){
  String mensaje = "Se *Activo Cerrar* el Porton por *Control Local*";
  sendComand(cuentaUser.account[0], mensaje, conFormat);
}

void enviarTimeOpenAdm(void){
  String mensaje = "Porton *Abierto* en aproximadamente:\n*";
  mensaje += timeOpenClousPot/10;
  mensaje += "* seg";
  sendComand(userID, mensaje, conFormat);
  if(userID != cuentaUser.account[0]){
    sendComand(cuentaUser.account[0], mensaje, conFormat);
  }
}

void enviarTimeClouseAdm(void){
  String mensaje = "Porton *Cerrado* en aproximadamente:\n*";
  mensaje += timeOpenClousPot/10;
  mensaje += "* seg";
  sendComand(userID, mensaje, conFormat);
  if(userID != cuentaUser.account[0]){
    sendComand(cuentaUser.account[0], mensaje, conFormat);
  }
}

void enviarOpenAdm(void){
  String mensaje = "Porton *Abierto* completamente";
  // mensaje += timeOpenClousPot/10;
  // mensaje += "* seg";
  sendComand(userID, mensaje, conFormat);
  if(userID != cuentaUser.account[0]){
    sendComand(cuentaUser.account[0], mensaje, conFormat);
  }
}

void enviarClouseAdm(void){
  String mensaje = "Porton *Cerrado* completamente";
  // mensaje += timeOpenClousPot/10;
  // mensaje += "* seg";
  sendComand(userID, mensaje, conFormat);
  if(userID != cuentaUser.account[0]){
    sendComand(cuentaUser.account[0], mensaje, conFormat);
  }
}

bool userIsBlock(int32_t idUser){
  for (uint8_t i = 0; i < USER_ALL; i++){
    if(cuentaUser.account[i] == idUser){
      userBlock = cuentaUserBlock.account[i];
      break;
    }
  }
  return userBlock;
}

void userBloqueado(void){
  String msgReg = "Usuari@ Bloqueado.\nDirijase con el Administrador para verificar su Estatus";
  //msgReg += userMsg.user.id;
  msgReg += "\nNombre Usuari@: *";
  msgReg += userMsg.user.username;
  msgReg += "*";
  int requestCommand = sendComand(userMsg.user.id, msgReg, conFormat);
  if(requestCommand == 200){
    #if DEBUG_MODE > 0
      Serial.println("\nEnvío a la Tx del BOT Exitosa");
    #endif
  }else{
    #if DEBUG_MODE > 0
      Serial.println("\nEnvio de comando Fallida");
    #endif
  }
}


void porton_FSM(void){
  // if(isActManualPor)
  // {
  //   flagActRF = true;
  // }
  portENTER_CRITICAL(&timerMux);
  bool flag = flagFSM;
  uint8_t timePorton = countTime;
  portEXIT_CRITICAL(&timerMux);
  if(flag){
    countFSM++;
    countScanSensor++;
    portENTER_CRITICAL(&timerMux);
    flagFSM = false;
    portEXIT_CRITICAL(&timerMux);
  }
  if(!flagScanSensor){
    if(countScanSensor > 4){
      flagScanSensor = true;
    }
  }else{
    countScanSensor = 0;
  }
  // bool flagScanSensor = false;
  // uint8_t  countScanSensor;
  switch (estados)
  {
    case (PortonClouse):
      if(medirTimePot){
        medirTimePot = false;
        timeOpenClousPot = timePorton;
        TxAdminTimeOpen = true;
        #if DEBUG_MODE > 0
        Serial.print("Time Clouse: ");
        Serial.println(timeOpenClousPot);
        #endif
      }
      //Pa Enviar por Telegram: Porton *Cerrado* completamente o Porton *Abierto* completamente
      if(infPorAbCe){
        infPorAbCe = false;
        TxAdminOpCl = true;           //Envia Msj: Porton *Cerrado* completamente
      }
      if(ActPorton){
        #if DEBUG_MODE > 0
        Serial.println("Ejecuta ActPorton");
        #endif
        ActPorton = false;
        estados = PortonA_Act;
        medirTimePot = true;
        infPorAbCe = false;
        flagAct1Vez = false;
      }
      else if(isActManualPor)
      //else if(flagActRF)
      {
        //flagActRF = false;
        if(flagAct1Vez){
          estados = PortonAbriendo;
          portonActAbrLoc = true;
          medirTimePot = true;
          infPorAbCe = false;
          userID = cuentaUser.account[0];
          flagAct1Vez = false;
          #if DEBUG_MODE > 0
            Serial.println("Ejecuta ActManual en PortonClouse");
          #endif
        }
      }else if(noPortonClouse){
        if(flagScanSensor){
          flagScanSensor = false;
          estados = PortonAbriendo;
          medirTimePot = true;
          portonActLoc = true;          //Envia Msj: Se Activo el Porton de manera Local
          infPorAbCe = false;
          #if DEBUG_MODE > 0
          Serial.println("Ejecuta noPortonClouse en PortonClouse");
          #endif
        }
      }else if(isPortonOpen){
        estados = PortonOpen;
      }
      // if(isActManualPor){
        
      // }
      countFSM = 0;
      timePorton = 0;
      noActPorton;
      break;
    case (PortonA_Act):
      siActPorton;
      if(countFSM > 4){
        estados = PortonAbriendo;
      }
      break;
    case (PortonAbriendo):
      noActPorton;
      if(!isActManualPor){
        flagAct1Vez = true;
        //flagActRF = false;
      }
      if(countFSM > 10){
        if(flagScanSensor){
          //flagScanSensor = false;
          if(isPortonClouse){
            estados = PortonClouse;
            flagScanSensor = false;
          }
          else if(isPortonOpen){
            estados = PortonOpen;
            flagScanSensor = false;
          }
        }
        
        if(isActManualPor)
        //if(flagActRF)
        {
          //flagActRF = false;
          if(flagAct1Vez){
            if(flagScanSensor){
              flagScanSensor = false;
              flagAct1Vez = false;
              #if DEBUG_MODE > 0
              Serial.println("PortonAbriendo + isActManualPor");
              #endif
              //portonActConLoc = true;
              portonActConLoc = true;
              medirTimePot = false;
              infPorAbCe = true;
              //userID = cuentaUser.account[0];
              countFSM = 0;
            }
          }
        }
        if(ActPorton){
          #if DEBUG_MODE > 0
          Serial.println("PortonAbriendo + ActPorton");
          #endif
          ActPorton = false;
          medirTimePot = false;
          infPorAbCe = true;
          if(countFSM > 10){
            estados = PortonA_Act;
            countFSM = 0;
          }
        }
      }
      if(timePorton > 253){
        medirTimePot = false;
      }
      break;
    case (PortonOpen):
      if(!isActManualPor){
        flagAct1Vez = true;
      }
      if(medirTimePot){
        medirTimePot = false;
        timeOpenClousPot = timePorton;
        TxAdminTimeOpen = true;
        #if DEBUG_MODE > 0
        Serial.print("Time Open: ");
        Serial.println(timeOpenClousPot);
        #endif
      }
      if(infPorAbCe){
        infPorAbCe = false;
        TxAdminOpCl = true;       //Envia Msj: Porton *Cerrado* completamente
      }
      if(ActPorton){
        ActPorton = false;
        estados = PortonC_Act;
        medirTimePot = true;
      }
      else if(isActManualPor)
      //else if(flagActRF)
      {
        //flagActRF = false;
        if(flagAct1Vez){
          if(flagScanSensor){
            flagScanSensor = false;
            flagAct1Vez = false;
            estados = PortonCerrando;
            portonActCerLoc = true;
            medirTimePot = true;
            userID = cuentaUser.account[0];
            #if DEBUG_MODE > 0
            Serial.println("Ejecuta ActManual en PortonOpen");
            #endif
          }
        }
      }else if(isPortonClouse){
        estados = PortonClouse;

      }else if(noPortonOpen && !isActManualPor){
        if(flagScanSensor){
          flagScanSensor = false;
          portonActLoc = true;            //Envia Msj: Se Activo el Porton de manera Local
          estados = PortonCerrando;
          medirTimePot = true;
          infPorAbCe = false;
          #if DEBUG_MODE > 0
          Serial.println("Ejecuta noPortonOpen en PortonOpen");
          #endif
        }
      }
      // if(isActManualPor){
        
      // }
      noActPorton;
      countFSM = 0;
      timePorton = 0;
      break;
    case (PortonC_Act):
      if(countFSM > 4){
        estados = PortonCerrando;
      }
      siActPorton;
      break;
    case (PortonCerrando):
      if(!isActManualPor){
        flagAct1Vez = true;
        //flagActRF = false;
      }
      if(countFSM > 10){
        if(flagScanSensor){
          if(isPortonClouse){
            estados = PortonClouse;
            flagScanSensor = false;
          }
          else if(isPortonOpen){
            estados = PortonOpen;
            flagScanSensor = false;
          }
        }
        if(isActManualPor)
        //if(flagActRF)
        {
          //flagActRF = false;
          if(flagAct1Vez){
            if(flagScanSensor){
              flagScanSensor = false;
              flagAct1Vez = false;
              #if DEBUG_MODE > 0
              Serial.println("PortonCerrando + isActManualPor");
              #endif
              //portonActLoc = true;
              portonActConLoc = true;
              medirTimePot = false;
              infPorAbCe = true;
              //userID = cuentaUser.account[0];
              countFSM = 0;
            }
          }
        }
        if(ActPorton){
          #if DEBUG_MODE > 0
          Serial.println("PortonCerrando + ActPorton");
          #endif
          ActPorton = false;
          medirTimePot = false;
          infPorAbCe = true;
          estados = PortonA_Act;
          countFSM = 0;
        }
      }
      if(timePorton > 253){
        medirTimePot = false;
      }
      noActPorton;
      break;
    default:
      estados = PortonAbriendo;
      noActPorton;
      break;
  }
  if(timePorton == 0){
    portENTER_CRITICAL(&timerMux);
    countTime = 0;
    portEXIT_CRITICAL(&timerMux);
  }
  
}

String credenciales(int32_t credenID){
  readAllUserName();
  readAllUserBlock();
  String jsonString;
  StaticJsonDocument<2048> root;
  root.clear();
  DynamicJsonDocument doc(1500); 
  doc.clear();
  //JsonObject s1 = doc.createNestedObject();
  
  uint8_t i;
  for (i = 0; i < USER_ALL; i++){
    if(cuentaUser.account[i] == credenID){
      break;
    }
  }
  doc["regID"] = i;
  doc["userId"] = cuentaUser.account[i];
  doc["userName"] = cuentaUserName.account[i];
  doc["userBlock"] = cuentaUserBlock.account[i];
  root["code"] = 0;
  root["data"] = doc;
  serializeJson(root, jsonString);
  return (jsonString);
}

String credencialesSinAdmin(){
  //readAllUserName();
  //readAllUserBlock();
  String jsonString;
  StaticJsonDocument<2048> root;
  root.clear();
  DynamicJsonDocument doc(1500); 
  doc.clear();
  //JsonObject s1 = doc.createNestedObject();
  
  doc["regID"] = 0;
  doc["userId"] = 0;
  doc["userName"] = "NoAdmin";
  doc["userBlock"] = false;
  root["code"] = 0;
  root["data"] = doc;
  serializeJson(root, jsonString);
  return (jsonString);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  #if DEBUG_MODE > 0
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
  #endif
}

void readDS1820(void){
  #if DEBUG_MODE > 0
  Serial.println("\nRequesting temperatures...");
  #endif
  //sensor_temp.setWaitForConversion(true);
  //sensor_temp.setCheckForConversion(true);
  //sensor_temp.requestTemperatures();
  sensor_temp.requestTemperaturesByAddress(insideThermometer);
  #if DEBUG_MODE > 0
  Serial.println("DONE");
  #endif
  //float tempC = sensor_temp.getTempC(insideThermometer);
  temperatureC = sensor_temp.getTempC(insideThermometer);
  //initISR_Timer();
  if(temperatureC == DEVICE_DISCONNECTED_C) 
  {
    #if DEBUG_MODE > 0
    Serial.println("Error: Could not read temperature data");
    #endif
    return;
  }
  #if DEBUG_MODE > 0
  Serial.print("Temp C: ");
  Serial.println(temperatureC);
  #endif
}