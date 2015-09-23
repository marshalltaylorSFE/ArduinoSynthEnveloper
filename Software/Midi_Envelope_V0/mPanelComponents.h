#ifndef PANELCOMPONENTS_H
#define PANELCOMPONENTS_H
#include <Arduino.h>

//---Switch------------------------------------------------------
class mPanelSwitch
{
public:
  mPanelSwitch( void );
  void update( void );
  void init( uint8_t );
  uint8_t getState( void );
  uint8_t state;
  uint8_t invert;
  uint8_t pinNumber;
  uint8_t newData;
protected:
private:
};

//---Knob--------------------------------------------------------
class mPanelKnob8Bit
{
public:
  mPanelKnob8Bit( void );
  void update( void );
  void init( uint8_t );
  uint8_t getState( void );
  uint8_t state;
  uint8_t pinNumber;
  uint8_t newData;
protected:
private:
};

//---Register----------------------------------------------------
class mPanelRegister
{
public:
  mPanelRegister( void );
  void update( void );
  void init( uint8_t, uint8_t, uint8_t );
  uint8_t getState( void );
  uint8_t state;
  uint8_t pinMap[8];
  uint8_t length;
  uint8_t newData;
protected:
private:
  mPanelSwitch switchBit0;
  mPanelSwitch switchBit1;
  mPanelSwitch switchBit2;
};

//---LED---------------------------------------------------------
class mPanelLed
{
public:
  mPanelLed( void );
  void init( uint8_t );
  void setState( uint8_t );
  void setPWM( uint8_t );
  uint8_t getState( void );
  uint8_t pinNumber;
protected:
private:
};

#endif // PANELCOMPONENTS_H


