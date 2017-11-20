/*  Beginning Robot Menu for GC Turtle Robots
 *   with Arduino and Display Shield
 *  by John Buschert   October 2015
*/
// ---------------------------------------------------------------------------------

#include <Wire.h>                         // The Display Shield needs Wire for its I2C communication.  It comes with Arduino
#include <Adafruit_RGBLCDShield.h>        // Display Shield library. Find this at Adafruit
#include <physMenu.h>                     // This is is our library used by this menu template.  Find this at GC
#include <Adafruit_MotorShield.h>         // Motor Shield functions.  From Adafruit
#include <Servo.h>                        // Servo library.   Standard with Arduino

PhysicsMenu m = PhysicsMenu();            // This line creates as instance "m" of the Menu object type PhysicsMenu
Adafruit_MotorShield AFMS;                //creates Motor Shield object
Adafruit_DCMotor *leftWheel = AFMS.getMotor(1);   // Left Wheel DC Motor is on port 1 of Shield
Adafruit_DCMotor *rightWheel = AFMS.getMotor(2);   // Right Wheel DC Motor is on port 2 of Shield
Servo rangerServo;                         // creates a servo object called rangerServo

// Use this call:   leftWheel->setSpeed(100);  // to set the left wheel motor speed in the range 0 - 255
// Use this call:   leftWheel->run(FORWARD);   // to make the left wheel move FORWARD, BACKWARD or use RELEASE to stop it

int somenumber;        // adjusted by setValue                 
float level;           // used by readAnalog
int wheelSpeed = 100;  
int wheelDiff = 0;
int range;          // distance (in cm) to object sensed by rangefinder
int angle = 90;     // angle of servo that aims rangefinder (0-180) 90 is straight ahead

// ----------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------
void loop() {
  // THis is where you usually put your main code but for this menu system it's just one line.
  m.dispatch();       //  Checks the buttons and updates the display or calls a function depanding on what you press                                
}

// ============================================== Functions ==============================
// Each function should begin by clearing the display, then printing its name, then pausing briefly
// Somewhere it must have a way back either automatically after it's done or in a loop which checks for the left arrow key  


//  Adjust the speed of the wheels and the difference in their speeds to get the robot to go straight
void setWheelSpeed() { 
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
  }
}  
// Follower
/*
void Follower() {
  m.lcd.clear();
  int prev;
  int idealdir;
  leftWheel->setSpeed(100);
  rightWheel->setSpeed(100);
  while(1) {
    byte buttons = m.lcd.readButtons();
    int distance = getRange();
    if (distance < 45) {
      leftWheel->run(BACKWARD);
      rightWheel->run(BACKWARD);
    }
    else if (distance > 55) {
      idealdir = Sweep();
      if (idealdir == 0) {
        while(Sweep() != 2) {
        // go right
          leftWheel->run(FORWARD);
          rightWheel->run(BACKWARD);
        }
      } else if (idealdir == 1) {
        while(Sweep() != 2) {
          // go left
          leftWheel->run(BACKWARD);
          rightWheel->run(FORWARD);
        }
      } else if (idealdir == 2) {
        leftWheel->run(FORWARD);
        rightWheel->run(FORWARD);
      }
    }
    else if (distance >= 45 && distance <= 55) {
      leftWheel->run(RELEASE);
      rightWheel->run(RELEASE);
    }
    if (buttons == BUTTON_LEFT) {
      leftWheel->run(RELEASE);
      rightWheel->run(RELEASE);
      return;
    }

  }
}
char* names[] = {"Alice", "Bob", "Charlie", "Delta", "Epsilon"};

struct ValueKeeper {
  int array[3];
};

struct PhysicalObject {
  String name;
  int tracking;
  // 0 for lost, 1 for tracking
  int ant;
  
  // short for anticipation which is a long word
  // 0 for left, 1 for none, 2 for right, for consistency. probably won't implement this anytime soon
};

void Follower() {
  int turnTime = 400;
  struct PhysicalObject obj;
  m.lcd.clear();
  // first time "setup" is different, hence why this first is out of the loop
  int probabledirection = Decider(Looker());
  obj.ant = probabledirection;
  obj.tracking = 1;
  obj.name = names[random(5)];
  m.lcd.print("Tracking ");
  m.lcd.print(obj.name);
  leftWheel->run(FORWARD);
  rightWheel->run(FORWARD);
  if (probabledirection == 0) {
    leftWheel->run(BACKWARD);
    rightWheel->run(FORWARD);
    delay(turnTime);
    leftWheel->run(RELEASE);
    rightWheel->run(RELEASE);
  } else if (probabledirection == 1) {
    leftWheel->run(FORWARD);
    rightWheel->run(FORWARD);
  } if (probabledirection == 2) {
    leftWheel->run(FORWARD);
    rightWheel->run(BACKWARD);
    delay(turnTime);
    leftWheel->run(FORWARD);
    rightWheel->run(FORWARD);
  }  
  while(1) {
    int probabledirection = Decider(Looker());
    if (probabledirection == 0) {
      leftWheel->run(BACKWARD);
      rightWheel->run(FORWARD);
      delay(turnTime);
      leftWheel->run(RELEASE);
      rightWheel->run(RELEASE);
    } else if (probabledirection == 1) {
      leftWheel->run(FORWARD);
      rightWheel->run(FORWARD);
    } if (probabledirection == 2) {
      leftWheel->run(FORWARD);
      rightWheel->run(BACKWARD);
      delay(turnTime);
      leftWheel->run(RELEASE);
      rightWheel->run(RELEASE);
    }
    leftWheel->run(FORWARD);
    rightWheel->run(FORWARD);
  }
}

struct ValueKeeper Looker() {
  ValueKeeper values;
  rangerServo.write(115);
  delay(500);
  values.array[0] = getRange();
  rangerServo.write(90);
  delay(500);
  values.array[1] = getRange();
  rangerServo.write(65);
  delay(500);
  values.array[2] = getRange();
  rangerServo.write(90);
  delay(300);
  return values;
}

int Decider(struct ValueKeeper values) {
  int directions[3]={values.array[0], values.array[1], values.array[2]};
  int biggest;
  biggest = 0;
  int probableTarget;
  for (int i = 0; i < 3; i++) {
    if (directions[i] > 70) {
      directions[i] = -1;
    }
    if (directions[i] < 70 && directions[i] > 15) {
      probableTarget = i;
    }
  }
  return probableTarget;
  /*for (int i = 0; i < 3; i++) {
    if (values.array[i] > values.array[biggest]) {
      biggest = i;
    }
    if (values.array[i] > 45 && values.array[i] < 55) {
      probableTarget = i; 
    }
  }
  if (values.array[0] == values.array[1] && values.array[1] == values.array[2]) {
    return 1;
    // if all the readings are the same (lost), go forward?
  }
  if (probableTarget == biggest) {
    return probableTarget;
  } else {
    if (biggest > probableTarget) {
      return biggest;
    }
  }
  */


