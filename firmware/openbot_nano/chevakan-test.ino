//---------------------------------
#include <AFMotor.h> 
#include <NewPing.h>      //add Ultrasonic sensor library

#define TRIG_PIN A0 // Pin A0 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN A1 // Pin A1 on the Motor Drive Shield soldered to the ultrasonic sensor
#define MAX_DISTANCE 300 // sets maximum useable sensor measuring distance to 300cm
#define MAX_SPEED 160 // sets speed of DC traction motors to 150/250 or about 70% of full speed - to get power drain down.
#define MAX_SPEED_OFFSET 40 // this sets offset to allow for differences between the two DC traction motors
#define COLL_DIST 30 // sets distance at which robot stops and reverses to 30cm
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.


int leftDistance, rightDistance; //distances on either side
int curDist = 0;
String motorSet = "";
int speedSet = 0;

//float inNums[]  = {0,0,0,0,0};
//char t;
char inByte=0, outByte=48;     
AF_DCMotor motor1(3); //M3 pin
AF_DCMotor motor2(4); //M4 pin

void setup() {
  // set up Serial library at 9600 bps
  Serial.begin(9600);            
  Serial.println("Chevakan!"); 
  // turn on motor
  motor1.setSpeed(100);
  motor2.setSpeed(100);  
}

void loop() {  
//  Serial.println("loop" ); 

  while(Serial.available() > 0) { 
       // read the incoming byte:
          inByte = Serial.read(); 
          Serial.println(inByte);

//          isObstacles();
          
          drive(inByte); 
          
//          setSpeed();
    } 
    //  
    //  motor1.run(FORWARD);
    //  for (i=0; i<255; i++) {
    //    motor1.setSpeed(i);  
    //    delay(10);
    // } 
}


void isObstacles(){
  curDist = readPing();   // read distance
  if (curDist < COLL_DIST) 
     {changePath();}  // if forward is blocked change direction
  forward();  // move forward
  delay(500);
}

void changePath() {
    stopit();   // stop forward movement
    right();
//  myservo.write(36);  // check distance to the right
    delay(500);
    rightDistance = readPing(); //set right distance
    delay(500);
    left();
//    myservo.write(144);  // check distace to the left
    delay(700);
    leftDistance = readPing(); //set left distance
    delay(500);
//    myservo.write(90); //return to center
    delay(100);
    compareDistance();
  }
 
void compareDistance()   // find the longest distance
{
  if (leftDistance>rightDistance) //if left is less obstructed 
  {
    left();
  }
  else if (rightDistance>leftDistance) //if right is less obstructed
  {
    right();
  }
   else //if they are equally obstructed
  {
    turnAround();
  }
}


//-------------------------------------------------------------------------------------------------------------------------------------

int readPing() { // read the ultrasonic sensor distance
  delay(70);   
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}

void setSpeed(){
for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);  
//    leftMotor1.setSpeed(speedSet);
//    leftMotor2.setSpeed(speedSet);
//    rightMotor1.setSpeed(speedSet); 
//    rightMotor2.setSpeed(speedSet); 
    delay(5);
  }
}

void drive( char dir){
//  Serial.println("halo..."  ); 
    delay(10);
  switch (dir) {
  case '1':
    forward();
    break;
  case '2':
    back();
    break;
  case '3':
    left();
    break;
  case '4':
    right();
    break;
  case '5':
    stopit();
    break;
  default: 
//    forward();
// back();
    break;
}
  }
  
void forward(){
     motor1.run(FORWARD); 
     motor2.run(FORWARD);  
     Serial.println("FW");
}

void back(){
     motor1.run(BACKWARD); 
     motor2.run(BACKWARD);   
     Serial.println("BW"); 
}
void left(){
      motor1.run(BACKWARD); 
      motor2.run(FORWARD);   
      Serial.println("LH");     
}
void right(){
     motor1.run(FORWARD); 
     motor2.run(BACKWARD);   
     Serial.print("RH"); 
}
void stopit(){
      motor1.run(RELEASE);
      motor2.run(RELEASE); 
}
void turnAround() {
  motorSet = "RIGHT";
  right();
//  leftMotor1.run(FORWARD);      // turn motor 1 forward
//  leftMotor2.run(FORWARD);      // turn motor 2 forward
//  rightMotor1.run(BACKWARD);    // turn motor 3 backward
//  rightMotor2.run(BACKWARD);    // turn motor 4 backward
 
//  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
//  rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);
  delay(200); // run motors this way for 1700
  motorSet = "FORWARD";
  forward();
//  leftMotor1.run(FORWARD);      // set both motors back to forward
//  leftMotor2.run(FORWARD);
//  rightMotor1.run(FORWARD);
//  rightMotor2.run(FORWARD);      
}  