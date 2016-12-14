
/* Zender Basis Staion voor 5 Units
   Ontwikkeld en geschreven door:
   Danny De Koning en Ruud van Galen
   *** Alle rechten uitdrukkelijk voorbehouden ***
   Copyright (C) Februari 2015.
   
 - V1.10 13/03/2015
 - Versie 9 juni 2015. 5 Boten zender.
    

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   8
#define CSN_PIN  9

//Define inputs - JOYSTICKS
#define KWAK_X A2
#define KWAK_Y A0
#define KWEK_X A3
#define KWEK_Y A1
#define KWIK_X A5
#define KWIK_Y A10
#define KWOK_X A6
#define KWOK_Y A9
#define KWUK_X A7
#define KWUK_Y A8

// Define BUTTONS
int KWAKBUT1 = 40;
int KWAKBUT2 = 22;
int KWEKBUT1 = 24;
int KWEKBUT2 = 26;
int KWIKBUT1 = 28;
int KWIKBUT2 = 30;
int KWOKBUT1 = 32;
int KWOKBUT2 = 34;
int KWUKBUT1 = 36;
int KWUKBUT2 = 38;

int STARTPULS = 10; //START KNOP, BEGIN VAAR TIJD 

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

// Define user
char user[] = "Clubzender Modelbouw Hellevoet";
char userpipe[] = "pipe = 0xE8E8F0F0E1LL";
char userboat[] = "5 Boten zender";

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int BOAT1[5];  //  element array holding Joystick readings
int BOAT2[5];
int BOAT3[5];
int BOAT4[5];
int BOAT5[5];
unsigned long runtime; // Time that boats can run
unsigned long time; //Actual time from bootup to store start time
boolean running = false; // Not able to run motors when == false

boolean start1 = false;
boolean start2 = false;
boolean start3 = false;
boolean start4 = false;
boolean start5 = false;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(57600);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openWritingPipe(pipe);
  pinMode(STARTPULS, INPUT);
  Serial.println("Zender gestart");
  Serial.println(user);
  Serial.println(userpipe);
  Serial.println(userboat);
  }//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{  
  if ((digitalRead(STARTPULS) == HIGH) & (running == false))  //Time starts when button is released
    {
    runtime = (timer() * 60000); //Find runtime (MIN) and make mSec
    time = millis(); //Find actual time
    Serial.print("Vaartijd ingesteld op ");
    Serial.print(timer());
    Serial.println(" minuten");    
  }  
   
  if (millis() >= (time + runtime)) //Find end time, when actual time is greater then runtime + start time
   {
   if (running == true) 
   {
   Serial.println("Vaartijd afgelopen ");
   }   
   running = false;
   start1 = false;
   start2 = false;
   start3 = false;
   start4 = false;
   start5 = false;
   }
  else 
  {
    running = true;
  }
    
  if (running == true)
  {
    if (analogRead(KWAK_X) == 500) {start1 = true;}
    if (analogRead(KWEK_X) == 500) {start2 = true;}
    if (analogRead(KWIK_X) == 500) {start3 = true;}
    if (analogRead(KWOK_X) == 500) {start4 = true;}
    if (analogRead(KWUK_X) == 500) {start5 = true;}
    
   if (start1 == true) {BOAT1[0] = analogRead(KWAK_X);} //Read motor setpoints and store in array
   if (start2 == true) {BOAT2[0] = analogRead(KWEK_X);}
   if (start3 == true) {BOAT3[0] = analogRead(KWIK_X);}
   if (start4 == true) {BOAT4[0] = analogRead(KWOK_X);}
   if (start5 == true) {BOAT5[0] = analogRead(KWUK_X);}
  }
  else
  {
    BOAT1[0] = 500; //Prevent motor from running, time is over.
    BOAT2[0] = 500;
    BOAT3[0] = 500;
    BOAT4[0] = 500;
    BOAT5[0] = 500;
  }
  
  //BOAT1[0] = analogRead(KWAK_X); //Comment out when not using timer
  BOAT1[1] = analogRead(KWAK_Y);
  BOAT1[2] = digitalRead(KWAKBUT1);
  BOAT1[3] = digitalRead(KWAKBUT2);
  BOAT1[4] = 1; //Define boat number
  radio.write( BOAT1, sizeof(BOAT1) );

  //BOAT2[0] = analogRead(KWEK_X); //Comment out when not using timer
  BOAT2[1] = analogRead(KWEK_Y);
  BOAT2[2] = digitalRead(KWEKBUT1);
  BOAT2[3] = digitalRead(KWEKBUT2);
  BOAT2[4] = 2; //Define boat number
  radio.write( BOAT2, sizeof(BOAT2) );
  
  //BOAT3[0] = analogRead(KWOK_X); //Comment out when not using timer
  BOAT3[1] = analogRead(KWIK_Y);
  BOAT3[2] = digitalRead(KWIKBUT1);
  BOAT3[3] = digitalRead(KWIKBUT2);
  BOAT3[4] = 3; //Define boat number
  radio.write( BOAT3, sizeof(BOAT3) );
  
  //BOAT4[0] = analogRead(KWOK_X); //Comment out when not using timer
  BOAT4[1] = analogRead(KWOK_Y);
  BOAT4[2] = digitalRead(KWOKBUT1);
  BOAT4[3] = digitalRead(KWOKBUT2);
  BOAT4[4] = 4; //Define boat number
  radio.write( BOAT4, sizeof(BOAT4) );  
  
  //BOAT5[0] = analogRead(KWUK_X); //Comment out when not using timer
  BOAT5[1] = analogRead(KWUK_Y);
  BOAT5[2] = digitalRead(KWUKBUT1);
  BOAT5[3] = digitalRead(KWUKBUT2);
  BOAT5[4] = 5; //Define boat number
  radio.write( BOAT5, sizeof(BOAT5) );
  
  /*
  Serial.println(BOAT2[0]);
  Serial.println(BOAT1[1]);
  Serial.println(BOAT1[2]);
  Serial.println(BOAT1[3]);
  Serial.println(BOAT1[4]);
  */

}//--(end main loop )---


int timer() //Loop for reading runtime boats
{
  int Sum = 0;
  if (digitalRead(7) == HIGH)
    Sum += 20; // 20 min
  if (digitalRead(6) == HIGH)
    Sum += 10; //10 min
  if (digitalRead(5) == HIGH)
    Sum += 5; // 5 min
  if (digitalRead(4) == HIGH)
    Sum += 2; // 2,5 min
  return(Sum);
}

//*********( THE END )***********