void Follower() {
  leftWheel->setSpeed(100);
  rightWheel->setSpeed(100);
  int values[3]; 
  m.lcd.clear();
  while(1) {
    rangerServo.write(115);
    delay(150);
    values[0] = getRange();
    rangerServo.write(90);
    delay(150);
    values[1] = getRange();
    rangerServo.write(65);
    delay(150);
    values[2] = getRange();
    m.lcd.clear();
    m.lcd.print(values[0]);
    m.lcd.print("  ");
    m.lcd.print(values[1]);
    m.lcd.print("  ");
    m.lcd.print(values[2]);
    if (values[0] > (values[1] - 20) && values[0] < (values[1] + 20) && values[0] > (values[2]-20) && values[0] < (values[2]+20) && values[1] > 15) {
      leftWheel->run(FORWARD);
      rightWheel->run(FORWARD);
      // this is the nothing case, no imminent objects
    } else if (values[1] < 15) {
      leftWheel->run(BACKWARD);
      rightWheel->run(BACKWARD);
      // this is the something is right in front of you case
    } else if (values[0] < values[1] && values[0] < values[2] && values[0] < 70) {
      // whatever's on the left is the closest
      leftWheel->run(BACKWARD);
      rightWheel->run(FORWARD);
      delay(250);
      leftWheel->run(FORWARD);
      rightWheel->run(FORWARD);
    } else if (values[2] < values[1] && values[2] < values[0] && values[2] < 70) {
      // whatever's on the right is the closest
      leftWheel->run(FORWARD);
      rightWheel->run(BACKWARD);
      delay(250);
      leftWheel->run(FORWARD);
      rightWheel->run(FORWARD);
    }
  }
}  

// Move robot with arrow buttons ---------------------------------------------
void moveRobot() {
  m.lcd.clear();
  m.lcd.print("Speed = ");
  m.lcd.print(wheelSpeed);  
  m.lcd.setCursor(0,1);
  m.lcd.print("Select for Menu");
  leftWheel->setSpeed(wheelSpeed + wheelDiff);
  rightWheel->setSpeed(wheelSpeed - wheelDiff);
  while(1) {
    delay(100); // This slows counting when a button is held down
    byte buttons = m.lcd.readButtons(); // 'buttons' value depends on what was presssed
    if (buttons == BUTTON_UP) {
      leftWheel->run(FORWARD);
      rightWheel->run(FORWARD);
    }  
    if (buttons == BUTTON_DOWN) {
      leftWheel->run(BACKWARD);
      rightWheel->run(BACKWARD);
    }   
    if (buttons == BUTTON_LEFT) {
      leftWheel->run(BACKWARD);
      rightWheel->run(FORWARD);
    }   
    if (buttons == BUTTON_RIGHT) {
      leftWheel->run(FORWARD);
      rightWheel->run(BACKWARD);
    }   
    if (buttons == BUTTON_SELECT) {
      leftWheel->run(RELEASE);
      rightWheel->run(RELEASE);
      return;
    }
  }
}

