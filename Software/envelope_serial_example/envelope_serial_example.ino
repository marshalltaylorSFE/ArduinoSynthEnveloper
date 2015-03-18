#include "envelope.h"
/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */
Envelope myEnvelope;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  myEnvelope.setAttack(.05);
  myEnvelope.setDecay(.04);
  myEnvelope.setSustain(64);
  myEnvelope.setRelease(.1);

  Serial.println("\nTo operate the test, enter 't', '1', and '0' in the serial monitor.\n");
  Serial.println("  't' causes an envelope tick to occur.");
  Serial.println("  '1' simulates a note-on event and moves towards attack peak");
  Serial.println("  '0' simulates a note-off event.");

}

void loop() {

}


void serialEvent() {
  if (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if( inChar == 't' ) {
      //do tick stuff
      Serial.println("\ntick...");
      myEnvelope.tick();
      Serial.print("Amplituce:  ");
      Serial.println(myEnvelope.amp);
      Serial.print("State:      ");
      Serial.println(myEnvelope.state);
      Serial.print("Note State: ");
      Serial.println(myEnvelope.noteState);

    }
    if( inChar == '1' ) {
      //do note on
      myEnvelope.setNoteOn();
    }
    if( inChar == '0' ) {
      //do note off
      myEnvelope.setNoteOff();
    }
    if( inChar == 'v' ) {
      //init envelope
      myEnvelope.setAttack(.5);
      myEnvelope.setDecay(.5);
      myEnvelope.setSustain(64);
      myEnvelope.setRelease(.5);
    }
  }
}



