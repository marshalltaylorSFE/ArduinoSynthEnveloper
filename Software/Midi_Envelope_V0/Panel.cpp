#include "Panel.h"

#define loadPin 2
#define regBit2Pin 3
#define regBit1Pin 4
#define regBit0Pin 5
#define attackKnobPin A0
#define decayKnobPin A1
#define sustainKnobPin A2
#define releaseKnobPin A3
#define masterKnobPin A4
#define greenLedPin 6
#define redLedPin 7

Panel::Panel( void )
{

}

void Panel::init( void )
{
  load.init( loadPin );
  attackKnob.init( attackKnobPin );
  decayKnob.init( decayKnobPin );
  sustainKnob.init( sustainKnobPin );
  releaseKnob.init( releaseKnobPin );
  masterKnob.init( masterKnobPin );
  reg1.init( regBit2Pin, regBit1Pin, regBit0Pin );
  greenLed.init( greenLedPin );
  redLed.init( redLedPin );
  
}

void Panel::update( void )
{
  load.update();
  attackKnob.update();
  decayKnob.update();
  sustainKnob.update();
  releaseKnob.update();
  masterKnob.update();
  reg1.update();
  
}

