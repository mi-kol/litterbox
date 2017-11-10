/*  Arduino Display Menu Template for GC Electronics Class
 *  Written by Peter Miller and John Buschert in 2015
 *  
 *  Requires an Adafriut LCD Display Shield
 *  

*/
// ---------------------------------------------------------------------------------

#include <Wire.h>                         // The Display Shield needs Wire for its I2C communication.  It comes with Arduino
#include <Adafruit_RGBLCDShield.h>        // Then Adafruit provides this library to talk to the shield.  Find this at Adafruit
#include <Wire.h>                         // The Display Shield needs Wire for its I2C communication.  It comes with Arduino
#include <Adafruit_RGBLCDShield.h>        // Display Shield library. Find this at Adafruit
#include <physMenu.h>                     // This is is our library used by this menu template.  Find this at GC
#include <Adafruit_MotorShield.h>         // Motor Shield functions.  From Adafruit
#include <Servo.h>

PhysicsMenu m = PhysicsMenu();            // This line creates as instance "m" of the Menu object type PhysicsMenu

int somenumber;        // adjusted by setValue                 
float level;           // used by readAnalog
int freq;              //used in squarewave
int pulse;             //used in PMW and servo6
int angle;             //used in servoSweep
int v;                 // abrv. of value used in light sense
int t;                 // time in range()
int wheelSpeed;
int wheelDiff;    
int x;
int y;
int z;  
int n; 
int l;
int o;
int b;  
Servo myredservo;
Adafruit_MotorShield AFMS; //creates Motor Shield object
Adafruit_DCMotor *leftWheel = AFMS.getMotor(1);   // Left Wheel DC Motor is on port 1 of Shield
Adafruit_DCMotor *rightWheel = AFMS.getMotor(2);   // Right Wheel DC Motor is on port 2 of Shield
Adafruit_StepperMotor *myStepper1 = AFMS.getStepper(200, 1);

// ----------------------------------------------------------------------------------
//85 is 90
void setup() {
  myredservo.attach(6);
  wheelSpeed=150;
  m.begin(); 
  m.addItem("1 Align wheels", &align);
  m.addItem("2 Range in cm", &range); 
  m.addItem("3 Test servo", &testServo);
  m.addItem("4 Test robot", &roboTest);
  m.addItem("5 Go chicken", &chicken);
  m.addItem("6 conga", &roboChase);
  m.lcd.setBacklight(0x1);             
  AFMS.begin();  
}

// Use this call:   leftWheel->setSpeed(100);  // to set the left wheel motor speed in the range 0 - 255
// Use this call:   leftWheel->run(FORWARD);   // to make the left wheel move FORWARD, BACKWARD or use RELEASE to stop it
// -----------------------------------------------------------------------------------
void loop() {
  m.dispatch();       //  Checks the buttons and updates the display or calls a function depanding on what you press                                
}
int range(){
  m.lcd.clear();
  pinMode(3,OUTPUT);
  pinMode(4,INPUT);
  while(1){
  byte buttons = m.getButtons();
  if (buttons == BUTTON_LEFT) {return;}
  digitalWrite(3,HIGH);
  delayMicroseconds(10);
  digitalWrite(3,LOW);
  t=pulseIn(4,HIGH);
  x=t/58;
  m.lcd.setCursor(0,1); 
  m.lcd.print(x); 
  m.lcd.print("  cm  ");}}

int findR(){
  pinMode(3,OUTPUT);
  pinMode(4,INPUT);
  digitalWrite(3,HIGH);
  delayMicroseconds(10);
  digitalWrite(3,LOW);
  t=pulseIn(4,HIGH);
  n=t/58;
  return n;}

void roboTest() {
  m.lcd.clear();
  m.lcd.print("Speed = ");
  m.lcd.print(wheelSpeed);  
  m.lcd.print("   ");
  m.lcd.setCursor(0,1);
  m.lcd.print("select to return");
  leftWheel->setSpeed(wheelSpeed + wheelDiff);
  rightWheel->setSpeed(wheelSpeed - wheelDiff);
  while(1) {
    byte buttons = m.lcd.readButtons();
    if (buttons == BUTTON_SELECT) {return;}
    if (buttons == BUTTON_UP) {leftWheel->run(FORWARD);rightWheel->run(FORWARD);delay(3000);leftWheel->run(RELEASE);rightWheel->run(RELEASE);}
    if (buttons == BUTTON_DOWN) {leftWheel->run(BACKWARD);rightWheel->run(BACKWARD);delay(3000);leftWheel->run(RELEASE);rightWheel->run(RELEASE);}
    if (buttons == BUTTON_LEFT) {leftWheel->run(BACKWARD);rightWheel->run(FORWARD);delay(3000);leftWheel->run(RELEASE);rightWheel->run(RELEASE);}
    if (buttons == BUTTON_RIGHT) {leftWheel->run(FORWARD);rightWheel->run(BACKWARD);delay(3000);leftWheel->run(RELEASE);rightWheel->run(RELEASE);}
  }
}

