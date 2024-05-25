/** 
 * This is an example that demonstrates the use a Transposer to transpose the
 * note events sent out by NoteButton%s.
 *
 * @boards  AVR, AVR USB, Due, Nano 33, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 2-9: momentary push buttons (to ground) to use as "piano keys"
 * - 10: momentary push button (to ground) to transpose one semitone up
 * - 11: momentary push button (to ground) to transpose one semitone down
 * 
 * The internal pull-up resistor for the buttons will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - By default, the 8 push buttons on pins 2-9 send MIDI Note events for notes
 *   C4 - C5.
 * - If you press the push button on pin 10, all notes are transposed one 
 *   semitone up.
 * - If you press the push button on pin 11, all notes are transposed one 
 *   semitone down.
 * - If you press the push buttons on pins 10 and 11 simultaneously, the 
 *   transposer resets.
 * 
 * Mapping
 * -------
 * 
 * Route the Arduino MIDI port into a synth to play the notes.
 * 
 * Written by PieterP, 2019-10-26  
 * https://github.com/tttapa/Control-Surface
 */
 
#include <Control_Surface.h> // Include the Control Surface library

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
 
// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;
 
using namespace MIDI_Notes;
 
// Instantiate a Transposer that can transpose from one octave down to one
// octave up
Transposer<-12, +12> transposer;
// Instantiate a Selector to change the transposition
IncrementDecrementSelector<transposer.getNumberOfBanks()> selector = {
  transposer,
  {11, 12},
  Wrap::Clamp,
};
 
// Instantiate an array of NoteButton objects
Bankable::NoteButton buttons[] = {
  {transposer, 2, {MIDI_Notes::C[2], Channel_16}},
};

CCAbsoluteEncoder encBombo = {
  {9, 10},       // pins
  {10, Channel_16, Cable_1}, // MIDI address (CC number + optional channel)
  1,            // optional multiplier if the control isn't fast enough
};
 
void setup() {
  Control_Surface.begin(); // Initialize Control Surface
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("JAIVA");
  lcd.setCursor(0, 1);
  lcd.print("BO:");
  lcd.setCursor(4, 1);
  lcd.print("01");

}

int previusTransposePos = 0;

void loop() {
  bomboModule();
  Control_Surface.loop(); // Update the Control Surface
}


void bomboModule(){

 int transposePos = transposer.getTranspositionSemitones();
  int mappedTransposerBomboPos = map(transposePos,-12,12,1,25);
  encBombo.setAddress(mappedTransposerBomboPos);
  int currentTransposePos = mappedTransposerBomboPos;
  // logic bridge
  if (currentTransposePos != previusTransposePos) {
    previusTransposePos = currentTransposePos;
    lcd.setCursor(4, 1);
    if (previusTransposePos < 10) {
      lcd.print("0" + String(previusTransposePos));
    } else {
      lcd.print(String(previusTransposePos));
    }
    
  }

  Serial.println(mappedTransposerBomboPos);
  
}