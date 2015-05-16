#include <MIDI.h>
#include "envelope.h"
#include "Panel.h"
#include "timerModule.h"
#include "stdint.h"

//Global Panel object
Panel myPanel;

//Global Envelope object(s)
Envelope myEnvelope;
Envelope myEnvelopeShadow;

#define ENVTICKRATEMS 5

uint8_t last_amp_sent = 0;


//Global timer object(s)
TimerClass envelopeTimer( ENVTICKRATEMS );
TimerClass idleTimer( 333 );
TimerClass panelTimer( 50 );

#define MAXINTERVAL 2000

uint16_t msTicks = 0;
uint8_t msTicksMutex = 1; //start locked out




MIDI_CREATE_DEFAULT_INSTANCE();

// -----------------------------------------------------------------------------
//

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  //myPanel.redLed.setState(1);
  myEnvelope.setNoteOn();
  // Do whatever you want when a note is pressed.
  MIDI.sendNoteOn(pitch, 20, channel);

}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  //myPanel.redLed.setState(0);
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
  
  pinMode(13, OUTPUT);
  
  //Default settings
  myEnvelope.setSustain( 127 );
  myEnvelope.setAttack( 255, -60 );
  myEnvelope.setDecay( 255, 60 );
  myEnvelope.setRelease( 255, 60 );
  
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
  OCR1A = 16000;

  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);

  // Set CS10 and CS12 bits for 1 prescaler:
  TCCR1B |= (1 << CS10);


  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);

  // enable global interrupts:
  sei();


  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  //Serial.begin(9600);
}

void loop()
{
  uint16_t inputVarTemp = 0;
  int16_t inputPowTemp = 0;
  
  if( msTicksMutex == 0 )  //Only touch the timers if clear to do so.
  {
    envelopeTimer.update(msTicks);
    idleTimer.update(msTicks);
    panelTimer.update(msTicks);

    //Done?  Lock it back up
    msTicksMutex = 1;
  }  //The ISR should clear the mutex.
  

  //Run the outputs here
  if(envelopeTimer.flagStatus() == PENDING)
  {
    myEnvelope.tick( ENVTICKRATEMS );
    myEnvelopeShadow.tick( ENVTICKRATEMS );
    MIDI.sendControlChange(7, myEnvelope.amp >> 1, 1);
    last_amp_sent = myEnvelope.amp;
    myPanel.greenLed.setPWM(255-last_amp_sent);

  }

  //Tick the panel
  if(panelTimer.flagStatus() == PENDING)
  {
    // Call to update the panel wheneve
    myPanel.update();
  }
  
  //Debug loop
  if(idleTimer.flagStatus() == PENDING)
  {
    digitalWrite( 13, digitalRead(13) ^ 1 );
    //Serial.print(myEnvelope.state);
    //Serial.println(myEnvelope.amp);
  }

  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();
  
  // See if anything changed
  if( myPanel.load.newData == 1 )
  {
    if( myPanel.load.getState() == 1 )
    {
      myEnvelope.setNoteOn();
            
    }
    else
    {
      myEnvelope.setNoteOff();
    }
  
  }
  
  if( myPanel.reg1.newData == 1 )
  {
    myPanel.reg1.getState();
    myPanel.redLed.setState(0);
  }
  
  inputPowTemp = myPanel.masterKnob.getState() - 128;
  
  if ( myPanel.attackKnob.newData == 1 )
  {
    inputVarTemp = myPanel.attackKnob.getState();
    //if( inputVarTemp < 1 ) inputVarTemp = 1;
    myEnvelope.setAttack(inputVarTemp, inputPowTemp);
  }

  if ( myPanel.decayKnob.newData == 1 )
  {
    inputVarTemp = myPanel.decayKnob.getState();
    //if( inputVarTemp < 1 ) inputVarTemp = 1;
    myEnvelope.setDecay(inputVarTemp, inputPowTemp);
  }

  if ( myPanel.sustainKnob.newData == 1 )
  {
    inputVarTemp = myPanel.sustainKnob.getState();
    //Allow sustain of zero
    myEnvelope.setSustain(inputVarTemp);
    
  }

  if ( myPanel.releaseKnob.newData == 1 )
  {
    inputVarTemp = myPanel.releaseKnob.getState();
    //if( inputVarTemp < 1 ) inputVarTemp = 1;
    myEnvelope.setRelease(inputVarTemp, inputPowTemp);
  }
  
}


ISR(TIMER1_COMPA_vect)
{
  uint32_t returnVar = 0;
  if(msTicks >= ( MAXTIMER + MAXINTERVAL ))
  {
    returnVar = msTicks - MAXTIMER;

  }
  else
  {
    returnVar = msTicks + 1;
  }
  msTicks = returnVar;
  msTicksMutex = 0;  //unlock
}
