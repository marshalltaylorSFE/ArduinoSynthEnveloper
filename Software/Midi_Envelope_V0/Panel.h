#ifndef PANEL_H
#define PANEL_H

#include "mPanelComponents.h"
#include <Arduino.h>

class Panel
{
public:
  Panel( void );
  void update( void );
  void init( void );
  mPanelSwitch load;
  mPanelKnob8Bit attackKnob;
  mPanelKnob8Bit decayKnob;
  mPanelKnob8Bit sustainKnob;
  mPanelKnob8Bit releaseKnob;
  mPanelKnob8Bit masterKnob;
  mPanelRegister reg1;
  mPanelLed redLed;
  mPanelLed greenLed;

protected:
private:
};

#endif // PANEL_H



