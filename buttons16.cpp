
#include <Adafruit_MCP23X17.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  

Adafruit_MCP23X17 mcpOUT;
Adafruit_MCP23X17 mcpIN;

const int buttonPin0 = 0; // Change this to your actual button pin
const int buttonPin1 = 1; // Change this to your actual button pin
const int buttonPin2 = 2;

const int led0 = 0;
const int led1 = 1; // #define LED_PIN 10     // MCP23XXX pin LED is attached to
const int led2 = 2;



int cnt = 0;


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

  mcpOUT.begin_I2C(0x20);
  mcpIN.begin_I2C(0x21);

  mcpIN.pinMode(0, INPUT_PULLUP);
  mcpIN.pinMode(1, INPUT_PULLUP);
  mcpIN.pinMode(2, INPUT_PULLUP); 
  
  mcpOUT.pinMode(0, OUTPUT);
  mcpOUT.pinMode(1, OUTPUT);
  mcpOUT.pinMode(2, OUTPUT);

  LedsOff();

  lcd.init();
  lcd.backlight();
  lcd.clear();

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

int cnt1 = 0;
int buttonState = 0; // Initial button state (not pressed)
bool ledState = false;    // Initial LED state (off)

void loop() {

  if (cnt1 < 16){

    if (mcpIN.digitalRead(cnt1)==LOW) {
      switch (cnt1) {
        case 0:
          Serial.println("boton_0");
          mcpOUT.digitalWrite(0,HIGH);
          mcpOUT.digitalWrite(1,LOW);
          mcpOUT.digitalWrite(2,LOW);
          break;

        case 1:
          Serial.println("boton_1");
          mcpOUT.digitalWrite(0,LOW);
          mcpOUT.digitalWrite(1,HIGH);
          mcpOUT.digitalWrite(2,LOW);
          break;

        case 2:
          Serial.println("boton_2");
          mcpOUT.digitalWrite(0,LOW);
          mcpOUT.digitalWrite(1,LOW);
          mcpOUT.digitalWrite(2,HIGH);
          break;

      }

    }
  }
  else {
    cnt1 = -1;
  }
  cnt1++;
}

void LedsOff() {
  while (cnt < 16){
    mcpOUT.digitalWrite(cnt,LOW);
    cnt++;
  }
  cnt = 0;
  mcpOUT.digitalWrite(0,HIGH);
}