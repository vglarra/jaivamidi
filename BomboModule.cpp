
#include <Control_Surface.h> // Include the Control Surface library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SD.h>

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

  updateStoredBomboPitchValues();

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
        encBombo.setValue(BomboPitchEncoderPos1.getValue(1, "BomboPitchDataLog.txt"));
        break;

      case 2:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos2.getValue(2, "BomboPitchDataLog.txt"));
        break;

      case 3:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos3.getValue(3, "BomboPitchDataLog.txt"));
        break;

      case 4:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos4.getValue(4, "BomboPitchDataLog.txt"));
        break;

      case 5:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos5.getValue(5, "BomboPitchDataLog.txt"));
        break;

      case 6:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos6.getValue(6, "BomboPitchDataLog.txt"));
        break;

      case 7:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos7.getValue(7, "BomboPitchDataLog.txt"));
        break;

      case 8:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos8.getValue(8, "BomboPitchDataLog.txt"));
        break;

      case 9:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos9.getValue(9, "BomboPitchDataLog.txt"));
        break;

      case 10:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos10.getValue(10, "BomboPitchDataLog.txt"));
        break;

      case 11:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos11.getValue(11, "BomboPitchDataLog.txt"));
        break;

      case 12:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos12.getValue(12, "BomboPitchDataLog.txt"));
        break;

      case 13:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos13.getValue(13, "BomboPitchDataLog.txt"));
        break;
        
      case 14:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos14.getValue(14, "BomboPitchDataLog.txt"));
        break;

      case 15:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos15.getValue(15, "BomboPitchDataLog.txt"));
        break;

      case 16:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos16.getValue(16, "BomboPitchDataLog.txt"));
        break;

      case 17:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos17.getValue(17, "BomboPitchDataLog.txt"));
        break;

      case 18:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos18.getValue(18, "BomboPitchDataLog.txt"));
        break;   

      case 19:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos19.getValue(19, "BomboPitchDataLog.txt"));
        break;  

      case 20:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos20.getValue(20, "BomboPitchDataLog.txt"));
        break; 

      case 21:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos21.getValue(21, "BomboPitchDataLog.txt"));
        break;   

      case 22:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos22.getValue(22, "BomboPitchDataLog.txt"));
        break;   

      case 23:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos23.getValue(23, "BomboPitchDataLog.txt"));
        break;   

      case 24:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos24.getValue(24, "BomboPitchDataLog.txt"));
        break;   

      case 25:
        BomboPitchResetLcdScreen();
        encBombo.setValue(BomboPitchEncoderPos25.getValue(25, "BomboPitchDataLog.txt"));
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