void roboChase() {
  m.lcd.clear();
  while(1){
    l=0;
    v=findR(); 
    l=v;
    v=findR();
    l=(l+v);
    v=findR();
    level=(l+v)/3;
    v=(int) level;
    l=0;
    if (v>20) {leftWheel->run(FORWARD);rightWheel->run(FORWARD);}
    if (v<20) {leftWheel->run(RELEASE);rightWheel->run(RELEASE);}}
  }

void chicken() {
  l=0;
  o=0;
  b=0;
  int i;
  i=800;
  m.lcd.clear();
  ck();
  while(1) {
    y=findR();
    if (y>15) {leftWheel->run(FORWARD);rightWheel->run(FORWARD);}
    if (y<15) {
    leftWheel->run(RELEASE);
    rightWheel->run(RELEASE);
    ck();
    if (z>y && z>x) {leftWheel->run(FORWARD);rightWheel->run(BACKWARD);delay(i);leftWheel->run(RELEASE);rightWheel->run(RELEASE);}                                                                                                                                  // Do i need a "return;" here and in ck?
    if (x>y && x>z) {leftWheel->run(BACKWARD);rightWheel->run(FORWARD);delay(i);leftWheel->run(RELEASE);rightWheel->run(RELEASE);}
    }
  }
}

void ck() {
  m.lcd.clear();
  m.lcd.home();
  
  myredservo.write(50);
  delay(300);
  x=findR(); // x is range in cm  at 60 deg
  //l=x;
  //x=findR(); // x is range in cm  at 60 deg
  //level=(l+x)/2;
  //x=(int) level;
  //l=0;
  
  myredservo.write(80);
  delay(300);
  y=findR(); // y is range in cm at 90 deg
 // o=y;
  //y=findR();
  //level=(o+y)/2;
  //y=(int) level;
  //o=0;
  
  myredservo.write(110);
  delay(300);
  z=findR(); //z is range in cm at 120 deg (LEFT)
  //b=z;
  //z=findR();
  //level=(b+z)/2;
  //z=(int) level;
  //b=0;
  
  myredservo.write(80);
  return;
}


void align() { 
  m.lcd.clear();
  while(1) {
    m.lcd.clear();
    m.lcd.print("Speed = ");
    m.lcd.print(wheelSpeed);  
    m.lcd.print("   ");
    m.lcd.setCursor(0,1);
    m.lcd.print("Diff = ");
    m.lcd.print(wheelDiff);  
    m.lcd.print("    ");
    delay(100); // This slows counting when a button is held down
    byte buttons = m.lcd.readButtons(); // 'buttons' value depends on what was presssed
    if (buttons == BUTTON_UP) {wheelSpeed += 10;}    // Up button  to speed up
    if (buttons == BUTTON_DOWN) {wheelSpeed -= 10;}  // Down button to slow down
    if (buttons == BUTTON_LEFT) {wheelDiff -= 1;}    // Left button to trim motor speed toward left
    if (buttons == BUTTON_RIGHT) {wheelDiff += 1;}   // Right button to trim toward right
    leftWheel->setSpeed(wheelSpeed + wheelDiff);
    rightWheel->setSpeed(wheelSpeed - wheelDiff);
    if (buttons == BUTTON_SELECT) {return;}   // Select Button to return to main menu
  }}

//reads voltage
void pot(){
   m.lcd.clear();
   pinMode(1,OUTPUT);
   while(1){
   byte buttons = m.getButtons();
   if (buttons == BUTTON_LEFT) {return;}
   x=analogRead(0); // read current after potentiometer 
   m.lcd.setCursor(0,1); 
   m.lcd.print(  x  );
   }
}

 

void movestep(){
  m.lcd.clear();
  m.lcd.print("  step  ");
  z=5;
  x=z;
  y=z;
  myStepper1->setSpeed(60); 
  while(1){
    byte buttons = m.getButtons();
    if (buttons == BUTTON_LEFT) {z=z-1;}
    if (buttons == BUTTON_RIGHT) {z=z+1;}
    if (buttons == BUTTON_UP) {myStepper1->step(x, FORWARD, DOUBLE);}
    if (buttons == BUTTON_DOWN) {myStepper1->step(y, BACKWARD, DOUBLE);}
  }
}


void lightSense() {
  m.lcd.clear();
  pinMode(5,OUTPUT);
  while(1){
  v=analogRead(0);
  m.lcd.setCursor(0,1); 
  m.lcd.print(v); 
  if (v<200) {digitalWrite(5,HIGH);}
  if (v>200) {digitalWrite(5,LOW);}
  byte buttons = m.getButtons();
  if (buttons == BUTTON_LEFT) {return;}
  }
}

void testServo() { 
  m.lcd.clear();
  angle=85;
  while(1) {
    m.lcd.home();
    m.lcd.print("Angle = ");
    m.lcd.print(angle);  
    m.lcd.print("   ");
    delay(100); // This slows counting when a button is held down
    byte buttons = m.lcd.readButtons(); // 'buttons' value depends on what was presssed
    if (buttons == BUTTON_UP) {angle += 5;}  // Up button increments menu option
    if (buttons == BUTTON_DOWN) {angle -= 5;}   // Down button decrements menu option
    myredservo.write(angle);
    if (buttons == BUTTON_SELECT) {return;}
  }
}



