//**********************************************************************)))
// Script para controlar un sensor resistivo para gatillar una nota 
// midi en canal 16 con 2 rotary encoder para controlar
// pitch y volumen de 25 sonidos por medio de CC values
//**********************************************************************)))
#include <Control_Surface.h> // Include the Control Surface library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SD.h>

LiquidCrystal_I2C lcd(0x27,16,2);
 
// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;
using namespace MIDI_Notes;

//**********************************************************************)))
// BOMBO PITCH Encoder: Conectar encoder a pines (9 y 10)
// para controlar pitch
//**********************************************************************)))
CCAbsoluteEncoder encBomboPitch = {
  {9, 10},       // pins
  {10, Channel_16, Cable_1}, // MIDI address (CC number + optional channel)
  1,            // optional multiplier if the control isn't fast enough
};
//**********************************************************************END

//**********************************************************************)))
// BOMBO VOL Encoder: Conectar encoder a pines (9 y 10)
// para controlar volumne
//**********************************************************************)))
CCAbsoluteEncoder encBomboVol = {
  {26, 27},       // pins
  {10, Channel_16, Cable_1}, // MIDI address (CC number + optional channel)
  1,            // optional multiplier if the control isn't fast enough
};
//**********************************************************************END

//**********************************************************************)))
// Conectar botones a pines (11 y 12) para controlar 2 octavas
// Instantiate a Transposer that can transpose from one octave down to one
// octave up
//**********************************************************************)))
Transposer<-12, +12> transposer;
// Instantiate a Selector to change the transposition
IncrementDecrementSelector<transposer.getNumberOfBanks()> selector = {
  transposer,
  {11, 12},
  Wrap::Clamp,
};
//**********************************************************************END

//**********************************************************************)))
// Conectar nota a pin (2) para controlar ejecución nota 
// Instantiate an array of NoteButton objects
//**********************************************************************)))
Bankable::NoteButton buttons[] = {
  {transposer, 2, {MIDI_Notes::C[2], Channel_16}},
};
//**********************************************************************END

//**********************************************************************)))
// Generic Data logger section
//**********************************************************************)))
void initializeLogFile(String fileName) {
  File dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  if (dataFile) {
    // Check if the specific lines exist
    bool linesExist = false;
    for (int i = 1; i <= 25; ++i) {
        String lineToCheck = "0";
        if (fileContainsLine(dataFile, lineToCheck)) {
            linesExist = true;
            break;
        }
    }

    if (!linesExist) {
        // Delete existing content
        dataFile.seek(0);
        dataFile.truncate(0);

        // Write initial lines with placeholders
        dataFile.println("0");
        for (int i = 2; i <= 25; ++i) {
          dataFile.println("0");
        }
        Serial.println("Log file init successfully.");
    } else {
        Serial.println("Log file already contains relevant lines.");
    }
    dataFile.close();
  } else {
      Serial.println("Error opening file for initialization.");
  }
}

bool fileContainsLine(File& file, const String& targetLine) {
  file.seek(0); // Start reading from the beginning
  while (file.available()) {
      String line = file.readStringUntil('\n');
      if (line.substring(0, 0) == targetLine.substring(0, 0)) {
          Serial.println(line.substring(0, 0) + " / Value placeholder detected");
          return true;
      }
  }
  return false;
}

void dataLoggerUpdater(int linePos, int encValue, String file) {

    // Open the data_log.txt file
    File dataLogFile = SD.open(file.c_str(), FILE_READ);
    if (!dataLogFile) {
        Serial.println("Error opening " + file);
        return;
    }

    // Read all lines and store them in a vector
    // using trim to extract the "\n" new line part
    std::vector<String> lines;
    while (dataLogFile.available()) {
        lines.push_back(dataLogFile.readStringUntil('\n').trim()); 
    }
    dataLogFile.close();

    // Update the specified line
    if (linePos - 1 >= 0 && linePos -1 < lines.size()) {
        String& line = lines[linePos -1 ];
        if (line.trim() != "") {
            String numericValueStr = line.trim();
            int numericValue = numericValueStr.toInt();
            numericValue = encValue;
            line = String(numericValue);
        } else {
            Serial.println("No record found in the line");
            return;
        }
    } else {
        Serial.println("Line position out of range");
        return;
    }

    // Open the file again for writing
    dataLogFile = SD.open(file.c_str(), FILE_WRITE);
    if (!dataLogFile) {
        Serial.println("Error opening " + file + " for writing");
        return;
    }

    // Delete existing content
    dataLogFile.seek(0);
    dataLogFile.truncate(0);

    // Write all updated lines back to the file
    for (const String& updatedLine : lines) {
        dataLogFile.println(updatedLine);
    } 
    dataLogFile.close();

    Serial.println("Data updated successfully!");
}

