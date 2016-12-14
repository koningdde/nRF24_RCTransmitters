//PPM_Sum receiver
#define channumber 8 //8 Channel radio
#define filter 2 // Ontvangstfilter
int channel[channumber]; //Array tbv channels
int lastReadChannel[channumber]; // Last values readed
int lastReadChannel2[channumber];
int count=0; //Teller

//Diverse
int coldstart;

//Servo control
#include <Servo.h>

//Motor control
#include <AFMotor.h>
AF_DCMotor motor(2, MOTOR12_8KHZ); //Motor 1 aangesloten
int speedm1 = 0;

//Uitgangen
const int rood = 22;
const int groen = 23;
const int wit = 24;
const int waterpomp = 47;

// Music items
#include <SD.h>               // need to include the SD library
#define SD_ChipSelectPin 49   //using digital pin 4 on arduino nano 328
#include <TMRpcm.h>           //  also need to include this library...
TMRpcm tmrpcm;                // create an object for use in this sketch
//End music items

//Servo en motor regelaar
Servo servoThrottle;
#define THROTTLE_OUT_PIN 10 //Servo op pin 7
int angle; //Waarde voor servoroer
Servo servoGas;
#define GAS_OUT_PIN 9
int gas;

void setup()
{
 coldstart=1;
 Serial.begin(9600); //Iniciamos com serial/ Serial Begin
 pinMode(2, INPUT); //PPM-Sum input
 pinMode(rood, OUTPUT); //PPM-Sum input
 pinMode(groen, OUTPUT); //PPM-Sum input
 pinMode(wit, OUTPUT); //PPM-Sum input
 pinMode(waterpomp, OUTPUT); //Output tbv relais
 tmrpcm.speakerPin = 46; //Speaker pin
  
  //Check aanwezigheid SD card 
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
  return;   // don't do anything more if not
  }
 tmrpcm.volume(7);
 
 //Set startup levels
 for(int i = 0; i <= channumber-1; i++) //Ciclo para imprimir valores/Cycle to print values 
     {
       lastReadChannel[i]=1090;
     }

} 

void loop()
{
  
 if(pulseIn(2, HIGH) > 3000) //If pulse > 3000 useconds, continues
 { 
   for(int i = 0; i <= channumber-1; i++) //Read the pulses of the channels
   {
     channel[i]=pulseIn(2, HIGH);
   }
   for(int i = 0; i <= channumber-1; i++) //Average the pulses
   {
     if((channel[i] > 1550) || (channel[i] < 600))//If channel > max range, chage the value to the last pulse
     {
      channel[i]= lastReadChannel[i];  
     }
     else
     { 
     channel[i]=(lastReadChannel[i]+channel[i])/2; //Average the last pulse eith the current pulse
     count++; //increment counter
     }
   }
}

//Bootup delay   
 if((count > filter) && (coldstart == 1))
   {
   delay(2000);
   coldstart=0;  
   count=0; 
   servoThrottle.attach(THROTTLE_OUT_PIN); //koppel de servo aan de uitgang en houd deze vast
   servoGas.attach(GAS_OUT_PIN); //koppel de servo aan de uitgang en houd deze vast
   }
//End delay   
     
   if((count > filter) && (coldstart == 0))//Si el contador es mayor al filtro imprime valores/ If counter is > than filter, then prints values
   {
     
     for(int i = 0; i <= channumber-1; i++) //Ciclo para imprimir valores/Cycle to print values 
     {
       lastReadChannel[i]=channel[i];
     }
   


 //Servoroer
     if ((channel[5] > 1000) && (channel[5] < 1100)) 
     { 
     angle = 1500;
     servoThrottle.writeMicroseconds(angle);
     //Serial.println(channel[5]); 
     }
     else 
     {
       angle = map(channel[5], 630,1520,1100,2000);
       servoThrottle.writeMicroseconds(angle); 
       //Serial.println(angle);      
     }
 //Einde servoroer
 
   
 //Verlichting
      if((channel[4] > 1000) & (channel[4] < 1300)) //si el canal 5 tiene un rango mayor a 500 enciende el LED/ If channel 5 is > than 500 turn on the led
     {
       digitalWrite(rood, HIGH);
       digitalWrite(groen, HIGH);
       digitalWrite(wit, LOW);
     }
     else if (channel[4] > 1300)
     {
     digitalWrite(rood, HIGH);
     digitalWrite(groen, HIGH);
     digitalWrite(wit, HIGH);
     }
     else
     {
     digitalWrite(rood, LOW);
     digitalWrite(groen, LOW);
     digitalWrite(wit, LOW);
     }
// Eind verlichting

//Stappenmotor
if ((channel[6] < 1000) && (channel[6] > 600))
     {
     //stepmotor.step(10, FORWARD, SINGLE);  
     }
      else if (channel[6] > 1300)
     {
     //stepmotor.step(10, BACKWARD, SINGLE);
     }
     else
     {    
     //stepmotor.release();    
     }
//Einde stappenmotor

//Muziek
     //Music loop
     if((channel[3] > 1000) && (channel[3] < 1300)) //Read valeu
     {
       //tmrpcm.play("1.wav");
       //Serial.println("speel");
     }
     else
     {
       //tmrpcm.disable();
     }
//Einde muziek

//Extra motor    
if (channel[1] > 1120)  { 
     
     speedm1 = map(channel[1], 1080,1530,20,255);
     motor.setSpeed(speedm1);
     motor.run(FORWARD);
     Serial.println(speedm1);
     
 }
 else {
 motor.run(RELEASE);
 }
//Einde extra motor

//Aandrijfmotor    
if (((channel[2] < 1080) && (channel[2] > 600)) | (channel[2] > 1130))  { 
     gas = map(channel[2], 680,1530,1000,2000);
     servoGas.writeMicroseconds(gas);
     //Serial.println(gas);
 }
 else {
   servoGas.writeMicroseconds(1500);
 }
//Einde aandrijfmotor

//Waterpomp    
if (channel[7] > 1080)  { 
     digitalWrite(waterpomp, HIGH);
 }
 else {
   digitalWrite(waterpomp, LOW);
 }
//Einde Waterpomp

 count=0; //Restart couter. 
   }
}

