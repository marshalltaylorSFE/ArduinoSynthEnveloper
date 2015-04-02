#include <MIDI.h>
#include "envelope.h"

#define LEDPIN 13
uint8_t sensorValue[5];
uint8_t sensorValueOld[5];
uint8_t switchValue[4];
uint8_t switchPins[4] = {2, 3, 4, 5};
uint8_t ledValue[2];
uint8_t ledPins[2] = {6, 7};


//tick variable
long ms_ticks = 0;
uint8_t last_amp_sent = 0;

Envelope myEnvelope;

MIDI_CREATE_DEFAULT_INSTANCE();

// -----------------------------------------------------------------------------
//

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  digitalWrite(ledPins[1], 0);
  myEnvelope.setNoteOn();
  // Do whatever you want when a note is pressed.
  MIDI.sendNoteOn(pitch, 20, channel);

}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  digitalWrite(ledPins[1], 1);
  myEnvelope.setNoteOff();
  // Do something when the note is released.
  // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
  MIDI.sendNoteOff(pitch, 20, channel);

}

//void handleControlChange(byte channel, byte number, byte value)
//{
//  if(number == 7)
//  {
//    last_amp_sent = value;
//    digitalWrite(stat1led, 0);
//  }
//}

// -----------------------------------------------------------------------------

void setup()
{
  myEnvelope.setAttack(.25);
  myEnvelope.setDecay(.8);
  myEnvelope.setSustain(75);
  myEnvelope.setRelease(2);
  
  pinMode(LEDPIN, OUTPUT);
  pinMode(switchPins[0], INPUT_PULLUP);
  pinMode(switchPins[1], INPUT_PULLUP);
  pinMode(switchPins[2], INPUT_PULLUP);
  pinMode(switchPins[3], INPUT_PULLUP);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  
  // Connect the handleNoteOn function to the library,
  // so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
  MIDI.setHandleNoteOff(handleNoteOff);

  //MIDI.setHandleControlChange(handleControlChange);

  // Initiate MIDI communications, listen to all channels

  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 1250;

  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);

  // Set CS
  TCCR1B |= (1 << CS11) | (1 << CS10);  // /64


  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);

  // enable global interrupts:
  sei();


  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

void loop()
{
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();

  //get sensors
  sensorValue[0] = analogRead(A0) >> 2;
  sensorValue[1] = analogRead(A1) >> 2;
  sensorValue[2] = analogRead(A2) >> 2;
  sensorValue[3] = analogRead(A3) >> 2;

  //  Check the state of the knobs, update as necessary
  if(sensorValue[0] != sensorValueOld[0])
  {
    myEnvelope.setAttack(((float)sensorValue[0]/256) * 0.5);
    sensorValueOld[0] = sensorValue[0];
  }
  if(sensorValue[1] != sensorValueOld[1])
  {
    myEnvelope.setDecay(((float)sensorValue[1]/256) * 0.5);
    sensorValueOld[1] = sensorValue[1];
  }
  if(sensorValue[2] != sensorValueOld[2])
  {
    myEnvelope.setSustain(sensorValue[2] >> 1);
    sensorValueOld[2] = sensorValue[2];
  }
  if(sensorValue[3] != sensorValueOld[3])
  {
    myEnvelope.setDecay(((float)sensorValue[3]/256) * 2);
    sensorValueOld[3] = sensorValue[3];
  }
  
  
  // main program
  if( ms_ticks > 200 )
  {
    ms_ticks = ms_ticks - 200;
    // Toggle LED
    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
    //Serial.println(ms_ticks);
  }
}

ISR(TIMER1_COMPA_vect)
{
  ms_ticks++;
  myEnvelope.tick();
  digitalWrite(ledPins[1], 1);
  if( myEnvelope.amp != last_amp_sent )
  {
    MIDI.sendControlChange(7, myEnvelope.amp, 1);
    last_amp_sent = myEnvelope.amp;
    digitalWrite(ledPins[1], 0);
  }
  //digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