int dataLoggerRead(int linePos, String file){
    int numericValue = 0;
    // Open the data_log.txt file
    File dataLogFile = SD.open(file.c_str(), FILE_READ);
    if (!dataLogFile) {
        Serial.println("Error opening " + file);
        return;
    }

    std::vector<String> lines;
    while (dataLogFile.available()) {
        lines.push_back(dataLogFile.readStringUntil('\n').trim()); 
    }
    dataLogFile.close();

    // Update the specified line
    if (linePos - 1 >= 0 && linePos -1 < lines.size()) {
        String& line = lines[linePos -1 ];
        if (line.trim() != "") {
            String numericValueStr = line.trim();
            numericValue = numericValueStr.toInt();
        } else {
            Serial.println("No record found in the line");
            return numericValue;
        }
    } else {
        Serial.println("Line position out of range");
        return numericValue;
    }

    return numericValue;
}
//**********************************************************************END

//**********************************************************************)))
// Generic Encoder data logger Value holder
//**********************************************************************)))
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

      int getValue(int linePos, String file) {
        dataLoggerUpdater(linePos, storedValue, file);
        return storedValue;
      }

      void updateStoredValue(int linePos, String file){
        storedValue = dataLoggerRead(linePos, file);
      }
};
//**********************************************************************END

//**********************************************************************)))
// Initialize EncoderValueDataLogger class Encoder value holder
//**********************************************************************)))
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

EncoderValueDataLogger BomboVolEncoderPos1;
EncoderValueDataLogger BomboVolEncoderPos2;
EncoderValueDataLogger BomboVolEncoderPos3;
EncoderValueDataLogger BomboVolEncoderPos4;
EncoderValueDataLogger BomboVolEncoderPos5;
EncoderValueDataLogger BomboVolEncoderPos6;
EncoderValueDataLogger BomboVolEncoderPos7;
EncoderValueDataLogger BomboVolEncoderPos8;
EncoderValueDataLogger BomboVolEncoderPos9;
EncoderValueDataLogger BomboVolEncoderPos10;
EncoderValueDataLogger BomboVolEncoderPos11;
EncoderValueDataLogger BomboVolEncoderPos12;
EncoderValueDataLogger BomboVolEncoderPos13;
EncoderValueDataLogger BomboVolEncoderPos14;
EncoderValueDataLogger BomboVolEncoderPos15;
EncoderValueDataLogger BomboVolEncoderPos16;
EncoderValueDataLogger BomboVolEncoderPos17;
EncoderValueDataLogger BomboVolEncoderPos18;
EncoderValueDataLogger BomboVolEncoderPos19;
EncoderValueDataLogger BomboVolEncoderPos20;
EncoderValueDataLogger BomboVolEncoderPos21;
EncoderValueDataLogger BomboVolEncoderPos22;
EncoderValueDataLogger BomboVolEncoderPos23;
EncoderValueDataLogger BomboVolEncoderPos24;
EncoderValueDataLogger BomboVolEncoderPos25;
//**********************************************************************END

