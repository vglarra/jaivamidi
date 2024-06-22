//************************************************************************
// Permite encender y apaga 16 leds usando MCP23017
//************************************************************************
#include <Adafruit_MCP23X17.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  

Adafruit_MCP23X17 mcpOUT;
Adafruit_MCP23X17 mcpIN;

int ledsOffCounter = 0;

void setup() {
  Wire.begin();

  Serial.begin(9600);

  if (!mcpOUT.begin_I2C(0x20)) {
      Serial.println("Error initializing MCP23017-1");
      while (1);
  }

  if (!mcpIN.begin_I2C(0x21)) {
      Serial.println("Error initializing MCP23017-2");
      while (1);
  }
  //**********(MCP23X17)****************
  mcpOUT.begin_I2C(0x20);
  mcpIN.begin_I2C(0x21);
  //*******************************
  //**********(LED)*********************
  mcpIN.pinMode(0, INPUT_PULLUP);
  mcpIN.pinMode(1, INPUT_PULLUP);
  mcpIN.pinMode(2, INPUT_PULLUP); 
  mcpIN.pinMode(3, INPUT_PULLUP);
  mcpIN.pinMode(4, INPUT_PULLUP);
  mcpIN.pinMode(5, INPUT_PULLUP); 
  mcpIN.pinMode(6, INPUT_PULLUP);
  mcpIN.pinMode(7, INPUT_PULLUP);
  mcpIN.pinMode(8, INPUT_PULLUP); 
  mcpIN.pinMode(9, INPUT_PULLUP);
  mcpIN.pinMode(10, INPUT_PULLUP);
  mcpIN.pinMode(11, INPUT_PULLUP); 
  mcpIN.pinMode(12, INPUT_PULLUP); 
  mcpIN.pinMode(13, INPUT_PULLUP);
  mcpIN.pinMode(14, INPUT_PULLUP);
  mcpIN.pinMode(15, INPUT_PULLUP); 
  //*******************************
  //**********(Switch)******************
  mcpOUT.pinMode(0, OUTPUT);
  mcpOUT.pinMode(1, OUTPUT);
  mcpOUT.pinMode(2, OUTPUT);
  mcpOUT.pinMode(3, OUTPUT);
  mcpOUT.pinMode(4, OUTPUT);
  mcpOUT.pinMode(5, OUTPUT);
  mcpOUT.pinMode(6, OUTPUT);
  mcpOUT.pinMode(7, OUTPUT);
  mcpOUT.pinMode(8, OUTPUT);
  mcpOUT.pinMode(9, OUTPUT);
  mcpOUT.pinMode(10, OUTPUT);
  mcpOUT.pinMode(11, OUTPUT);
  mcpOUT.pinMode(12, OUTPUT);
  mcpOUT.pinMode(13, OUTPUT);
  mcpOUT.pinMode(14, OUTPUT);
  mcpOUT.pinMode(15, OUTPUT);
  //*******************************

  LedsOff();

  //**********(LCD Screen init)******************
  lcd.init();
  lcd.backlight();
  lcd.clear();
//*******************************

	// Turn on the blacklight and print a message.
  lcd.setCursor(0, 0);
  lcd.print("JAIVA");
  lcd.setCursor(0, 1);
  lcd.print("CH:");
  lcd.setCursor(4, 1);
  lcd.print("01");

  lcd.setCursor(8, 1);
  lcd.print("BK:");
  lcd.setCursor(12, 1);
  lcd.print("0");

  lcd.setCursor(8, 0);
  lcd.print("CC:");
  lcd.setCursor(12, 0);
  lcd.print("01");
  
}

int ledOnOffCounter = 0;

void loop() {

  if (ledOnOffCounter < 16){

    if (mcpIN.digitalRead(ledOnOffCounter)==LOW) {
      switch (ledOnOffCounter) {
        case 0:
          lightOnLed(ledOnOffCounter);
          break;

        case 1:
          lightOnLed(ledOnOffCounter);
          break;

        case 2:
          lightOnLed(ledOnOffCounter);
          break;

        case 3:
          lightOnLed(ledOnOffCounter);
          break;

        case 4:
          lightOnLed(ledOnOffCounter);
          break;
          
        case 5:
          lightOnLed(ledOnOffCounter);
          break;

        case 6:
          lightOnLed(ledOnOffCounter);
          break;
          
        case 7:
          lightOnLed(ledOnOffCounter);
          break;

        case 8:
          lightOnLed(ledOnOffCounter);
          break;
          
        case 9:
          lightOnLed(ledOnOffCounter);
          break;

        case 10:
          lightOnLed(ledOnOffCounter);
          break;
          
        case 11:
          lightOnLed(ledOnOffCounter);
          break;

        case 12:
          lightOnLed(ledOnOffCounter);
          break;

        case 13:
          lightOnLed(ledOnOffCounter);
          break;
          
        case 14:
          lightOnLed(ledOnOffCounter);
          break;

        case 15:
          lightOnLed(ledOnOffCounter);
          break;
      }
    }
  }
  else {
    ledOnOffCounter = -1;
  }
  ledOnOffCounter++;
}

void LedsOff() {
  while (ledsOffCounter < 16){
    mcpOUT.digitalWrite(ledsOffCounter,LOW);
    ledsOffCounter++;
  }
  ledsOffCounter = 0;
  mcpOUT.digitalWrite(0,HIGH);
}

void lightOnLed(int ledPos) {
  mcpOUT.digitalWrite(ledPos,HIGH);
  for (int i = 0; i < 16; i++){
    if (i != ledPos){
      mcpOUT.digitalWrite(i,LOW);
    }
  }
}