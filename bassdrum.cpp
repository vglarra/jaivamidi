#include <Control_Surface.h>
USBMIDI_Interface midi;

// Push button connected between pin 2 and ground.
// Note message is sent when pressed.
Button pushbutton {2};

// MIDI address of the note to send
const MIDIAddress noteAddress {MIDI_Notes::C[4], Channel_1};

void setup() {
  Control_Surface.begin();
  pushbutton.begin(); // enables internal pull-up
  midi.begin();       // initialize the MIDI interface
}

void loop() {
  Control_Surface.loop();
  midi.update(); // read and handle or discard MIDI input
  pushbutton.update(); // read the button state

  // Read the analog signal from A0
  int analogValue = analogRead(A0);

  // Map the analog value to a MIDI velocity (0-127)
  uint8_t velocity = map(analogValue, 0, 1023, 0, 127);

  if (pushbutton.getState() == Button::Falling) {
    midi.sendNoteOn(noteAddress, velocity);
  } // send a note on event
  else if (pushbutton.getState() == Button::Rising) {
    midi.sendNoteOff(noteAddress, velocity);
  } // send a note off event
}
