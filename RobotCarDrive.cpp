/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
 
#include <Arduino.h>
#include <RobotCarDrive.h>
#include <Debugger.h>
#define SPEED_STEPS 100

RobotCarDrive::RobotCarDrive(int M1Sp, int M2Sp, int M1Dir1, int M1Dir2, int M2Dir1, int M2Dir2, int MaxSpeed, Debugger* debRef)
{
  //Motor Initialization
  deb = debRef;
  deb->debug(3,"RobotCarDrive Start", "Setting parameters");
  pinMode(M1Sp, OUTPUT);
  pinMode(M2Sp, OUTPUT);
  pinMode(M1Dir1, OUTPUT);
  pinMode(M1Dir2, OUTPUT);
  pinMode(M2Dir1, OUTPUT);
  pinMode(M2Dir2, OUTPUT);
  _M1Sp = M1Sp;
  _M2Sp = M2Sp;
  _M1Dir1 = M1Dir1;
  _M1Dir2 = M1Dir2;
  _M2Dir1 = M2Dir1;
  _M2Dir2 = M2Dir2;
  _max_speed = MaxSpeed;
  _speed_steps = MaxSpeed / SPEED_STEPS;
  _direction = true; // true = Forward | false = Backward
  _speed = 0; // 0 = none | every point sends the percentage of MAX_SPEED relative to one speed step to the speed
  _right_speed = 0; // 0 = none | every point sends the percentage of MAX_SPEED relative to one speed step to the right motor set
  _right_direction = true; // true = Forward | false = Backward
  _left_speed = 0; // 0 = none | every point sends the percentage of MAX_SPEED relative to one speed step to the left motor set
  _left_direction = true; // true = Forward | false = Backward
  setForward();
}


void RobotCarDrive::setForward() {
  setForwardM1();
  setForwardM2();
  _direction = true;
}

void RobotCarDrive::setForwardM1() {
  digitalWrite(_M1Dir1, HIGH);
  digitalWrite(_M1Dir2,LOW);
  _right_direction = true;
}

void RobotCarDrive::setForwardM2() {
  digitalWrite(_M2Dir1, HIGH);
  digitalWrite(_M2Dir2, LOW);
  _left_direction = true;
}

void RobotCarDrive::setBackward() {
  setBackwardM1();
  setBackwardM2();
  _direction = false;
}

void RobotCarDrive::setBackwardM1(){
  digitalWrite(_M1Dir1, LOW);
  digitalWrite(_M1Dir2,HIGH);
  _right_direction = false;
}

void RobotCarDrive::setBackwardM2(){
  digitalWrite(_M2Dir1, LOW);
  digitalWrite(_M2Dir2, HIGH);
  _left_direction = false;
}

void RobotCarDrive::AxleTurn(char side[]) {
  deb->debug(2,"AxleTurnRight","Making axle turn to the left");
  Turn(side, -100);
}

void RobotCarDrive::Turn(char side[], int percentage) {
 deb->debug(2,"Right Turn","Making easy turn to the right");
 MotorGrpCmd(side, _speed * percentage / 100);
 char* other = otherSide(side);
 MotorGrpCmd(other, _speed);
}

char* RobotCarDrive::otherSide(char side[]) {
  if (side == "r") {
    return "l";
  } else {
    return "r";
  }
}

void RobotCarDrive::MotorGrpCmd(char mtGrp[], int spd) {

  int* mtdir1 = &_M1Dir1;
  int* mtdir2 = &_M1Dir2;
  int* mtspd = &_M1Sp;
  int* mtspd_b = &_left_speed;
  boolean* m_direction = &_left_direction;
  
  if(mtGrp =="r") {
    mtdir1 = &_M2Dir1;
    mtdir2 = &_M2Dir2;
    mtspd = &_M2Sp;
    mtspd_b = &_right_speed;
    m_direction = &_right_direction;
  }

  if ( (*m_direction  and spd < 0) || (!*m_direction and spd > 0) ) {
    analogWrite(*mtspd, 0);
    digitalWrite(*mtdir1, !digitalRead(*mtdir1));
    digitalWrite(*mtdir2, !digitalRead(*mtdir2));
    *m_direction = !*m_direction;
    deb->debug(2,"RobotCarDrive - Direction", "Changing direction for motor ");
  }
  deb->debug(3,"RobotCarDrive - Speed", (char) _speed_steps);
  deb->debug(3,"RobotCarDrive - Speed to set", (char) spd);
  
deb->debug(2,"RobotCarDrive - To Coef1", spd);
  int new_speed = abs(spd) * _speed_steps * (_speed / 100);
  deb->debug(3,"RobotCarDrive - Tunning", mtGrp);
  deb->debug(3,"RobotCarDrive - To speed", (char) new_speed);
  deb->debug(2,"RobotCarDrive - To Coef2", (char) spd);
  analogWrite(*mtspd, new_speed);
  *mtspd_b = spd;
  deb->debug(2,"RobotCarDrive - To speed final", (char) *mtspd_b);
}

void RobotCarDrive::goStraight(int spd) {
  if(_direction) {
    spd = abs(spd);
  } else {
    spd = -abs(spd);
  }
  _speed = spd;
  MotorGrpCmd("r", spd);
  MotorGrpCmd("l", spd);
  deb->debug(3,"RobotCarDrive", "Going Straight");
  deb->debug(2,"RobotCarDrive - Speed right", (char) _right_speed);
  deb->debug(2,"RobotCarDrive - Speed left", (char) _left_speed);
}

void RobotCarDrive::SetSpeed(int spd) {
  _speed = spd;
}

void RobotCarDrive::ChangeDirection() {
  //_right_speed = -1 * _right_speed;
  //_left_speed = -1 * _left_speed;
  MotorGrpCmd("r", -1 * _right_speed);
  MotorGrpCmd("l", -1 * _left_speed);
}

void RobotCarDrive::Stop() {
  _speed = 0;
  MotorGrpCmd("r", _speed);
  MotorGrpCmd("l", _speed);
}
