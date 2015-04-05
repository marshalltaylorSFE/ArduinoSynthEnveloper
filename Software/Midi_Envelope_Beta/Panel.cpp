#include "Panel.h"

#define loadPin 2
#define regBit2Pin 3
#define regBit1Pin 4
#define regBit0Pin 5
#define attackKnobPin 0
#define decayKnobPin 1
#define sustainKnobPin 2
#define releaseKnobPin 3
#define masterKnobPin 4
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

