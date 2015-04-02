#include <MIDI.h>
#include "envelope.h"
#include "Panel.h"

//Global Panel object
Panel myPanel;

//Global Envelope object(s)
Envelope myEnvelope;


//tick variable
long ms_ticks = 0;
uint8_t last_amp_sent = 0;


MIDI_CREATE_DEFAULT_INSTANCE();

// -----------------------------------------------------------------------------
//

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  myPanel.redLed.setState(1);
  myEnvelope.setNoteOn();
  // Do whatever you want when a note is pressed.
  MIDI.sendNoteOn(pitch, 20, channel);

}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  myPanel.redLed.setState(0);
  myEnvelope.setNoteOff();
  // Do something when the note is released.
  // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
  MIDI.sendNoteOff(pitch, 20, channel);

}

// -----------------------------------------------------------------------------

void setup()
{
  //Required to init panel object
  myPanel.init();
  
  //Default settings  
  myEnvelope.setAttack(.25);
  myEnvelope.setDecay(.8);
  myEnvelope.setSustain(75);
  myEnvelope.setRelease(2);
  
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
  
  // Call to update the panel wheneve
  myPanel.update();

  // See if anything changed
  if( myPanel.load.newData == 1 )
  {
  }
  
  if( myPanel.reg1.newData == 1 )
  {
  }
  
  if( myPanel.attackKnob.newData == 1 )
  {
    myEnvelope.setAttack(((float)myPanel.attackKnob.getState()) * 0.5);
  }
  
  if( myPanel.decayKnob.newData == 1 )
  {
    myEnvelope.setDecay(((float)myPanel.decayKnob.getState()/256) * 0.5);
  }
  
  if( myPanel.sustainKnob.newData == 1 )
  {
    myEnvelope.setSustain(myPanel.sustainKnob.getState() >> 1);
  }
  
  if( myPanel.releaseKnob.newData == 1 )
  {
    myEnvelope.setDecay(((float)myPanel.releaseKnob.getState()/256) * 2);
  }
  
  // main program
//  if( ms_ticks > 200 )
//  {
//    ms_ticks = ms_ticks - 200;
//    // Toggle LED
//    myPanel.greenLed.setState( myPanel.greenLed.getState() ^ 1 );
//  }
}

ISR(TIMER1_COMPA_vect)
{
  ms_ticks++;
  myEnvelope.tick();
  if( myEnvelope.amp != last_amp_sent )
  {
    MIDI.sendControlChange(7, myEnvelope.amp, 1);
    last_amp_sent = myEnvelope.amp;
    myPanel.greenLed.setPWM(last_amp_sent);
  }
}
