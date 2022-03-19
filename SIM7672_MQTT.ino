/*
  GCP (Google Cloud Platform) IoT Core GSM

  This sketch securely connects to GCP IoT Core using MQTT over GSM/3G.
  It uses a private key stored in the ATECC508A and a JSON Web Token (JWT) with
  a JSON Web Signature (JWS).

  It publishes a message every 5 seconds to "/devices/{deviceId}/state" topic
  and subscribes to messages on the "/devices/{deviceId}/config" and
  "/devices/{deviceId}/commands/#" topics.

  The circuit:
  - MKR GSM 1400 board
  - Antenna
  - SIM card with a data plan
  - LiPo battery

  This example code is in the public domain.
*/

#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include <Arduino_JSON.h>

#define TINY_GSM_MODEM_SIM7600

#include <TinyGsmClient.h>

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include "ciotc_config.h" // Update this file with your configuration


// GSM settings
#define SECRET_PINNUMBER     ""
#define SECRET_GPRS_APN      "GPRS_APN" // replace with your GPRS APN
#define SECRET_GPRS_LOGIN    "login"    // replace with your GPRS login
#define SECRET_GPRS_PASSWORD "password" // replace with your GPRS password

// Fill in your Google Cloud Platform - IoT Core info
#define SECRET_PROJECT_ID   ""
#define SECRET_CLOUD_REGION ""
#define SECRET_REGISTRY_ID  ""
#define SECRET_DEVICE_ID    ""

/////// Enter your sensitive data in arduino_secrets.h
const char pinnumber[]     = SECRET_PINNUMBER;
const char gprs_apn[]      = SECRET_GPRS_APN;
const char gprs_login[]    = SECRET_GPRS_LOGIN;
const char gprs_password[] = SECRET_GPRS_PASSWORD;

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
#define SerialAT Serial2

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 1650
#endif

#define TINY_GSM_USE_GPRS true

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

#define CLOUD_TIMER 10000
#define FANOUT 2
#define FANINP 15
#define OFF 0
#define ON 1
#define RED 14
#define GREEN 27
#define BLUE 26
#define WHITE 100

char timestamp[30];
char formattedDate4G[21];

long unixepoch = 0;

#define DEBUG_OUT 1

const char projectId[]     = SECRET_PROJECT_ID;
const char cloudRegion[]   = SECRET_CLOUD_REGION;
const char registryId[]    = SECRET_REGISTRY_ID;
const String deviceId      = SECRET_DEVICE_ID;

const char broker[]        = "mqtt.googleapis.com";

MqttClient    mqttClient(client);
CloudIoTCoreDevice *device;

const char apn[]  = "WWW";
const char gprsUser[] = "";
const char gprsPass[] = "";

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Calculate and set the client id used for MQTT
  String clientId = calculateClientId();

  mqttClient.setId(clientId);

  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);
}

void loop() {
 /* if (gsmAccess.status() != GSM_READY || gprs.status() != GPRS_READY) {
    connectGSM();
  }*/

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

  // publish a message roughly every 5 seconds.
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();

    publishMessage();
  }
}

#if 0
unsigned long getTime() {
  // get the current time from the cellular module
  return gsmAccess.getTime();
}
#endif

#if 0
void connectGSM() {
  Serial.println("Attempting to connect to the cellular network");

  while ((gsmAccess.begin(pinnumber) != GSM_READY) ||
         (gprs.attachGPRS(gprs_apn, gprs_login, gprs_password) != GPRS_READY)) {
    // failed, retry
    Serial.print(".");
    delay(1000);
  }

  Serial.println("You're connected to the cellular network");
  Serial.println();
}
#endif

void connectGSM(){

    if(DEBUG_OUT) Serial.println(F("[SIM7672] Starting GSM Setup"));
    TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
    delay(200);
    if(DEBUG_OUT) Serial.println(F("[SIM7672] Initializing modem..."));
    modem.restart();

    String modemInfo = modem.getModemInfo();
    if(DEBUG_OUT) Serial.print(F("[SIM7672] Modem Info: "));
    Serial.println(modemInfo);

    #if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
    // The XBee must run the gprsConnect function BEFORE waiting for network!
    modem.gprsConnect(apn, gprsUser, gprsPass);
    #endif

    if(DEBUG_OUT) SerialMon.println(F("[SIM7672] Waiting for network..."));
    if (!modem.waitForNetwork()) {
        if(DEBUG_OUT) SerialMon.println(F("[SIM7672] fail"));
        delay(200);
        return;
  }
  if(DEBUG_OUT) SerialMon.println(F("[SIM7672] success"));

  if (modem.isNetworkConnected()) {
    if(DEBUG_OUT) SerialMon.println(F("[SIM7672] Network connected"));
  }

#if TINY_GSM_USE_GPRS
  // GPRS connection parameters are usually set after network registration
    if(DEBUG_OUT) SerialMon.print(F("[SIM7672] Connecting to "));
    SerialMon.print(apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
      if(DEBUG_OUT) SerialMon.println(F("[SIM7672] fail"));
      delay(200);
      return;
    }
    if(DEBUG_OUT) SerialMon.println(F("[SIM7672] success"));

  if (modem.isGprsConnected()) {
    if(DEBUG_OUT) SerialMon.println(F("[SIM7672]GPRS connected"));
  }
 
    int csq = modem.getSignalQuality();
    Serial.println("Signal quality: "+String(csq));
    delay(1);

#endif


}

