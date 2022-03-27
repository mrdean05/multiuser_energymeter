#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ctype.h>
#include <string.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include "ACS712.h"
#include <EEPROM.h>

//Users

float users;
float users1;
float users2;
float price;

char data;

//Relays
const unsigned int relay = 4;
const unsigned int relay1 = 8;
const unsigned int relay2 = 6;

int alert1 = 1;
int alert2 = 1;
int alert3 = 1;

ACS712  ACS_1(A1, 5.0, 1023, 185);
ACS712  ACS_2(A2, 5.0, 1023, 185);
ACS712  ACS_3(A3, 5.0, 1023, 185);

float voltage = 230;
float current_A;
unsigned long startTimer = 0;
unsigned long startTimer1 = 0;
unsigned long startTimer2 = 0;

unsigned long startSTimer = 0;
unsigned long currentTimer;
unsigned long currentTimer1;
unsigned long currentTimer2;
unsigned long switchTimer;

float power;
float energy;
float energy1;
float energy2;

int number = 1;


// Select your modem:
#define DUMP_AT_COMMANDS
#define TINY_GSM_MODEM_SIM800

// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET "0558073309"
#define READ_SMS

unsigned long stimer = 0;
unsigned long ctimer;

// Range to attempt to autobaud
// NOTE:  DO NOT AUTOBAUD in production code.  Once you've established
// communication, set a fixed baud rate using modem.setBaud(#).
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 9600

HardwareSerial &SerialMon = Serial;
#define TINY_GSM_DEBUG SerialMon
#include <TinyGSM.h>

SoftwareSerial SerialAT(2, 3);


#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif


