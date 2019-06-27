// Broadlink RM 3 mini + Lolin D1 mini v3.0 + Fujitsu ASYG7LMCA + Blynk
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266_SSL.h>

//Defines
#define TIMEOUT 5000  //5 sec
#define ONTEMP  24    //24 °C

//Global variables
const unsigned int kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
const char* auth          = "5275352dc1fa4a78b15b81604290bf9c";
const char* ssid          = "DarpAsusNet_2.4";
const char* password      = "andrew243";

unsigned int powerValue   = 0;
unsigned int swingValue   = 2;
unsigned int modeValue    = 1;
unsigned int fanValue     = 1;
unsigned int spoilerMove  = 0;
unsigned int ecoMode      = 0;
unsigned int powerfulMode = 0;
byte tempValue = FUJITSU_AC_MIN_TEMP;

IRFujitsuAC ac(kIrLed);

BLYNK_WRITE(V0)
{
  powerValue = param.asInt();
  // process received value
    switch (powerValue)
  {
    case 0:
    ac.setCmd(kFujitsuAcCmdTurnOff);
    swingValue = 2;
    ac.setSwing(kFujitsuAcSwingOff);
    Blynk.virtualWrite(V1, swingValue);
    modeValue = 1;
    ac.setMode(kFujitsuAcModeAuto);
    Blynk.virtualWrite(V2, modeValue);
    fanValue = 1;
    ac.setFanSpeed(kFujitsuAcFanAuto);
    Blynk.virtualWrite(V3, fanValue);
    tempValue = FUJITSU_AC_MIN_TEMP;
    ac.setTemp(tempValue);
    Blynk.virtualWrite(V4, tempValue);
    ecoMode = 0;
    Blynk.virtualWrite(V6, ecoMode);
    powerfulMode = 0;
    Blynk.virtualWrite(V7, powerfulMode);
    ac.send();
    break;
    
    case 1:
    ac.setCmd(kFujitsuAcCmdTurnOn);
    swingValue = 1;
    ac.setSwing(kFujitsuAcSwingVert);
    Blynk.virtualWrite(V1, swingValue);
    modeValue = 2;
    ac.setMode(kFujitsuAcModeCool);
    Blynk.virtualWrite(V2, modeValue);
    fanValue = 1;
    ac.setFanSpeed(kFujitsuAcFanAuto);
    Blynk.virtualWrite(V3, fanValue);
    tempValue = ONTEMP;
    ac.setTemp(tempValue);
    Blynk.virtualWrite(V4, tempValue);
    ecoMode = 0;
    Blynk.virtualWrite(V6, ecoMode);
    powerfulMode = 0;
    Blynk.virtualWrite(V7, powerfulMode);
    ac.send();
    break;
    
    default:
    break;
  }
}

BLYNK_WRITE(V1)
{
  swingValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // process received value
  switch (swingValue)
  {
    case 1:
    ac.setSwing(kFujitsuAcSwingVert);
    break;
    case 2:
    ac.setSwing(kFujitsuAcSwingOff);
    break;
    default:
    break;
  }
  if (powerValue == 1)
  {
    ac.setCmd(kFujitsuAcCmdStayOn);
    ac.send();    
  }
}

BLYNK_WRITE(V2)
{
  modeValue = param.asInt(); // assigning incoming value from pin V2 to a variable
  // process received value
  switch (modeValue)
  {
    case 1:
    ac.setMode(kFujitsuAcModeAuto);
    break;
    case 2:
    ac.setMode(kFujitsuAcModeCool);
    break;
    case 3:
    ac.setMode(kFujitsuAcModeDry);
    break;
    case 4:
    ac.setMode(kFujitsuAcModeFan);
    break;
    case 5:
    ac.setMode(kFujitsuAcModeHeat);
    break;
    default:
    break;
  }
  ecoMode = 0;
  powerfulMode = 0;
  Blynk.virtualWrite(V7, powerfulMode);
  Blynk.virtualWrite(V6, ecoMode);
  if (powerValue == 1)
  {
    ac.setCmd(kFujitsuAcCmdStayOn);
    ac.send();    
  }
}

BLYNK_WRITE(V3)
{
  fanValue = param.asInt(); // assigning incoming value from pin V3 to a variable
  // process received value
  switch (fanValue)
  {
    case 1:
    ac.setFanSpeed(kFujitsuAcFanAuto);
    break;
    case 2:
    ac.setFanSpeed(kFujitsuAcFanHigh);
    break;
    case 3:
    ac.setFanSpeed(kFujitsuAcFanMed);
    break;
    case 4:
    ac.setFanSpeed(kFujitsuAcFanLow);
    break;
    case 5:
    ac.setFanSpeed(kFujitsuAcFanQuiet);
    break;
    default:
    break;
  }
  ecoMode = 0;
  powerfulMode = 0;
  Blynk.virtualWrite(V7, powerfulMode);
  Blynk.virtualWrite(V6, ecoMode);
  if (powerValue == 1)
  {
    ac.setCmd(kFujitsuAcCmdStayOn);
    ac.send();    
  }
}

BLYNK_WRITE(V4)
{
  tempValue = byte(param.asInt()); // assigning incoming value from pin V4 to a variable
  // process received value
  ac.setTemp(tempValue);
  ecoMode = 0;
  powerfulMode = 0;
  Blynk.virtualWrite(V7, powerfulMode);
  Blynk.virtualWrite(V6, ecoMode);
  if (powerValue == 1)
  {
    ac.setCmd(kFujitsuAcCmdStayOn);
    ac.send();    
  }
}

BLYNK_WRITE(V5)
{
  spoilerMove = param.asInt(); // assigning incoming value from pin V4 to a variable
  // process received value
  if (powerValue == 1)
  {
    ac.setSwing(kFujitsuAcSwingOff);
    ac.setCmd(kFujitsuAcCmdStepVert);
    ac.send();
  }
}

BLYNK_WRITE(V6)
{
  ecoMode = param.asInt(); // assigning incoming value from pin V4 to a variable
  // process received value
  powerfulMode = 0;
  Blynk.virtualWrite(V7, powerfulMode);
  if (powerValue == 1)
  {
    ac.setCmd(kFujitsuAcCmdEcono);
    ac.send();
  }
}

BLYNK_WRITE(V7)
{
  powerfulMode = param.asInt(); // assigning incoming value from pin V4 to a variable
  // process received value
  ecoMode = 0;
  Blynk.virtualWrite(V6, ecoMode);
  if (powerValue == 1)
  {
    ac.setCmd(kFujitsuAcCmdPowerful);
    ac.send();
  }
}


void setup() {
  unsigned long timeouts;
  
  ac.begin();
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  // Wait for connection
  timeouts = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (millis()-timeouts > TIMEOUT)
    {
      break;
    }
  }
  Blynk.config(auth);
  Blynk.connect();
  ac.setModel(ARREB1E);
  ac.setCmd(kFujitsuAcCmdTurnOff);
  ac.setSwing(kFujitsuAcSwingOff);
  ac.setMode(kFujitsuAcModeAuto);
  ac.setFanSpeed(kFujitsuAcFanAuto);
  ac.setTemp(kFujitsuAcMinTemp);
  
  Blynk.virtualWrite(V0, powerValue);
  Blynk.virtualWrite(V1, swingValue);
  Blynk.virtualWrite(V2, modeValue);
  Blynk.virtualWrite(V3, fanValue);
  Blynk.virtualWrite(V4, tempValue);
}

void loop() {
  if (Blynk.connected())
  {
    Blynk.run();
  }
  else
  {
    Blynk.disconnect();
    delay(1000);
    Blynk.connect();
  }
}
