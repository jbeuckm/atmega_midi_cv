#include <MIDI.h>
#include "AH_MCP4922.h"

#define LED 13   		    // LED pin on Arduino Uno

#define AI1 A1                                  //Analog input monitor 1
#define AI2 A2                                  //Analog input monitor 2

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN) 
AH_MCP4922 AnalogOutput1(11,10,12,LOW,HIGH);
AH_MCP4922 AnalogOutput2(11,10,12,HIGH,HIGH);

AH_MCP4922 AnalogOutput3(8,7,9,LOW,HIGH);
AH_MCP4922 AnalogOutput4(8,7,9,HIGH,HIGH);



MIDI_CREATE_DEFAULT_INSTANCE();


void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  digitalWrite(LED,HIGH);
  
  AnalogOutput1.setValue((pitch - 12) * 42);
  AnalogOutput2.setValue(velocity * 32);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
        digitalWrite(LED,LOW);
}

void handleControlChange(byte channel, byte number, byte value)
{
  if (number == 1) {
    AnalogOutput3.setValue(value * 32);
  }
}

void handlePitchBend(byte channel, int bend)
{
    AnalogOutput4.setValue(bend/4 + 2048);
}


// -----------------------------------------------------------------------------

void setup()
{
    pinMode(LED, OUTPUT);
    
    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    MIDI.setHandleNoteOff(handleNoteOff);
    
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandlePitchBend(handlePitchBend);

    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
    
    // C8 at full velocity for calibration on powerup
    handleNoteOn(1, 108, 127);
}

void loop()
{
    MIDI.read();
}