void writeIntIntoEEPROM(int address, int number)
{
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void setup() {
  // put your setup code here, to run once:
  // Set debug baud rate

  //initializing and display data on the screen
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("MULTI-USER METER");
  lcd.setCursor(3, 1);
  lcd.print("LOADING");
  for (int i = 0; i <= 4; i++) {

    lcd.print(".");
    delay (1000);
  }

  SerialMon.begin(9600);
  while (!SerialMon);
  SerialAT.listen();

  // Set GSM module baud rate
  if (TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX) > 0) {
    modem.restart();
    delay(3000);

    String modemInfo = modem.getModemInfo();
    DBG("\r\nModem: ", modemInfo);

    DBG("Waiting for network...");
    lcd.setCursor(3, 1);
    lcd.print("Reconnecting");
    if (!modem.waitForNetwork(10000L)) {
      DBG("GSM Network Error");
    }
  }

  SerialMon.println("Complete");

  lcd.clear();
  lcd.setCursor(3, 2);
  lcd.print("NETWORK CONNECTED");

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  //initialize relay pins
  pinMode (relay, OUTPUT);
  pinMode (relay1, OUTPUT);
  pinMode (relay2, OUTPUT);


  //USER 1

  //reading user 1 data from eeprom
  int number = readIntFromEEPROM(1);

  float o = number / 100.00;
  float price1 = o;
  float data = 1.5 * price1;
  users = data;



  //USER 2
  
  //reading user 2 data from eeprom
  number = readIntFromEEPROM(10);
  o =  number / 100.00;
  float price2 = o;
  data = 1.5 * price2;
  users1 = data;


  //USER 3
 //reading user 3 data from eeprom
 
  number = readIntFromEEPROM(20);

  o =  number / 100.00;
  float price3 = o;
  data = 1.5 * price3;
  users2 = data;

  lcd.setCursor(3, 3);
  lcd.print("CREDIT LOADED");
  delay (3000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("MULTI-USER METER");
  lcd.setCursor(1, 1);
  lcd.print("USER");
  lcd.setCursor(2, 2);
  lcd.print("KWH");
  lcd.setCursor(0, 3);
  lcd.print("CEDIS");

}

bool res = false;
unsigned long countdown = 0;

void loop() {
  // put your main code here, to run repeatedly:

  int *idxbuf;
  SMSmessage msg;
  String sms;

  String sendmsg;

  //  res = modem.sendSMS(SMS_TARGET, sendmsg);
  //  DBG("SMS:", res ? "OK" : "fail");
  countdown = millis();

  for (unsigned long start = millis(); millis() - start < 5000;) {
    int msgcnt = modem.getUnreadSMSLocs(idxbuf);
    for (int j = 0; j < msgcnt; j++) {
      msg = modem.readSMS(msgcount[j]);
      DBG(msg.message);
    }

    data = msg.message.charAt(0);

  }

  lcd.setCursor(17, 1);
  lcd.print(data);


  if (data == '1') {

    String data2 = msg.message.substring(1);

    int data3 = data2.toInt();
    Serial.println (data3);

    float energy = 1.5 * data3;
    Serial.println (energy);
    users = users + energy;
    price = users / 1.5;

  }

  else if (data == '2') {

    String data2 = msg.message.substring(1);

    int data3 = data2.toInt();
    Serial.println (data3);

    float energy = 1.5 * data3;
    Serial.println (energy);
    users1 = users1 + energy;
    price = users1 / 1.5;

  }

  else if (data == '3') {

    String data2 = msg.message.substring(1);

    int data3 = data2.toInt();
    Serial.println (data3);

    float energy = 1.5 * data3;
    Serial.println (energy);

    users2 = users2 + energy;
    price = users2 / 1.5;

  }

  if (users <= 0.00000) {

    digitalWrite(relay, LOW);
    users = 0;
  }

  else {
    digitalWrite(relay, HIGH);
    float mA = ACS_1.mA_AC();
    float current_A = mA;


    currentTimer = millis();

    if (currentTimer - startTimer >= 500) {

      float timerOne =   currentTimer - startTimer;
      float timeInHour = timerOne / 3600000;

      startTimer = currentTimer;
      energy = voltage * current_A * timeInHour;
      energy = energy / 1000;


      users = users - energy;
    }
  }

  if (users1 <= 0.0000) {

    digitalWrite(relay1, LOW);
    users1 = 0;
 }
  else {

    digitalWrite(relay1, HIGH);

    float mA = ACS_2.mA_AC();
    float current_A = mA;
    Serial.println("Im on");

    currentTimer1 = millis();

    if (currentTimer1 - startTimer1 >= 500) {

      float timerOne =   currentTimer1 - startTimer1;
      float timeInHour = timerOne / 3600000;

      startTimer1 = currentTimer1;
      energy1 = voltage * current_A * timeInHour;
      energy1 = energy1 / 1000;


      users1 = users1 - energy1;
    }
  }

  if (users2 <= 0.0000) {

    digitalWrite(relay2, LOW);
    users2 = 0;
  }
  else {

    digitalWrite(relay2, HIGH);

    float mA = ACS_3.mA_AC();
    float current_A = mA;

    currentTimer2 = millis();

    if (currentTimer2 - startTimer2 >= 500) {

      float timerOne =   currentTimer2 - startTimer2;
      float timeInHour = timerOne / 3600000;

      startTimer2 = currentTimer2;
      energy2 = voltage * current_A * timeInHour;
      energy2 = energy2 / 1000;
      users2 = users2 - energy2;
    }
  }


  switchTimer = millis();

  if (switchTimer - startSTimer >= 2000) {

    startSTimer = switchTimer;


    switch (number) {

      case 1:

        details(1, users);
        number = 2;
        break;

      case 2:

        details(2, users1);

        number = 3;
        break;

      case 3:

        details(3, users2);

        number = 1;
        break;

      default:
        break;
    }
  }
}

void details( int userNumber, float calEnergy) {

  lcd.clear();
  lcd.print(" MULTI-USER METER");
  lcd.setCursor(1, 1);
  lcd.print("USER");
  lcd.setCursor(2, 2);
  lcd.print("KWH");
  lcd.setCursor(0, 3);
  lcd.print("CEDIS");

  lcd.setCursor(10, 1);
  lcd.print(userNumber);

  lcd.setCursor(8, 2);
  lcd.print(calEnergy, 3);

  price = calEnergy / 1.5;

  float storage = price;
  lcd.setCursor(8, 3);
  lcd.print(price, 2);


  store (userNumber, storage);

}


void store(int usernumber, float storage) {

  // writing data into the eeprom memory
  
  storage = storage * 100;
  storage = round(storage);
  int storeX = storage;

  if (usernumber == 1) {
    writeIntIntoEEPROM(1, storeX);
  }

  else if (usernumber == 2) {

    writeIntIntoEEPROM(10, storeX);

  }

  else if (usernumber == 3) {
    writeIntIntoEEPROM(20, storeX);
  }

}
