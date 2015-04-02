/*
  Read envelope type panel
 
 This example code is in the public domain.
 */

#include "Panel.h"

uint8_t temp;
Panel myPanel;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  myPanel.init();
  
}

uint8_t i = 0;
// the loop routine runs over and over again forever:
void loop() {
  myPanel.update();
  uint8_t gotSomething = myPanel.load.newData | myPanel.reg1.newData
                          | myPanel.attackKnob.newData | myPanel.decayKnob.newData | myPanel.sustainKnob.newData | myPanel.releaseKnob.newData;
  if( gotSomething )
  {
    Serial.print(myPanel.load.getState());
    Serial.print(myPanel.reg1.getState());
    Serial.print(myPanel.attackKnob.getState());
    Serial.print(myPanel.decayKnob.getState());
    Serial.print(myPanel.sustainKnob.getState());
    Serial.print(myPanel.releaseKnob.getState());
    
    //myPanel.redLed.setState(myPanel.reg1.getState());
    myPanel.redLed.setState(myPanel.load.getState());
    
    Serial.println();

  }

  i++;
  myPanel.greenLed.setPWM(i);
  

  delay(10);        // delay in between reads for stability
}