void connectMQTT() {
  Serial.print("Attempting to connect to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connected()) {
    // Calculate the JWT and assign it as the password
    String jwt = getJwt4G();
    mqttClient.setUsernamePassword("", jwt);

    if (!mqttClient.connect(broker, 8883)) {
      // failed, retry
      Serial.print(".");
      delay(5000);
    }
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to topics
  mqttClient.subscribe("/devices/" + deviceId + "/config", 1);
  mqttClient.subscribe("/devices/" + deviceId + "/commands/#");
}

String calculateClientId() {
  String clientId;

  // Format:
  //
  //   projects/{project-id}/locations/{cloud-region}/registries/{registry-id}/devices/{device-id}
  //

  clientId += "projects/";
  clientId += projectId;
  clientId += "/locations/";
  clientId += cloudRegion;
  clientId += "/registries/";
  clientId += registryId;
  clientId += "/devices/";
  clientId += deviceId;

  return clientId;
}

String getJwt4G(){
  //iat = unixepoch;
  getiat();
  Serial.print("Unix is:");
  Serial.println(unixepoch);
  Serial.println("Refreshing JWT");
  String jwt = device->createJWT(unixepoch, jwt_exp_secs);
  Serial.println(jwt);
  return jwt;
//return "sillyfellows";
}

void getiat()
{
  SerialAT.println("AT+CCLK?");
  timeResp(timestamp, 6000);
  SerialMon.print(F("Timestamp : "));
  SerialMon.println(timestamp);
  for (int i = 0; i < strlen(timestamp); i++)
  {
    SerialMon.print(i);
    SerialMon.print(":");
    SerialMon.println(timestamp[i]);
  }
  timeEpoch();

}

void timeEpoch()
{
  struct tm t;
  time_t t_of_day;

  String y = String(timestamp[29]) + String(timestamp[30]);
  t.tm_year = 2000 - 1900 + y.toInt();
  String mo = String(timestamp[32]) + String(timestamp[33]);
  t.tm_mon = mo.toInt() - 1;         // Month, where 0 = jan
  String da = String(timestamp[35]) + String(timestamp[36]);
  t.tm_mday = da.toInt();         // Day of the month
  String hr = String(timestamp[38]) + String(timestamp[39]);
  t.tm_hour = hr.toInt();
  String mi = String(timestamp[41]) + String(timestamp[42]);
  t.tm_min = mi.toInt();
  String se = String(timestamp[44]) + String(timestamp[45]);
  t.tm_sec = se.toInt();
  t.tm_isdst = -1;                                                  // Is DST on? 1 = yes, 0 = no, -1 = unknown

  Serial.println(t.tm_year);
  Serial.println(t.tm_mon);
  Serial.println(t.tm_mday);
  Serial.println(t.tm_hour);
  Serial.println(t.tm_min);
  Serial.println(t.tm_sec);
  t_of_day = mktime(&t);

  printf("seconds since the Epoch: %ld\n", (long) t_of_day);

  unixepoch = (long) t_of_day ;
  unixepoch = unixepoch - 39600 ;                                 //India // 19800
  //unixepoch = unixepoch - 7200;                                 //Europe // 3600
  printf("UTC Epoch: %ld\n", unixepoch);

  snprintf_P(formattedDate4G, 21, PSTR("%04u-%02u-%02uT%02u:%02u:%02uZ"), (t.tm_year + 1900), (t.tm_mon + 1), (t.tm_mday), t.tm_hour, t.tm_min, t.tm_sec);


}

void timeResp(char *buffer, unsigned int timeout)
{
  const char *crtResp = "+CCLK: ";                                  // Success
  int len1 = strlen(crtResp);

  int sum1 = 0;
  int i = 0;

  unsigned long timerStart, timerEnd;
  unsigned long startTime = millis();

  while (millis() - startTime < 8000) {
    if (SerialAT.available()) {
      char c = SerialAT.read();
      Serial.print(c);
      buffer[i++] = c;
      sum1 = (c == crtResp[sum1]) ? sum1 + 1 : 0;
      if (sum1 == len1)
      {
        SerialMon.print(F("*********Time captured:"));
      }
    }

  }
  SerialMon.print(buffer);
  SerialMon.println(F("******"));
}

#if 0
String calculateJWT() {
 // unsigned long now = getTime();
  
  // calculate the JWT, based on:
  //   https://cloud.google.com/iot/docs/how-tos/credentials/jwts
  JSONVar jwtHeader;
  JSONVar jwtClaim;
getiat();
  jwtHeader["alg"] = "ES256";
  jwtHeader["typ"] = "JWT";

  jwtClaim["aud"] = projectId;
  jwtClaim["iat"] = unixepoch;
  jwtClaim["exp"] = unixepoch + (24L * 60L * 60L); // expires in 24 hours 

  return ECCX08JWS.sign(0, JSON.stringify(jwtHeader), JSON.stringify(jwtClaim));
}
#endif

void publishMessage() {
  Serial.println("Publishing message");

  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage("/devices/" + deviceId + "/state");
  mqttClient.print("hello ");
  mqttClient.print(millis());
  mqttClient.endMessage();
}

void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();

  Serial.println();
}
