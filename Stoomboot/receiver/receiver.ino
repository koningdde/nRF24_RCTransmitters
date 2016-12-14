
/* Ontvanger Basis Staion voor 1 Unit
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
#include <Servo.h>
#include "printf.h"

/*-----( Declare Constants and Pin Numbers )-----*/
#define CSN_PIN    53
#define CE_PIN     9
#define M1         7  
#define M2         6
#define ServoPIN1  2
#define ServoPIN2  3
#define ServoPIN3  4
#define M1speed    5

#define JOY1X       A0
#define JOY1Y       A1
#define JOY2X       A2
#define JOY2Y       A3
#define JOY3X       A4
#define JOY3Y       A5

#define BUT0       0
#define BUT1       1
#define BUT2       5
#define BUT3       6
#define BUT4       7

#define Save


// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xC8E8F0F0E1LL; // Define the receive pipe
const uint64_t pipe2 = 0xC8E8F0F0E0LL; // Define the transmit pipe

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

Servo myservo1;  // create servo object to control servo1 
Servo myservo2;  // create servo object to control servo1 
Servo myservo3;  // create servo object to control servo1 

/*-----( Declare Variables )-----*/
int joystick[13];  //4 element array holding Joystick readings
int myData[6];
int Position1;    // variable to store the servo1 position 
int X1;
int Y1;
int X2;
int Y2;
int X3;
int Y3;
int XB0;
int XB1;
int XB2;
int XB3;
int XB4;
int XB5;
int SWITCHES;
unsigned long started_waiting_at;
unsigned long timeNow;
boolean timeout;

// Define telemetry items
int ketelLow;
int ketelHigh;
int voedingLow;
int pressure;
int actSpeed;
int water;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  printf_begin();
  Serial.begin(57600);
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.openWritingPipe(pipe2);
  radio.printDetails(); 

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(BUT0, OUTPUT);
  pinMode(BUT1, OUTPUT);
  pinMode(BUT2, OUTPUT);
  pinMode(BUT3, OUTPUT);
  pinMode(BUT4, OUTPUT);

  pinMode(M1speed, OUTPUT);
  pinMode(X1, OUTPUT);
  pinMode(Y1, OUTPUT);
  pinMode(X2, OUTPUT);
  pinMode(Y2, OUTPUT);
  pinMode(X3, OUTPUT);
  pinMode(Y3, OUTPUT);
       
  myservo1.attach(ServoPIN1);  // attaches the servo 
  myservo2.attach(ServoPIN2);  // attaches the servo 
  myservo3.attach(ServoPIN3);  // attaches the servo 
}

//--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  radio.startListening();  
  started_waiting_at = micros();               // timeout period, get the current microseconds
  timeout = false;
  
  while ( ! radio.available() ) {                            // While nothing is received
    if (micros() - started_waiting_at > 2000000 ) {           // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }

  if ( timeout ) // Describe the results timeout
  { 
    safe(); // call function failsafe, always run 
  }
  else
  {
  radio.read( joystick, sizeof(joystick) );     
      X1 = (joystick[0]);
      Y1 = (joystick[1]);
      X2 = (joystick[2]);
      Y2 = (joystick[3]);
      X3 = (joystick[4]);
      Y3 = (joystick[5]);
      XB0 = (joystick[6]); 
      XB1 = (joystick[7]); 
      XB2 = (joystick[8]); 
      XB3 = (joystick[9]); 
      XB4 = (joystick[10]);
      XB5 = (joystick[11]);
      SWITCHES = (joystick[12]);
      timeNow = micros();
          
    radio.stopListening();
    delay(10);
    myData[0] = 1;
    myData[1] = 2;
    myData[2] = 3;
    myData[3] = 4;
    myData[4] = 5;
    myData[5] = XB5;
    radio.write( myData, sizeof(myData));   
  }   
 


//------------------------------------------
// Send data to functions

//      motorrun(motor);
      roerout(Y2);
//      butout1run(butout1);
//      butout2run(butout2); 
    

      SWITCHOUT(SWITCHES); //Send BCD to Switch table
       
//------------------------------------------

  }

void motorrun(int joystick){
     int speedset;     
     if (joystick <= 450)
                {      
                digitalWrite (M1, LOW);
                digitalWrite (M2, HIGH);
                speedset = map(joystick, 0, 450, 255, 100); 
                analogWrite(M1speed, speedset);               
                }   
                
     else if (joystick >= 560)
                { 
                digitalWrite (M1, HIGH);
                digitalWrite (M2, LOW);
                speedset = map(joystick, 560, 1023, 100, 255);
                analogWrite(M1speed, speedset);                
                }
                
     else 
                {
                digitalWrite(M1, LOW);
                digitalWrite(M2, LOW);
                } 
}

void butout1run(int test){
  
  if (test != 1)
                {
                digitalWrite(BUT1, LOW); 
                }
     else
                {
                digitalWrite(BUT1, HIGH);
                }

}
  
void butout2run(int test){
  
  if (test != 1)
                {
                digitalWrite(BUT2, LOW); 
                }
     else
                {
                digitalWrite(BUT2, HIGH);
                }

}  

void roerout(int roerwaarde){    
       if (roerwaarde <= 512)
                {      
                Position1 = map(roerwaarde, 0, 512, 0, 90);
                myservo1.write(Position1);
                }   
                
     else if (roerwaarde > 512)
                { 
                Position1 = map(roerwaarde, 512, 1023, 90, 180);
                myservo1.write(Position1);
                }       
}

void safe(){
  Serial.println("failsafe mode"); //run failsafe functions
}

void SWITCHOUT(int DATAIN){
  if (DATAIN > 128) {digitalWrite (40, HIGH); DATAIN = DATAIN - 128;}
    else {digitalWrite (40, LOW);}    
  if (DATAIN > 64) {digitalWrite (41, HIGH); DATAIN = DATAIN - 64;}
    else {digitalWrite (41, LOW);}    
  if (DATAIN > 32) {digitalWrite (42, HIGH); DATAIN = DATAIN - 32;}
    else {digitalWrite (42, LOW);}
  if (DATAIN > 16) {digitalWrite (43, HIGH); DATAIN = DATAIN - 16;}
    else {digitalWrite (43, LOW);}
  if (DATAIN > 8) {digitalWrite (44, HIGH); DATAIN = DATAIN - 8;}
    else {digitalWrite (44, LOW);}
  if (DATAIN > 4) {digitalWrite (45, HIGH); DATAIN = DATAIN - 4;}
    else {digitalWrite (45, LOW);}
  if (DATAIN > 2) {digitalWrite (46, HIGH); DATAIN = DATAIN - 2;}
    else {digitalWrite (46, LOW);}
  if (DATAIN == 1) {digitalWrite (47, HIGH);}
    else {digitalWrite (47, LOW);}    
}


//--(end main loop )---

