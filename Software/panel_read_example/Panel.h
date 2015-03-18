#ifndef PANEL_H
#define PANEL_H

#include "PanelComponents.h"
#include <Arduino.h>

class Panel
{
public:
  Panel( void );
  void update( void );
  void init( void );
  PanelSwitch load;
  PanelKnob8Bit attackKnob;
  PanelKnob8Bit decayKnob;
  PanelKnob8Bit sustainKnob;
  PanelKnob8Bit releaseKnob;
  PanelRegister reg1;

protected:
private:
};

#endif // PANEL_H



