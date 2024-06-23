
#include <Control_Surface.h> // Include the Control Surface library

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
 
// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;
using namespace MIDI_Notes;

//*************************************************************************
// Conectar encoder a pines (9 y 10) para controlar pitch
CCAbsoluteEncoder encBombo = {
  {9, 10},       // pins
  {10, Channel_16, Cable_1}, // MIDI address (CC number + optional channel)
  1,            // optional multiplier if the control isn't fast enough
};
//*************************************************************************

class EncoderValueDataLogger {
  private:
      int storedValue;

  public:
      EncoderValueDataLogger() {
          storedValue = 0;
      }

      void updateValue(int newValue) {
          storedValue = newValue;
      }

      int getValue() {
          return storedValue;
      }
};
//*************************************************************************
// Conectar botones a pines (11 y 12) para controlar 2 octavas
// Instantiate a Transposer that can transpose from one octave down to one
// octave up
Transposer<-12, +12> transposer;
// Instantiate a Selector to change the transposition
IncrementDecrementSelector<transposer.getNumberOfBanks()> selector = {
  transposer,
  {11, 12},
  Wrap::Clamp,
};
//*************************************************************************
//*************************************************************************
// Conectar nota a pin (2) para controlar ejecución nota 
// Instantiate an array of NoteButton objects
Bankable::NoteButton buttons[] = {
  {transposer, 2, {MIDI_Notes::C[2], Channel_16}},
};


EncoderValueDataLogger BomboPitchEncoderPos1;
EncoderValueDataLogger BomboPitchEncoderPos2;
EncoderValueDataLogger BomboPitchEncoderPos3;
EncoderValueDataLogger BomboPitchEncoderPos4;
EncoderValueDataLogger BomboPitchEncoderPos5;
EncoderValueDataLogger BomboPitchEncoderPos6;
EncoderValueDataLogger BomboPitchEncoderPos7;
EncoderValueDataLogger BomboPitchEncoderPos8;
EncoderValueDataLogger BomboPitchEncoderPos9;
EncoderValueDataLogger BomboPitchEncoderPos10;
EncoderValueDataLogger BomboPitchEncoderPos11;
EncoderValueDataLogger BomboPitchEncoderPos12;
EncoderValueDataLogger BomboPitchEncoderPos13;
EncoderValueDataLogger BomboPitchEncoderPos14;
EncoderValueDataLogger BomboPitchEncoderPos15;
EncoderValueDataLogger BomboPitchEncoderPos16;
EncoderValueDataLogger BomboPitchEncoderPos17;
EncoderValueDataLogger BomboPitchEncoderPos18;
EncoderValueDataLogger BomboPitchEncoderPos19;
EncoderValueDataLogger BomboPitchEncoderPos20;
EncoderValueDataLogger BomboPitchEncoderPos21;
EncoderValueDataLogger BomboPitchEncoderPos22;
EncoderValueDataLogger BomboPitchEncoderPos23;
EncoderValueDataLogger BomboPitchEncoderPos24;
EncoderValueDataLogger BomboPitchEncoderPos25;
 
