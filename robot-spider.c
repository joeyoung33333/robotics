/*Robot Spider (Darby)
  Inverse Kinematic Equation and Functions by Robbie Nakata.
  Applied to and edited by Joseph Young for Project:Robot Spider (Darby).

  The current code is set to loop through the three functions: Up, Around, and Rotate.
  This program is not connected to the Ambient light sensor.

*/


#include <Servo.h>

#define rightsFront 1
#define rightBack 2
#define leftFront 3
#define leftBack 4

//The physical lengths of each legs
double coxa = 2.36;
double femur = 3.35;
double tibia = 6.89;

double pi = 3.14;

//The following variables determine how wide the legs are spread from each other
//default postion for Y
double centerY = 6;
//default postion for X
double centerX = 0.8;

double xVal = -pi / 4;
double RFx2, RFy2, RFz2;


//These are almost always the current position of the quarduped's leg
double RFx, RFy, RFz;
double RBx, RBy, RBz;
double LFx, LFy, LFz;
double LBx, LBy, LBz;

//Right Front Servos (1)
Servo RF0; //D1 50
Servo RF1; //D2 51
Servo RF2; //D3 52

//Right Back Servos (2)
Servo RB0; //C1 41
Servo RB1; //C2 42
Servo RB2; //C3 43

//Left Front Servos (3)
Servo LF0; //B1 24
Servo LF1; //B2 25
Servo LF2; //B3 26

//Left Back Servos (4)
Servo LB0; //A1 14
Servo LB1; //A2 15
Servo LB2; //A3 16

//Timers for the millis() function for some movements

unsigned long tNow = 0;
unsigned long tOld = 0;

unsigned long tNowSM = 0;
unsigned long tOldSM = 0;

unsigned long tNow1 = 0;
unsigned long tOld1 = 0;

//counters
int j = 0;
int counter = 0;

char inByte;

void setup() {
  //attaching all of the servos to each output port on the Arduino
  RB0.attach(41);
  RB1.attach(42);
  RB2.attach(43);

  RF0.attach(50);
  RF1.attach(51);
  RF2.attach(52);

  LB0.attach(14);
  LB1.attach(15);
  LB2.attach(16);

  LF0.attach(24);
  LF1.attach(25);
  LF2.attach(26);

  Serial.begin(9600);

  //Starting position at power up
  fullMove(0, 0, 4);
}


//Loop allows for the robot move autonomously 
//Easily edited to be controlled by sensors or serial monitor control 
void loop() {
  Serial.begin(9600);

  Serial.println("Up");
  up(3, 5);
  delay(100);
  up(3, 5);
  delay(100);
  up(3, 5);
  delay(1000);

  Serial.println("Around");
  around(1.5, 3);
  delay(100);
  around(1.5, 3);
  delay(100);
  around(1.5, 3);
  delay(1000);

  Serial.println("Rotate");
  rotate(25);
  rotate(-50);
  rotate(25);
  delay(100);
  rotate(25);
  rotate(-50);
  rotate(25);
  delay(100);
  rotate(25);
  rotate(-50);
  rotate(25);
  delay(1000);

  Serial.println("Reset");
  fullMove(0, 0, 4);
  delay(1000);


  //Checks to see if there is new serial data
  if (Serial.available()) {
    inByte = Serial.read();
    Serial.println(inByte);
  }

/*
//serial monitor control

  //moves the robot body in a circle
  if (inByte == 'a') {
    around(1.5, 3);
  }
  //The following three functions are the basic creep gait walk functions. By varying the number of steps in shiftWalk(steps,time) you can control the speed of the grait
  else if (inByte == 'q') {
    shiftWalk(50, 10);
  }
  else if (inByte == 'w') {
    shiftWalk(25, 10);
  }
  else if (inByte == 'e') {
    shiftWalk(13, 10);
  }

  //center the robot
  else if (inByte == 'c') {
    fullMove(0, 0, 4);
  }

  //move the robot up and down using IK
  else if (inByte == 'u') {
    up(3, 5);
  }

  //Rotate the robots body back and forth
  else if (inByte == 'l') {
    rotate(25);
    rotate(-50);
    rotate(25);
  }

  else if (inByte == 'r') {
    smoothMove(3, LFx, LFy, 4 - 2, 50, 10);
  }
  else if (inByte == 't') {
    smoothMove(1, RFx, RFy, RFz - 2, 50, 10);
  }
  else if (inByte == 'f') {
    smoothMove(4, LBx, LBy, 4 - 2, 50, 10);
    smoothMove(2, RBx, RBy, 4 - 2, 50, 10);
  }
  else if (inByte == 'g') {
    smoothMove(2, RBx, RBy, 4 - 2, 50, 10);
  }

}
*/

}

