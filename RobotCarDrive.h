/*
  This library will help control 4WD Robot car (two motor sets controlled separatelly)
  Made by: Felippe Piazza
  Version: 0.1
  Date: 03/04/2016
*/

#ifndef RobotCarDrive_h
#define RobotCarDrive_h

#include "Arduino.h"
#include "Debugger.h"


class RobotCarDrive
{
  public:
  	RobotCarDrive(int M1Sp, int M2Sp, int M1Dir1, int M1Dir2, int M2Dir1, int M2Dir2, int MaxSpeed, Debugger* debRef);
  	void setForward();
  	void setBackward();
  	void AxleTurn(char side[]);
    void Turn(char side[], int percentage);
  	void MotorGrpCmd(char mtGrp[], int spd);
  	void goStraight(int spd);
  	void SetSpeed(int spd);
  	void ChangeDirection();
	void Stop();

  private:
    void setForwardM1();
    char* otherSide(char side[]);
    void setForwardM2();
    void setBackwardM1();
    void setBackwardM2();
  int _M1Sp ;
  int _M2Sp ;
  int _M1Dir1 ;
  int _M1Dir2 ;
  int _M2Dir1 ;
  int _M2Dir2 ;
  int _max_speed ;
  float _speed_steps;
  boolean _direction; // true = Forward | false = Backward
  int _speed ; // 0 = none | every point sends the percentage of MAX_SPEED relative to one speed step to the speed
  int _right_speed; // 0 = none | every point sends the percentage of MAX_SPEED relative to one speed step to the right motor set
  boolean _right_direction; // true = Forward | false = Backward
  int _left_speed; // 0 = none | every point sends the percentage of MAX_SPEED relative to one speed step to the left motor set
  boolean _left_direction; // true = Forward | false = Backward
  Debugger*	deb;
};

#endif