void setup() {
  Control_Surface.begin(); // Initialize Control Surface

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

int BomboPitchPreviousTransposePos = 0;

void loop() {
  bomboModule();
  Control_Surface.loop(); // Update the Control Surface
}

void BomboPitchResetLcdScreen() {
  lcd.setCursor(13, 1);
  lcd.print("000");
}

int BomboPitchPreviousEncoderValue = 0;

void bomboModule(){
 int transposePos = transposer.getTranspositionSemitones();
  int mappedTransposerBomboPos = map(transposePos,-12,12,1,25);
  encBombo.setAddress(mappedTransposerBomboPos);

  // Serial.println(encBombo.getValue());
  //*****************************************************************
  // This block arranges the zero on the left of the pitch indicator
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
  
  int BomboCurrentTransposePos = mappedTransposerBomboPos;

  // logic bridge to set the new encoder value
  // only when the transpose value changes
  if (BomboCurrentTransposePos != BomboPitchPreviousTransposePos) {
    BomboPitchPreviousTransposePos = BomboCurrentTransposePos;

    switch (BomboPitchPreviousTransposePos) {
      case 1:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos1.getValue());
        break;

      case 2:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos2.getValue());
        break;

      case 3:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos3.getValue());
        break;

      case 4:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos4.getValue());
        break;

      case 5:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos5.getValue());
        break;

      case 6:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos6.getValue());
        break;

      case 7:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos7.getValue());
        break;

      case 8:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos8.getValue());
        break;

      case 9:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos9.getValue());
        break;

      case 10:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos10.getValue());
        break;

      case 11:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos11.getValue());
        break;

      case 12:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos12.getValue());
        break;

      case 13:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos13.getValue());
        break;
        
      case 14:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos14.getValue());
        break;

      case 15:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos15.getValue());
        break;

      case 16:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos16.getValue());
        break;

      case 17:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos17.getValue());
        break;

      case 18:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos18.getValue());
        break;   

      case 19:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos19.getValue());
        break;  

      case 20:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos20.getValue());
        break; 

      case 21:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos21.getValue());
        break;   

      case 22:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos22.getValue());
        break;   

      case 23:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos23.getValue());
        break;   

      case 24:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos24.getValue());
        break;   

      case 25:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos25.getValue());
        break;      
    }

    lcd.setCursor(4, 1);
    if (BomboPitchPreviousTransposePos < 10) {
      lcd.print("0" + String(BomboPitchPreviousTransposePos));
    } else {
      lcd.print(String(BomboPitchPreviousTransposePos));
    }
    
  }

  int CurrentBomboPitchRotaryValue = encBombo.getValue();

  // logic bridge to update the new encoder value only when 
  // the encodervalue changes.
  if (CurrentBomboPitchRotaryValue != BomboPitchPreviousEncoderValue) {

    BomboPitchPreviousEncoderValue = CurrentBomboPitchRotaryValue;
    switch (BomboCurrentTransposePos) {
      case 1:
        BomboPitchEncoderPos1.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 2:
        BomboPitchEncoderPos2.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 3:
        BomboPitchEncoderPos3.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 4:
        BomboPitchEncoderPos4.updateValue(CurrentBomboPitchRotaryValue);
        break;
        
      case 5:
        BomboPitchEncoderPos5.updateValue(CurrentBomboPitchRotaryValue);
        break;
        
      case 6:
        BomboPitchEncoderPos6.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 7:
        BomboPitchEncoderPos7.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 8:
        BomboPitchEncoderPos8.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 9:
        BomboPitchEncoderPos9.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 10:
        BomboPitchEncoderPos10.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 11:
        BomboPitchEncoderPos11.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 12:
        BomboPitchEncoderPos12.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 13:
        BomboPitchEncoderPos13.updateValue(CurrentBomboPitchRotaryValue);
        break;
        
      case 14:
        BomboPitchEncoderPos14.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 15:
        BomboPitchEncoderPos15.updateValue(CurrentBomboPitchRotaryValue);
        break;
    
      case 16:
        BomboPitchEncoderPos16.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 17:
        BomboPitchEncoderPos17.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 18:
        BomboPitchEncoderPos18.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 19:
        BomboPitchEncoderPos19.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 20:
        BomboPitchEncoderPos20.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 21:
        BomboPitchEncoderPos21.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 22:
        BomboPitchEncoderPos22.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 23:
        BomboPitchEncoderPos23.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 24:
        BomboPitchEncoderPos24.updateValue(CurrentBomboPitchRotaryValue);
        break;

      case 25:
        BomboPitchEncoderPos25.updateValue(CurrentBomboPitchRotaryValue);
        break;
    }
  }

  
}


