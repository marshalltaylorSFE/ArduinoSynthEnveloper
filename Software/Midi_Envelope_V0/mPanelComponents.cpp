#include "mPanelComponents.h"

//---Switch------------------------------------------------------
mPanelSwitch::mPanelSwitch( void )
{

}

void mPanelSwitch::init( uint8_t pinNum )
{
  pinNumber = pinNum;
  pinMode( pinNumber, INPUT_PULLUP );
  digitalWrite( pinNumber, 1 );  //Might not do anything
  update();
  //force newData high in case knob starts on last value
  newData = 1;
}

void mPanelSwitch::update( void )
{
  uint8_t tempState = digitalRead( pinNumber ) ^ 0x01;
  if( state != tempState )
  {
    state = tempState;
    newData = 1;
  }
}

uint8_t mPanelSwitch::getState( void )
{
  newData = 0;

  return state;
}


//---Knob--------------------------------------------------------
mPanelKnob8Bit::mPanelKnob8Bit( void )
{

}

void mPanelKnob8Bit::init( uint8_t pinNum )
{
  pinNumber = pinNum;
  pinMode( pinNumber, INPUT );
  update();
  //force newData high in case knob starts on last value
  newData = 1;
}

void mPanelKnob8Bit::update( void )
{
  uint8_t tempState = (analogRead( pinNumber ) >> 2) & 0xFF;
  if( state != tempState )
  {
    state = tempState;
    newData = 1;
  }
}

uint8_t mPanelKnob8Bit::getState( void )
{
  newData = 0;

  return state;
}

//---Register----------------------------------------------------
mPanelRegister::mPanelRegister( void )
{

}

void mPanelRegister::init( uint8_t pinNumBit2, uint8_t pinNumBit1, uint8_t pinNumBit0 )
{
  length = 3;
  pinMap[2] = pinNumBit2;
  pinMap[1] = pinNumBit1;
  pinMap[0] = pinNumBit0;

  pinMode( pinNumBit2, INPUT_PULLUP );
  pinMode( pinNumBit1, INPUT_PULLUP );
  pinMode( pinNumBit0, INPUT_PULLUP );

  update();
  //force newData high in case knob starts on last value
  newData = 1;
}

void mPanelRegister::update( void )
{
  uint8_t tempState;
  tempState = digitalRead( pinMap[2] ) << 2;
  tempState |= digitalRead( pinMap[1] ) << 1;
  tempState |= digitalRead( pinMap[0] );

  //Invert
  tempState ^= 0x07;

  if( state != tempState )
  {
    state = tempState;
    newData = 1;
  }
}

uint8_t mPanelRegister::getState( void )
{
  newData = 0;

  return state;
}

//---LED---------------------------------------------------------
mPanelLed::mPanelLed( void )
{

}

void mPanelLed::init( uint8_t pinVariable )
{
  pinNumber = pinVariable;
  digitalWrite( pinNumber, 1 ); //off
  pinMode( pinNumber, OUTPUT );

}

void mPanelLed::setState( uint8_t state )  // Overload with some enum type later
{
  state ^= 1;
  digitalWrite( pinNumber, state );
}

uint8_t mPanelLed::getState( void )
{
  uint8_t stateVar = digitalRead( pinNumber ) ^ 1;
  return stateVar;
}

void mPanelLed::setPWM( uint8_t state )
{
  analogWrite( pinNumber, state );
}
