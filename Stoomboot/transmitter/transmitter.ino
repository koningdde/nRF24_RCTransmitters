
/* Zender Basis Staion voor 1 Unit
   Ontwikkeld en geschreven door:
   Danny de Koning en Ruud van Galen
   *** Alle rechten uitdrukkelijk voorbehouden ***
   Copyright (C) Februari 2015.
   
 - Versie 11 november 2016. 1 Boot zender.
 - Setup voor atmega 2560   

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "printf.h"
LiquidCrystal_I2C lcd(0x27, 16,4);  // Set the LCD I2C address

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN  53

//Define inputs - JOYSTICKS
#define Joy1X A0
#define Joy1Y A1
#define Joy2X A2
#define Joy2Y A3
#define Joy3X A4 //not used
#define Joy3Y A5 //not used

// Define BUTTONS
int BUT0 = 2;
int BUT1 = 3;
int BUT2 = 4;
int BUT3 = 5;
int BUT4 = 6;

int SW1 = 40;
int SW2 = 41;
int SW3 = 42;
int SW4 = 43;
int SW5 = 44;
int SW6 = 45;
int SW7 = 46;
int SW8 = 47;

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xC8E8F0F0E1LL; // Define the transmit pipe
const uint64_t pipe2 = 0xC8E8F0F0E0LL; // Define the receive pipe

// Define user
char user[] = "Henk Valkhof";
char userpipe[] = "pipe=0xC8E8F0F0E1LL";
char userboat[] = "S.S. DOCKYARD VIJF";

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int BOAT1[13];   //  element array holding joystick readings
int myData[6];   //  element array holding telemetry data
boolean start1 = false;
unsigned long started_waiting_at;
unsigned long timeNow;
boolean timeout;

//Define telemetry items
int ketelLow;
int ketelHigh;
int voedingLow;
int pressure;
int actSpeed;
int water;
int SWITCHES;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  pinMode(40, INPUT_PULLUP);
  pinMode(41, INPUT_PULLUP);
  pinMode(42, INPUT_PULLUP);
  pinMode(43, INPUT_PULLUP);
  pinMode(44, INPUT_PULLUP);
  pinMode(45, INPUT_PULLUP);
  pinMode(46, INPUT_PULLUP);
  pinMode(47, INPUT_PULLUP);
  
  printf_begin();
  Serial.begin(57600);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.openReadingPipe(1, pipe2);
  lcd.init();
  Serial.println("Zender gestart");
  Serial.println(user);
  Serial.println(userpipe);
  Serial.println(userboat);

  radio.printDetails();
  
  lcd.backlight();
  lcd.setCursor(1,0); //1 = karakter 0 = regel 1
  lcd.print(userboat);
  lcd.setCursor(4,1);
  lcd.print(user);
  lcd.setCursor(4,2);
  lcd.print("Gebouwd door:");
  lcd.setCursor(4,3);
  lcd.print("MBV Voorne");
  delay(2500);
  lcd.clear();
  lcd.noBacklight();
  delay(500);
  lcd.backlight();
  lcd.setCursor(3,0); //1 = karakter 0 = regel 1
  lcd.print("Zender gestart");
  lcd.setCursor(3,1);
  lcd.print("Schakel");
  lcd.setCursor(3,2);
  lcd.print("ontvanger in");
  delay(500);
  
  }//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
  {  

  int SWITCHES = switchread(); //Lees stand extra knoppen en maak BCD

  radio.stopListening(); //First try to send
  delay(10);
  
  /*
  
  if (analogRead(Joy1X) == 500) {start1 = true;}   
    if (start1 == true) {BOAT1[0] = analogRead(Joy1X);} //Read motor setpoints and store in array
 
  else
  {
    BOAT1[0] = 500; //Prevent motor from running while startup
  }
*/

  BOAT1[0] = analogRead(Joy1X);
  BOAT1[1] = analogRead(Joy1Y); 
  BOAT1[2] = analogRead(Joy2X);
  BOAT1[3] = analogRead(Joy2Y);
  BOAT1[4] = analogRead(Joy3X);
  BOAT1[5] = analogRead(Joy3Y); 
  BOAT1[6] = analogRead(Joy3Y); 
  BOAT1[7] = digitalRead(BUT0);
  BOAT1[8] = digitalRead(BUT1);
  BOAT1[9] = digitalRead(BUT2);
  BOAT1[10] = digitalRead(BUT3);
  BOAT1[11] = digitalRead(BUT4);
  BOAT1[12] = SWITCHES;
  radio.write( BOAT1, sizeof(BOAT1) );
  
  //Over naar ontvangen telemetrie 
  radio.startListening();  
  started_waiting_at = micros(); // timeout period, get the current microseconds
  timeout = false;
  
  while ( ! radio.available() ) {                       // While nothing is received
    if (micros() - started_waiting_at > 2000000 ) {     // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }

  if ( timeout ) // Describe the results timeout
  {
  lcd.clear();   
  lcd.setCursor(2,3);
  lcd.print("Verbindingsfout");
  }
  else
  {
    // Grab the telemetry
    radio.read( myData, sizeof(myData) );
      ketelLow = (myData[0]);
      ketelHigh = (myData[1]);
      voedingLow = (myData[2]);
      pressure = (myData[3]);
      actSpeed = (myData[4]);
      water = (myData[5]);
    timeNow = micros();
    lcd.clear();
    lcd.setCursor(0,3);  
    lcd.print(userboat);
  }
  
  lcd.setCursor(0,0); 
  lcd.print("Low  :");
  lcd.print(ketelLow);
  lcd.print(" High :");
  lcd.print(ketelHigh);

  lcd.setCursor(0,1);
  lcd.print("Voed :");
  lcd.print(voedingLow);
  lcd.print(" Druk :");
  lcd.print(pressure);
  lcd.print(" m");
  
  lcd.setCursor(0,2);
  lcd.print("Speed:");
  lcd.print(actSpeed);
  lcd.print(" Water:");
  lcd.print(water);
 
}//--(end main loop )---


int switchread(){
  int a,b,c,d,e,f,g,h,BCD;
a = digitalRead(SW1);
b = digitalRead(SW2);
c = digitalRead(SW3);
d = digitalRead(SW4);
e = digitalRead(SW5);
f = digitalRead(SW6);
g = digitalRead(SW7);
h = digitalRead(SW8);

if (a == 1) {BCD = BCD + 1;};
if (b == 1) {BCD = BCD + 2;};
if (c == 1) {BCD = BCD + 4;};
if (d == 1) {BCD = BCD + 8;};
if (e == 1) {BCD = BCD + 16;};
if (f == 1) {BCD = BCD + 32;};
if (g == 1) {BCD = BCD + 64;};
if (h == 1) {BCD = BCD + 128;};
return BCD;
}

//*********( THE END )***********


