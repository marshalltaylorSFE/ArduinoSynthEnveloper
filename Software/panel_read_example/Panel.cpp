#include "Panel.h"

#define loadPin 2
#define regBit2Pin 3
#define regBit1Pin 4
#define regBit0Pin 5
#define attackKnobPin 0
#define decayKnobPin 1
#define sustainKnobPin 2
#define releaseKnobPin 3

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
  reg1.init( regBit2Pin, regBit1Pin, regBit0Pin );
  
}

void Panel::update( void )
{
  load.update();
  attackKnob.update();
  decayKnob.update();
  sustainKnob.update();
  releaseKnob.update();
  reg1.update();
  
}

