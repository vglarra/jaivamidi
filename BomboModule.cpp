
#include <Control_Surface.h> // Include the Control Surface library

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
 
// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;
 
using namespace MIDI_Notes;
 
CCAbsoluteEncoder encBombo = {
  {9, 10},       // pins
  {10, Channel_16, Cable_1}, // MIDI address (CC number + optional channel)
  1,            // optional multiplier if the control isn't fast enough
};

class EncoderValues {
  private:
      int storedValue;

  public:
      EncoderValues() {
          storedValue = 0;
      }

      void updateValue(int newValue) {
          storedValue = newValue;
      }

      int getValue() {
          return storedValue;
      }
};

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

EncoderValues EncoderPos8;
EncoderValues EncoderPos9;
EncoderValues EncoderPos10;
EncoderValues EncoderPos11;
EncoderValues EncoderPos12;
EncoderValues EncoderPos13;
EncoderValues EncoderPos14;
EncoderValues EncoderPos15;
EncoderValues EncoderPos16;
EncoderValues EncoderPos17;
EncoderValues EncoderPos18;
 
void setup() {
  Control_Surface.begin(); // Initialize Control Surface
  //Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("JAIVA");
  lcd.setCursor(0, 1);
  lcd.print("BO:");
  lcd.setCursor(4, 1);
  lcd.print("01");
  lcd.setCursor(8, 1);
  lcd.print("Pch:");
  lcd.setCursor(13, 1);
  lcd.print("000");

}

int previusTransposePos = 0;

int previousEncVal8 = 0;
int previousEncVal9 = 0;
int previousEncVal10 = 0;
int previousEncVal11 = 0;
int previousEncVal12 = 0;
int previousEncVal13 = 0;
int previousEncVal14 = 0;
int previousEncVal15 = 0;
int previousEncVal16 = 0;
int previousEncVal17 = 0;
int previousEncVal18 = 0;

void loop() {
  bomboModule();
  Control_Surface.loop(); // Update the Control Surface
}

void bomboModule(){
 int transposePos = transposer.getTranspositionSemitones();
  int mappedTransposerBomboPos = map(transposePos,-12,12,1,25);
  encBombo.setAddress(mappedTransposerBomboPos);

  // Serial.println(encBombo.getValue());
  //*****************************************************************
  // This block arrages the zero on the left of the pitch indicator
  //*****************************************************************
  lcd.setCursor(13, 1);
  int pitchCurrentValue = encBombo.getValue();
  if (pitchCurrentValue < 10) {
    lcd.print("00"+ String(encBombo.getValue()));
  }
  else if (pitchCurrentValue >= 10 and pitchCurrentValue < 100){
    lcd.print("0"+ String(encBombo.getValue()));
  }
  else {
    lcd.print(String(encBombo.getValue()));
  }
  //*****************************************************************
  //*****************************************************************
  
  int currentTransposePos = mappedTransposerBomboPos;

  // logic bridge
  if (currentTransposePos != previusTransposePos) {
    previusTransposePos = currentTransposePos;

    switch (previusTransposePos) {

      case 8:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos8.getValue());
        break;

      case 9:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos9.getValue());
        break;

      case 10:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos10.getValue());
        break;

      case 11:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos11.getValue());
        break;

      case 12:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos12.getValue());
        break;

      case 13:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos13.getValue());
        break;
        
      case 14:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos14.getValue());
        break;

      case 15:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos15.getValue());
        break;

      case 16:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos16.getValue());
        break;

      case 17:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos17.getValue());
        break;

      case 18:
        lcd.setCursor(13, 1);
        lcd.print("000");
        encBombo.setValue(EncoderPos18.getValue());
        break;      
    }

    lcd.setCursor(4, 1);
    if (previusTransposePos < 10) {
      lcd.print("0" + String(previusTransposePos));
    } else {
      lcd.print(String(previusTransposePos));
    }
    
  }

  int currentRotarValue = encBombo.getValue();

  switch (currentTransposePos) {

    case 8:
      EncoderPos8.updateValue(currentRotarValue);
      currentRotarValue 
      break;
    case 9:
      EncoderPos9.updateValue(currentRotarValue);
      break;

    case 10:
      EncoderPos10.updateValue(currentRotarValue);
      break;

    case 11:
      EncoderPos11.updateValue(currentRotarValue);
      break;

    case 12:
      EncoderPos12.updateValue(currentRotarValue);
      break;

    case 13:
      EncoderPos13.updateValue(currentRotarValue);
      break;
      
    case 14:
      EncoderPos14.updateValue(currentRotarValue);
      break;

    case 15:
      EncoderPos15.updateValue(currentRotarValue);
      break;
  
    case 16:
      EncoderPos16.updateValue(currentRotarValue);
      break;

    case 17:
      EncoderPos17.updateValue(currentRotarValue);
      break;
    case 18:
      EncoderPos18.updateValue(currentRotarValue);
      break;
    
  }

  //Serial.println(mappedTransposerBomboPos);
  
}