//This function rotates the robot body from the current position to "degree" and then back.
void rotate(int degree) {
  int RFcur = RF0.read();
  int RBcur = RB0.read();
  int LFcur = LF0.read();
  int LBcur = LB0.read();
  Serial.println(RFcur);
  if (degree > 0) {
    for (int count1 = 0; count1 < degree; count1++) {
      RFcur += 1;
      RBcur += 1;
      LFcur += 1;
      LBcur += 1;
      RF0.write(RFcur);
      RB0.write(RBcur);
      LF0.write(LFcur);
      LB0.write(LBcur);
      delay(10);
    }
  }
  else {
    for (int count1 = 0; count1 > degree; count1--) {
      RFcur -= 1;
      RBcur -= 1;
      LFcur -= 1;
      LBcur -= 1;
      RF0.write(RFcur);
      RB0.write(RBcur);
      LF0.write(LFcur);
      LB0.write(LBcur);
      delay(10);
    }
  }

}



//Move a leg from current position to new position smoothly by using small angle chages broken up into "steps" every t1 milliseconds

void smoothMove(int servoNumber, double x, double y, double z, int t1, int steps) {

  //RF
  if (servoNumber == 1) {
    //how big each step is
    double dx = (x - RFx) / steps;
    double dy = (y - RFy) / steps;
    double dz = (z - RFz) / steps;

    //moves the leg along each axis by the corresponding delta, waits t1 then repeats
    for (int i = 0; i <= steps; i++) {
      RFx += dx;
      RFy += dy;
      RFz += dz;
      moveServos2RF(RFx, RFy, RFz);
      delay(t1);
    }
  }

  //RB
  else if (servoNumber == 2)
  {
    double dx = (x - RBx) / steps;
    double dy = (y - RBy) / steps;
    double dz = (z - RBz) / steps;
    for (int i = 0; i <= steps; i++) {
      RBx += dx;
      RBy += dy;
      RBz += dz;
      moveServos2RB(RBx, RBy, RBz);
      delay(t1);
    }
  }

  //LF
  if (servoNumber == 3)
  {
    double dx = (x - LFx) / steps;
    double dy = (y - LFy) / steps;
    double dz = (z - LFz) / steps;
    for (int i = 0; i <= steps; i++) {
      LFx += dx;
      LFy += dy;
      LFz += dz;
      moveServos2LF(LFx, LFy, LFz);
      delay(t1);
    }
  }

  //LB
  else if (servoNumber == 4)
  {
    double dx = (x - LBx) / steps;
    double dy = (y - LBy) / steps;
    double dz = (z - LBz) / steps;
    for (int i = 0; i <= steps; i++) {
      LBx += dx;
      LBy += dy;
      LBz += dz;
      moveServos2LB(LBx, LBy, LBz);
      delay(t1);
    }
  }
  else if (servoNumber == 0) {

  }
}


//This is the primary creep gait that I've developed. It uses the smoothMove functions broken up into a number of "steps" with a delay "time" between each step
void shiftWalk(int steps, int time) {
  //starting vertical position
  double z = 3;
  //How far to raise the leg for each movement
  double dz = -2;

  //shift entire body forward and to the left. This causes the robot to move forward 1 inch
  //and prepares for the left legs to move
  smoothFullMove(2, -1, z, 2, 1, z, steps * 2, time);
  print3(LBx, LBy, LBz);

  //Moves left back leg up then forward then down
  smoothMove(4, LBx, LBy, z + dz, steps, time); //moveServos2LB(3,5,1);
  smoothMove(4, 1, 5, z + dz, steps, time); //moveServos2LB(1,5,1);
  smoothMove(4, 1, 5, z, steps, time); //moveServos2LB(0,5,3);

  //Moves left front leg up then forward then down
  smoothMove(3, -1, 5, z + dz, steps, time); //moveServos2LF(-1,5,0);
  smoothMove(3, -3, 5, z + dz, steps, time); //moveServos2LF(-3,5,1);
  smoothMove(3, -3, 5, z, steps, time); //moveServos2LF(-3,5,3);

  //shift entire body forward and to the right. This causes the robot to move forward 1 inch
  //and prepares for the right legs to move
  smoothFullMove(2, 1, z, 2, -1, z, steps * 2, time);

  //Moves right back leg up then forward then down
  smoothMove(2, 3, 5, z + dz, steps, time); //moveServos2LB(3,5,1);
  smoothMove(2, 1, 5, z + dz, steps, time); //moveServos2LB(1,5,1);
  smoothMove(2, 1, 5, z, steps, time); //moveServos2LB(0,5,3);

  //Moves right front leg up then forward then down
  smoothMove(1, -1, 5, z + dz, steps, time); //moveServos2LF(-1,5,0);
  smoothMove(1, -3, 5, z + dz, steps, time); //moveServos2LF(-3,5,1);
  smoothMove(1, -3, 5, z, steps, time); //moveServos2LF(-3,5,3);

}