//**********************************************************************)))
// SETUP ---------------------------------------------------------------
//**********************************************************************)))
void setup() {
  Serial.begin(115200);
  Control_Surface.begin(); // Initialize Control Surface
  
  lcd.init();
  lcd.backlight();
  lcd.clear();

    // Initialize SD card
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("SD card ini failed!");
    return;
  }
  Serial.println("SD card OK!");

  initializeLogFile("BomboPitchDataLog.txt");
  initializeLogFile("BomboVolDataLog.txt");

  updateStoredBomboPitchValues();
  updateStoredBomboVolValues();

  lcd.setCursor(0, 0);
  lcd.print("JAIVA");
  lcd.setCursor(0, 1);
  lcd.print("BO:");
  lcd.setCursor(4, 1);
  lcd.print("01");
  lcd.setCursor(8, 0);
  lcd.print("Vol:");
  lcd.setCursor(13, 0);
  lcd.print("000");
  lcd.setCursor(8, 1);
  lcd.print("Pch:");
  lcd.setCursor(13, 1);
  lcd.print("000");
}
//**********************************************************************END

//**********************************************************************)))
// LOOP ---------------------------------------------------------------
//**********************************************************************)))
void loop() {
  bomboModule();
  Control_Surface.loop(); // Update the Control Surface
}
//**********************************************************************END

void BomboCcValResetLcdScreen() {
  lcd.setCursor(13, 0);
  lcd.print("000");
  lcd.setCursor(13, 1);
  lcd.print("000");
}

//*****************************************************************
// This block arranges the zero on the left of the pitch indicator
//*****************************************************************
void CCValueZeroArranger(int lcdCol, int lcdRow, int ccVal) {
  lcd.setCursor(lcdCol, lcdRow);
  if (ccVal < 10) {
    lcd.print("00"+ String(ccVal));
  }
  else if (ccVal >= 10 and ccVal < 100){
    lcd.print("0"+ String(ccVal));
  }
  else {
    lcd.print(String(ccVal));
  }
}
//*************************************************************END

int BomboPitchPreviousEncoderValue = 0;
int BomboVolPreviousEncoderValue = 0;
int BomboPreviousTransposePos = 0;

