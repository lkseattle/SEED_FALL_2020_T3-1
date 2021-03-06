//Finding responce of instantaneous velocity for the system with 2 wheels spinning. Need to  fix in order to get more accurate readings
#include <Encoder.h>
#include "DualMC33926MotorShield.h" //PWm goes from -400 to 400 
#include <AutoPID.h>

#define nD2 4 //tri-state disables both outputs of both motor channels when low
#define M1Dir 7 // Motor 1 direction input (voltage sign M1)
#define M2Dir 8 // motor 2 direction input (Voltage sign M2)
#define M1PWM 9 // Motor 1 speed input (Command Voltage M1)
#define M2PWM 10 // Motor 2 speed input (command Votlage M2)
#define nSF 12 //Status flag

Encoder MotorEncoder1(2,5); //used 2 and 3 as they have the best interrupt timing, will need to change when adding second motor
Encoder MotorEncoder2(3,6); //used 2 and 3 as they have the best interrupt timing, will need to change when adding second motor

int WheelSpinTime=3000; // Spin wheel for 3 seconds
double sampleTime=10;//10 ms
int RealTime=0;
int i=0;
double Rad1Prev=0;
double Rad2Prev=0;
double RadV1=0;
double RadV2=0;
//interupt times 
double interuptTime=0;


double counter1=0; //coutner for motor 1 and 2 resp.
double counter2=0;
double rev1=0;
double rev2=0;// revolutions made for motor 1 and 2 resp.
double Rad1=0;
double Rad2=0;//Rads for motor 1 and 2 resp.
double RotationalV1=0;
double RotationalV2=0;


DualMC33926MotorShield md;

void setup() {
  // put your setup code here, to run once:
   pinMode(nD2,OUTPUT); 
   pinMode(M1Dir,OUTPUT);
   pinMode(M2Dir,OUTPUT);
   pinMode(M1PWM,OUTPUT);
   pinMode(M1PWM,OUTPUT);
   pinMode(nSF,INPUT);     
   md.init(); 
   Serial.begin (250000);//baud rate set to 250000
  // attachInterrupt(digitalPinToInterrupt(2),EncoderChange,CHANGE);
  // attachInterrupt(digitalPinToInterrupt(3),EncoderChange,CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(millis()>RealTime+sampleTime && millis() < WheelSpinTime) // Spins wheel for 1 second every 10ms and collects the angular velocity
  {
   md.setM1Speed(400); //Full speed for wheel 1 might half
   md.setM2Speed(400); //Full speed for wheel 2 might half
    
   counter1=MotorEncoder1.read(); //gets motor counts of wheel 1
   counter2=MotorEncoder2.read();  //gets motor counts of wheel 2
    
   rev1=(counter1)/(64*50); //Gets current revelution point of wheel 1
   rev2=(counter2)/(64*50); //Gets current revelution point of wheel 2
    
   Rad1=rev1*6.2832; //Gets current radial point of wheel 1
   Rad2=rev2*6.2832; //Gets current radial point of wheel 2
    
   RadV1=(Rad1-Rad1Prev)/(sampleTime/1000); // Computes the rotational velocity of wheel 1
   RadV2=(Rad2-Rad2Prev)/(sampleTime/1000); // Computes the rotational velocity of wheel 2
    
   Serial.println(RadV1); //Sends Radial velocity 
   Serial.println(RadV2);
    
   Rad1Prev=Rad1;
   Rad2Prev=Rad2;
    
   RealTime+=sampleTime;
  }
 if(millis()> WheelSpinTime && i==0) //stops the motor from running after we reach whatever value is assigned to WheelSpinTime
 {
  md.setM1Speed(0); //Stops wheel 1
  md.setM2Speed(0); //Stops wheel 2 
  i++;
  } 
}
