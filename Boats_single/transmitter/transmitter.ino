
/* Zender Basis Staion voor 1 Unit
   Ontwikkeld en geschreven door:
   Danny De Koning en Ruud van Galen
   *** Alle rechten uitdrukkelijk voorbehouden ***
   Copyright (C) Februari 2015.
   
 - Versie 9 juni 2015. 1 Boot zender.
    

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN  53
#define SCK_PIN   52
#define Miso_PIN  50
#define Mosi_PIN  51

//Define inputs - JOYSTICKS
#define Joy0 A0
#define Joy1 A1
#define Joy2 A2
#define Joy3 A3
#define Joy4 A4
#define Joy5 A5

// Define BUTTONS
int BUT0 = 0;
int BUT1 = 1;
int BUT2 = 2;
int BUT3 = 3;
int BUT4 = 4;

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xC8E8F0F0E1LL; // Define the transmit pipe

// Define user
char user[] = "Single zender Ruud";
char userpipe[] = "pipe = 0xC8E8F0F0E1LL";
char userboat[] = "1 Boot zender";

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int BOAT1[11];  //  element array holding Joystick readings
boolean start1 = false;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(57600);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openWritingPipe(pipe);
  Serial.println("Zender gestart");
  Serial.println(user);
  Serial.println(userpipe);
  Serial.println(userboat);
  }//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
  {  
    if (analogRead(Joy0) == 500) {start1 = true;}   
    if (start1 == true) {BOAT1[0] = analogRead(Joy1);} //Read motor setpoints and store in array
 
  else
  {
    BOAT1[0] = 500; //Prevent motor from running while startup
  }
  
  BOAT1[1] = analogRead(Joy1); //Comment out when not using timer
  BOAT1[2] = analogRead(Joy2);
  BOAT1[3] = analogRead(Joy3);
  BOAT1[4] = analogRead(Joy4);
  BOAT1[5] = analogRead(Joy5); 
  BOAT1[6] = digitalRead(BUT0);
  BOAT1[7] = digitalRead(BUT1);
  BOAT1[8] = digitalRead(BUT2);
  BOAT1[9] = digitalRead(BUT3);
  BOAT1[10] = digitalRead(BUT4);
  radio.write( BOAT1, sizeof(BOAT1) );
  
  /*
  Serial.println(BOAT2[0]);
  Serial.println(BOAT1[1]);
  Serial.println(BOAT1[2]);
  Serial.println(BOAT1[3]);
  Serial.println(BOAT1[4]);
  */

}//--(end main loop )---



//*********( THE END )***********