/*IK MOVE FUNCTIONS*/
//moves the robot vertically from z1 to z2 and then back to z1
void up(double z1, double z2) {
  double z = z1;
  while (z <= z2) {
    fullMove(0, 0, z);
    delay(25);
    z += .1;
  }
  while (z >= z1) {
    fullMove(0, 0, z);
    delay(25);
    z -= .1;
  }
}

//moves the robot in a circle of radius r and at a height z
void around(double r, double z) {
  double x = 0;
  while (x <= 2 * 3.14) {
    fullMove(r * cos(x), r * sin(x), z);
    delay(50);
    x += .1;
  }
}



//Moves the entire robot body to position x,y,z. Note this results in very jerky movements
void fullMove(double x, double y, double z) {

  double rightY = centerY - y;
  double leftY = centerY + y;

  double frontX = x - centerX;
  double backX = x + centerX;

  RFx = x - centerX;
  RFy = centerY - y;
  RFz = z;

  RBx = x + centerX;
  RBy = centerY - y;
  RBz = z;

  LFx = x - centerX;
  LFy = centerY + y;
  LFz = z;

  LBx = x + centerX;
  LBy = centerY + y;
  LBz = z;

  moveServosRF(gamma(RFx, RFy, RFz), alpha(RFx, RFy, RFz), beta(RFx, RFy, RFz));
  moveServosRB(gamma(RBx, RBy, RBz), alpha(RBx, RBy, RBz), beta(RBx, RBy, RBz));
  moveServosLF(gamma(LFx, LFy, LFz), alpha(LFx, LFy, LFz), beta(LFx, LFy, LFz));
  moveServosLB(gamma(LBx, LBy, LBz), alpha(LBx, LBy, LBz), beta(LBx, LBy, LBz));

}

//Moves the entire body from x0,y0,z0 to x,y,z smoothly with "steps" steps separated by t1 milliseconds. This is usefully for shifting the center of gravity
void smoothFullMove(double x0, double y0, double z0, double x, double y, double z, int t1, int steps) {
  double dx = (x - x0) / steps;
  double dy = (y - y0) / steps;
  double dz = (z - z0) / steps;
  for (int i = 0; i < steps; i++) {
    x0 += dx;
    y0 += dy;
    z0 += dz;
    fullMove(x0, y0, z0);
    delay(t1);
  }
}

//used for attaching the legs to the servos at the right position
void calibrate() {
  RB0.write(90);
  RB1.write(0);
  RB2.write(90);

  RF0.write(90);
  RF1.write(0);
  RF2.write(90);

  LB0.write(90);
  LB1.write(0);
  LB2.write(90);

  LF0.write(90);
  LF1.write(0);
  LF2.write(90);
}

void calibrate2() {
  moveServosRF(90, 90, 90);
  moveServosRB(90, 90, 90);
  moveServosLF(90, 90, 90);
  moveServosLB(90, 90, 90);
}

//debugging function to print out three ints
void print3(int i, int o, int p) {
  Serial.print(i);
  Serial.print("\t");
  Serial.print(o);
  Serial.print("\t");
  Serial.println(p);
}


/***INVERSE KINEMATICS***/

//IK for gamma (coxa angle)
int gamma(double x, double y, double a) {
  double g = atan(x / y);
  return int(g * 57.3) + 90;
}