// Continuously use rangefinder to give a live display ---------------------------------
void testRanger() {
  m.lcd.clear();
  while(1){
    range = getRange();
    m.lcd.home();
    m.lcd.print("Range = ");
    m.lcd.print(range);  
    m.lcd.print("   ");
  
    if (m.lcd.readButtons() == BUTTON_LEFT) {return;}
  }
}

// Use arrow buttons to move servo in small increments ----------------------------------
void testServo() { 
  m.lcd.clear();
  while(1) {
    m.lcd.home();
    m.lcd.print("Angle = ");
    m.lcd.print(angle);  
    m.lcd.print("   ");
    delay(100); // This slows counting when a button is held down
    byte buttons = m.lcd.readButtons(); // 'buttons' value depends on what was presssed
    if (buttons == BUTTON_UP) {angle += 5;}  // Up button increments menu option
    if (buttons == BUTTON_DOWN) {angle -= 5;}   // Down button decrements menu option
    rangerServo.write(angle);
    if (buttons == BUTTON_SELECT) {return;}
  }
}

// Adjust a number using the up and down buttons ---------------------------
void setValue() {
  m.lcd.clear();
  while(1) {
    m.lcd.home();
    m.lcd.print("Number = ");
    m.lcd.print(somenumber);  
    m.lcd.print("   ");
    delay(100); // This slows counting when a button is held down
    byte buttons = m.getButtons(); // 'buttons' value depends on what was presssed
    if (buttons == BUTTON_UP) {somenumber++;}  // Up button increments menu option
    if (buttons == BUTTON_DOWN) {somenumber--;}   // Down button decrements menu option
    if (buttons == BUTTON_LEFT) {return;}
  }
}  

int Sweep() {
  m.lcd.clear();
  m.lcd.home();
  int dir;
  int delayperiod = 600;
  int forward = getRange();
  rangerServo.write(125);
  delay(delayperiod);
  int left = getRange();
  rangerServo.write(55);
  delay(delayperiod);
  int right = getRange();
  if (left > forward) {
    if (right > left) {
      dir = 0;
    } else {
      dir = 1;
    }
  } else {
    dir = 2;
  }
  m.lcd.clear();
  if (dir == 0) {
    m.lcd.print("right");
  } else if (dir == 1) {
    m.lcd.print("left");
  } else if (dir == 2) {
    m.lcd.print("forward");
  }
  return dir;
}

void Chicken() {
  leftWheel->setSpeed(100);
  rightWheel->setSpeed(100);
  m.lcd.clear();
  while(1) {
    leftWheel->run(FORWARD);
    rightWheel->run(FORWARD);
    int sweepres = Sweep();
    rangerServo.write(90);
    delay(400);
    int dist = getRange();
    if (dist < 25) {
      if (sweepres == 0) {
        leftWheel->run(FORWARD);
        rightWheel->run(BACKWARD);
        delay(1500);
        leftWheel->run(RELEASE);
        rightWheel->run(RELEASE);
        }
      else if (sweepres == 1){
        leftWheel->run(BACKWARD);
        rightWheel->run(FORWARD);
        delay(1500);
        leftWheel->run(RELEASE);
        rightWheel->run(RELEASE);
       }
    }
  }
}

void Dora() {
  m.lcd.clear();
  while(1) {
   int range = getRange();
   m.lcd.print("90");
   m.lcd.print(range); 
  }
}
//------------------------  Non-menu Functions -------------------------------



// returns the range in cm
int getRange() {
  int rangecm;
  digitalWrite(3, HIGH);
  delayMicroseconds(20);
  digitalWrite(3,LOW);
  rangecm = pulseIn(4, HIGH);
  rangecm = rangecm / 58;
  return rangecm;
}

void setup() {
  // Here is where you put lines that execute just once.  Like setting input and output.
  m.begin();  // this starts up the menu
  m.addItem("1 Wheel Speeds", &setWheelSpeed);    //  Here is where you put the menu items.
  m.addItem("2 Move Robot  ", &moveRobot);       //  For each menu item, you give the text of the menu label
  m.addItem("3 Range Finder", &testRanger);     //  and then the name of the function to be called when you select it
  m.addItem("4 Move Servo  ", &testServo);     
  m.addItem("5 Adjust someN", &setValue);  
  m.addItem("6 Follower    ", &Follower);   
  m.addItem("7 Sweep       ", &Sweep);
  m.addItem("8 Chicken     ", &Chicken);


  m.lcd.setBacklight(0x1);     // Turns on the backlight.  Will be done by default in next version of physMenu
  pinMode(5, OUTPUT);  // Rangefinder trigger
  pinMode(6, INPUT);   // Rangefinder response
  AFMS.begin();  // Initialize the motor shield
  rangerServo.attach(2);  // Associates rangerServo with pin 2
}