//**********************************************************************)))
// bomboModule - Updates and stores current Bombo encoder values
//**********************************************************************)))
void bomboModule(){
  int transposePos = transposer.getTranspositionSemitones();
  int mappedTransposerBomboPos = map(transposePos,-12,12,1,25);
  encBomboPitch.setAddress(mappedTransposerBomboPos);
  encBomboVol.setAddress(mappedTransposerBomboPos + 25);
  
  CCValueZeroArranger(13, 0, encBomboVol.getValue());
  CCValueZeroArranger(13, 1, encBomboPitch.getValue());

  int BomboCurrentTransposePos = mappedTransposerBomboPos;

  // logic bridge to set the new encoder value
  // only when the transpose value changes
  if (BomboCurrentTransposePos != BomboPreviousTransposePos) {
    BomboPreviousTransposePos = BomboCurrentTransposePos;

    switch (BomboPreviousTransposePos) {
      case 1:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos1.getValue(1, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos1.getValue(1, "BomboVolDataLog.txt"));
        break;

      case 2:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos2.getValue(2, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos2.getValue(2, "BomboVolDataLog.txt"));
        break;

      case 3:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos3.getValue(3, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos3.getValue(3, "BomboVolDataLog.txt"));
        break;

      case 4:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos4.getValue(4, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos4.getValue(4, "BomboVolDataLog.txt"));
        break;

      case 5:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos5.getValue(5, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos5.getValue(5, "BomboVolDataLog.txt"));
        break;

      case 6:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos6.getValue(6, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos6.getValue(6, "BomboVolDataLog.txt"));
        break;

      case 7:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos7.getValue(7, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos7.getValue(7, "BomboVolDataLog.txt"));
        break;

      case 8:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos8.getValue(8, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos8.getValue(8, "BomboVolDataLog.txt"));
        break;

      case 9:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos9.getValue(9, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos9.getValue(9, "BomboVolDataLog.txt"));
        break;

      case 10:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos10.getValue(10, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos10.getValue(10, "BomboVolDataLog.txt"));
        break;

      case 11:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos11.getValue(11, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos11.getValue(11, "BomboVolDataLog.txt"));
        break;

      case 12:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos12.getValue(12, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos12.getValue(12, "BomboVolDataLog.txt"));
        break;

      case 13:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos13.getValue(13, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos13.getValue(13, "BomboVolDataLog.txt"));
        break;
        
      case 14:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos14.getValue(14, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos14.getValue(14, "BomboVolDataLog.txt"));
        break;

      case 15:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos15.getValue(15, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos15.getValue(15, "BomboVolDataLog.txt"));
        break;

      case 16:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos16.getValue(16, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos16.getValue(16, "BomboVolDataLog.txt"));
        break;

      case 17:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos17.getValue(17, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos17.getValue(17, "BomboVolDataLog.txt"));
        break;

      case 18:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos18.getValue(18, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos18.getValue(18, "BomboVolDataLog.txt"));
        break;   

      case 19:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos19.getValue(19, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos19.getValue(19, "BomboVolDataLog.txt"));
        break;  

      case 20:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos20.getValue(20, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos20.getValue(20, "BomboVolDataLog.txt"));
        break; 

      case 21:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos21.getValue(21, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos21.getValue(21, "BomboVolDataLog.txt"));
        break;   

      case 22:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos22.getValue(22, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos22.getValue(22, "BomboVolDataLog.txt"));
        break;   

      case 23:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos23.getValue(23, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos23.getValue(23, "BomboVolDataLog.txt"));
        break;   

      case 24:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos24.getValue(24, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos24.getValue(24, "BomboVolDataLog.txt"));
        break;   

      case 25:
        BomboCcValResetLcdScreen();
        encBomboPitch.setValue(BomboPitchEncoderPos25.getValue(25, "BomboPitchDataLog.txt"));
        encBomboVol.setValue(BomboVolEncoderPos25.getValue(25, "BomboVolDataLog.txt"));
        break;      
    }

    lcd.setCursor(4, 1);
    if (BomboPreviousTransposePos < 10) {
      lcd.print("0" + String(BomboPreviousTransposePos));
    } else {
      lcd.print(String(BomboPreviousTransposePos));
    }
  }

  int CurrentBomboPitchRotaryValue = encBomboPitch.getValue();
  int CurrentBomboVolRotaryValue = encBomboVol.getValue();

  // logic bridge to update the new encoder value only when 
  // the encodervalue changes.
  if (CurrentBomboPitchRotaryValue != BomboPitchPreviousEncoderValue || 
                  CurrentBomboVolRotaryValue != BomboVolPreviousEncoderValue) {

    BomboPitchPreviousEncoderValue = CurrentBomboPitchRotaryValue;
    BomboVolPreviousEncoderValue = CurrentBomboVolRotaryValue;

    switch (BomboCurrentTransposePos) {
      case 1:
        BomboPitchEncoderPos1.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos1.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 2:
        BomboPitchEncoderPos2.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos2.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 3:
        BomboPitchEncoderPos3.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos3.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 4:
        BomboPitchEncoderPos4.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos4.updateValue(CurrentBomboVolRotaryValue);
        break;
        
      case 5:
        BomboPitchEncoderPos5.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos5.updateValue(CurrentBomboVolRotaryValue);
        break;
        
      case 6:
        BomboPitchEncoderPos6.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos6.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 7:
        BomboPitchEncoderPos7.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos7.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 8:
        BomboPitchEncoderPos8.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos8.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 9:
        BomboPitchEncoderPos9.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos9.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 10:
        BomboPitchEncoderPos10.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos10.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 11:
        BomboPitchEncoderPos11.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos11.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 12:
        BomboPitchEncoderPos12.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos12.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 13:
        BomboPitchEncoderPos13.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos13.updateValue(CurrentBomboVolRotaryValue);
        break;
        
      case 14:
        BomboPitchEncoderPos14.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos14.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 15:
        BomboPitchEncoderPos15.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos15.updateValue(CurrentBomboVolRotaryValue);
        break;
    
      case 16:
        BomboPitchEncoderPos16.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos16.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 17:
        BomboPitchEncoderPos17.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos17.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 18:
        BomboPitchEncoderPos18.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos18.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 19:
        BomboPitchEncoderPos19.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos19.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 20:
        BomboPitchEncoderPos20.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos20.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 21:
        BomboPitchEncoderPos21.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos21.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 22:
        BomboPitchEncoderPos22.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos22.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 23:
        BomboPitchEncoderPos23.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos23.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 24:
        BomboPitchEncoderPos24.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos24.updateValue(CurrentBomboVolRotaryValue);
        break;

      case 25:
        BomboPitchEncoderPos25.updateValue(CurrentBomboPitchRotaryValue);
        BomboVolEncoderPos25.updateValue(CurrentBomboVolRotaryValue);
        break;
    }
  }
}
//**********************************************************************END