//IK for beta (femur angle)
double beta(double x, double y, double a) {
  double g = atan(x / y);
  x /= cos(g);
  //double bet =acos((pow(x,2)+pow(y,2)+pow(a,2)-pow(c,2)-pow(b,2))/(-2*c*b));
  double bet = acos((pow(a, 2) + pow(y - coxa, 2) - pow(tibia, 2) - pow(femur, 2)) / (-2 * femur * tibia));
  return int(bet * 57.3);

}

//IK for alpha (tibia angle)
double alpha(double x, double y, double a) {
  double g = atan(x / y);
  x /= cos(g);
  double L2 = sqrt(pow(a, 2) + pow(y - coxa, 2));
  double alp1 = atan((y - coxa) / a);
  double alp2 = acos((pow(tibia, 2) - pow(femur, 2) - pow(L2, 2)) / (-2 * femur * L2));
  double alp = alp1 + alp2;
  return int(alp * 57.3);
}

/**Move Functions**/

//The following "moveServos2XX" move the tip of the corresponding legs to the points x,y,z. This probably could have been combined with the
//"moveServosXX" functions but were written after so they simply call those functions.
void moveServos2RB(double x, double y, double z) {
  //
  RBx = x;
  RBy = y;
  RBz = z;
  moveServosRB(gamma(RBx, RBy, RBz), alpha(RBx, RBy, RBz), beta(RBx, RBy, RBz));
}

void moveServos2RF(double x, double y, double z) {
  RFx = x;
  RFy = y;
  RFz = z;
  moveServosRF(gamma(RFx, RFy, RFz), alpha(RFx, RFy, RFz), beta(RFx, RFy, RFz));

}
void moveServos2LB(double x, double y, double z) {
  LBx = x;
  LBy = y;
  LBz = z;
  //  Serial.print("alpha");
  // Serial.println(alpha(LBx,LBy,LBz));
  moveServosLB(gamma(LBx, LBy, LBz), alpha(LBx, LBy, LBz), beta(LBx, LBy, LBz));

}
void moveServos2LF(double x, double y, double z) {
  LFx = x;
  LFy = y;
  LFz = z;
  moveServosLF(gamma(LFx, LFy, LFz), alpha(LFx, LFy, LFz), beta(LFx, LFy, LFz));
}

//The following "moveServosXX" functions move the servos in one leg to the given angles. gamma (g), alpha (a), and beta (b) as defined in the IK model
void moveServosRB(int g, int a, int b) {
  //The following calibrations were determined experimentally to get the angles to line up with the IK equations
  g = (g);
  a = (180 - a + 10);
  b = (b + 10);

  RB0.write(g);
  RB1.write(a);
  RB2.write(b);
  //if out of range
  if (g > 180 || g < 0 || a > 180 || a < 0 || b > 180 || b < 0)
    Serial.println("RB error");
  // print3(g,a,b);
}


//move the joints in the RF servo to angles g, a, and b
void moveServosRF(int g, int a, int b) {
  //The following calibrations were determined experimentally to get the angles to line up with the IK equations
  g = (g);
  a = (180 - a + 10);
  b = (b + 10);

  RF0.write(g);
  RF1.write(a);
  RF2.write(b);
  //if out of range
  if (g > 180 || g < 0 || a > 180 || a < 0 || b > 180 || b < 0)
    Serial.println("RF error");
}

//move the joints in the LB servo to angles g, a, and b
void moveServosLB(int g, int a, int b) {
  //The following calibrations were determined experimentally to get the angles to line up with the IK equations
  g = 180 - g - 10;
  a = 180 - a + 5;
  b = b + 5;
  LB0.write(g);
  LB1.write(a);
  LB2.write(b);
  //if out of range
  if (g > 180 || g < 0 || a > 180 || a < 0 || b > 180 || b < 0) {
    Serial.println("LB error");
  }
}

//move the joints in the LF servo to angles g, a, and b
void moveServosLF(int g, int a, int b) {
  //The following calibrations were determined experimentally to get the angles to line up with the IK equations
  g = 180 - g + 5;
  a = 180 - a - 7;
  b = b + 15;
  LF0.write(g);
  LF1.write(a);
  LF2.write(b);
  //if out of range
  if (g > 180 || g < 0 || a > 180 || a < 0 || b > 180 || b < 0)
    Serial.println("LF error");
}

