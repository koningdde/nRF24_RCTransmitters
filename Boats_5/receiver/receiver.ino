// Ontvanger horende bij 5 boten zender; bootnummer staat vast, zie onderin.
 

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
/*-----( Declare Constants and Pin Numbers )-----*/
//      SCK_PIN   13
//      Miso_PIN  12
//      Mosi_PIN  11
#define CSN_PIN    9
#define CE_PIN     8
#define M1         7  
#define M2         6
#define BUT1       4
#define BUT2       10
#define ServoPIN1  3
#define M1speed    5

#define Save
int failsave = 0;
#define Retries = 0;

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

Servo myservo1;  // create servo object to control servo1 

/*-----( Declare Variables )-----*/
int joystick[5];  //4 element array holding Joystick readings
int Position1;    // variable to store the servo1 position 
int roer;
int motor;
int butout1;
int butout2;
int failsafe = 0; //Interger failsafe, set to 0
int boatnumber;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(57600);
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();;
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(BUT1, OUTPUT);
  pinMode(BUT2, OUTPUT);
  pinMode(M1speed, OUTPUT);
  myservo1.attach(ServoPIN1);  // attaches the servo 
  boatnumber = Binary(); //Find boatnumber, run once in bootup
  Serial.print("Dit is boot nummer ");
  Serial.println(boatnumber);
}

//--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while (!done)
    {
      done = radio.read( joystick, sizeof(joystick) );
      if (joystick[4] == boatnumber)
      {      
      motor = (joystick[0]);
      roer = (joystick[1]);
      butout1 = (joystick[2]);
      butout2 = (joystick[3]);
      failsafe = 0;
      ///Serial.println("data");
      //Serial.println(joystick[0]);
      //Serial.println(joystick[1]);
      //Serial.println(joystick[2]);
      //Serial.println(joystick[3]);
      //Serial.println(joystick[4]);
      }
      
      }  
  }   
  else
  {    
      //Failsafe setup, every 5th transmit pipes is for this receiver. Ignore 4 reading pipes.
      failsafe = failsafe + 1; //count failsafe retries.
      //Serial.println(failsafe);
   }

//------------------------------------------
// Send data to functions
      motorrun(motor);
      roerout(roer);
      butout1run(butout1);
      butout2run(butout2);  
      safe(failsafe); // call function failsafe, always run 
//------------------------------------------
  }

void motorrun(int joystick){
     int speedset;     
     if ((joystick <= 450) && (failsafe < 40))
                {      
                digitalWrite (M1, LOW);
                digitalWrite (M2, HIGH);
                speedset = map(joystick, 0, 450, 255, 100); 
                analogWrite(M1speed, speedset);               
                }   
                
     else if ((joystick >= 560) && (failsafe < 40))
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
                //Serial.println(speedset);
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

void safe(int retries){
  if (retries > 40){ 
  //Serial.println("failsafe mode"); //run failsafe functions
  }
}

//Calculate binairy input to integer boat number
int Binary()
{
  int Sum = 0;
  if (digitalRead(0) == HIGH)
    Sum += 1;
  if (digitalRead(1) == HIGH)
    Sum += 2;
  if (digitalRead(2) == HIGH)
    Sum += 4;
  //if (digitalRead(A3) > 500)
  //  Sum += 8;
  return(Sum);
}
//--(end main loop )---