//**********************************************************************)))
// Update Bombo pitch encoder pos at start from sd card
//**********************************************************************)))
void updateStoredBomboPitchValues() {
	BomboPitchEncoderPos1.updateStoredValue(1, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos2.updateStoredValue(2, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos3.updateStoredValue(3, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos4.updateStoredValue(4, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos5.updateStoredValue(5, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos6.updateStoredValue(6, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos7.updateStoredValue(7, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos8.updateStoredValue(8, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos9.updateStoredValue(9, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos10.updateStoredValue(10, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos11.updateStoredValue(11, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos12.updateStoredValue(12, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos13.updateStoredValue(13, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos14.updateStoredValue(14, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos15.updateStoredValue(15, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos16.updateStoredValue(16, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos17.updateStoredValue(17, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos18.updateStoredValue(18, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos19.updateStoredValue(19, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos20.updateStoredValue(20, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos21.updateStoredValue(21, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos22.updateStoredValue(22, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos23.updateStoredValue(23, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos24.updateStoredValue(24, "BomboPitchDataLog.txt");
	BomboPitchEncoderPos25.updateStoredValue(25, "BomboPitchDataLog.txt");
}
//**********************************************************************END

//**********************************************************************)))
// Update Bombo volume encoder pos at start from sd card
//**********************************************************************)))
void updateStoredBomboVolValues() {
	BomboVolEncoderPos1.updateStoredValue(1, "BomboVolDataLog.txt");
	BomboVolEncoderPos2.updateStoredValue(2, "BomboVolDataLog.txt");
	BomboVolEncoderPos3.updateStoredValue(3, "BomboVolDataLog.txt");
	BomboVolEncoderPos4.updateStoredValue(4, "BomboVolDataLog.txt");
	BomboVolEncoderPos5.updateStoredValue(5, "BomboVolDataLog.txt");
	BomboVolEncoderPos6.updateStoredValue(6, "BomboVolDataLog.txt");
	BomboVolEncoderPos7.updateStoredValue(7, "BomboVolDataLog.txt");
	BomboVolEncoderPos8.updateStoredValue(8, "BomboVolDataLog.txt");
	BomboVolEncoderPos9.updateStoredValue(9, "BomboVolDataLog.txt");
	BomboVolEncoderPos10.updateStoredValue(10, "BomboVolDataLog.txt");
	BomboVolEncoderPos11.updateStoredValue(11, "BomboVolDataLog.txt");
	BomboVolEncoderPos12.updateStoredValue(12, "BomboVolDataLog.txt");
	BomboVolEncoderPos13.updateStoredValue(13, "BomboVolDataLog.txt");
	BomboVolEncoderPos14.updateStoredValue(14, "BomboVolDataLog.txt");
	BomboVolEncoderPos15.updateStoredValue(15, "BomboVolDataLog.txt");
	BomboVolEncoderPos16.updateStoredValue(16, "BomboVolDataLog.txt");
	BomboVolEncoderPos17.updateStoredValue(17, "BomboVolDataLog.txt");
	BomboVolEncoderPos18.updateStoredValue(18, "BomboVolDataLog.txt");
	BomboVolEncoderPos19.updateStoredValue(19, "BomboVolDataLog.txt");
	BomboVolEncoderPos20.updateStoredValue(20, "BomboVolDataLog.txt");
	BomboVolEncoderPos21.updateStoredValue(21, "BomboVolDataLog.txt");
	BomboVolEncoderPos22.updateStoredValue(22, "BomboVolDataLog.txt");
	BomboVolEncoderPos23.updateStoredValue(23, "BomboVolDataLog.txt");
	BomboVolEncoderPos24.updateStoredValue(24, "BomboVolDataLog.txt");
	BomboVolEncoderPos25.updateStoredValue(25, "BomboVolDataLog.txt");
}
//**********************************************************************END


