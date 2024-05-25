
//*************************************************************************
//Board: teensy 4.1
//Código controla 16 canales midi's con inputs teensy 2,3
//3 bancos con inputs teensy 4,5
//16 CC messages con inputs teensy 8,9
//**************************************

// MIDI Libs
#include <Control_Surface.h>
USBMIDI_Interface midiusb;
HardwareSerialMIDI_Interface midiser {Serial1, MIDI_BAUD};
MIDI_PipeFactory<2> pipes;

//LCD name of display LCD1602 I2C 
//LiquidCrystal_I2C lcd(0x27,16,2);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);     


// Push button connected between pin 2 and ground.
// Note message is sent when pressed.
Button pushbutton1 {2};
Button pushbutton2 {3};
Button pushbutton3 {4};
Button pushbutton4 {5};
Button pushbutton5 {8};
Button pushbutton6 {9};

 
// A unidirectional pipe that connects a MIDI source to a MIDI sink.
// Messages coming from the source are handled by the `mapForwardMIDI()`
// function, where they can be modified or filtered out, and then sent
// to the MIDI sink by calling the `sourceMIDItoSink()` function.
struct MyMIDIFilter : MIDI_Pipe {

  int  starChan;
  int midiTransposeVal;

  // Pass on only Note On/Off messages, changing the channel to 5 and
  // transposing all notes an octave down.
  void mapForwardMIDI(ChannelMessage msg) override {
    switch (msg.getMessageType()) {
      case MIDIMessageType::NoteOff:  // fallthrough
      case MIDIMessageType::NoteOn:
        msg.setChannel(changeCH(starChan));
        msg.data1 += midiTransposeVal;
        sourceMIDItoSink(msg);
        break;
      default: break;
    }
  }
  // Do not pass on System Exclusive messages.
  void mapForwardMIDI(SysExMessage) override {}
  // Do not pass on System Common messages.
  void mapForwardMIDI(SysCommonMessage) override {}
  // Pass on Real-Time messages without changes.
  void mapForwardMIDI(RealTimeMessage msg) override {
    sourceMIDItoSink(msg);
  }

  Channel changeCH(int channel) {
    Channel chan = Channel_1;

    switch (channel){
      case 1:
        chan = Channel_1;
        break;
      case 2:
        chan = Channel_2;
        break;
      case 3:
        chan = Channel_3;
        break;
      case 4:
        chan = Channel_4;
        break;
      case 5:
        chan = Channel_5;
        break;
      case 6:
        chan = Channel_6;
        break;
      case 7:
        chan = Channel_7;
        break;
      case 8:
        chan = Channel_8;
        break;
      case 9:
        chan = Channel_9;
        break;
      case 10:
        chan = Channel_10;
        break;
      case 11:
        chan = Channel_11;
        break;
      case 12:
        chan = Channel_12;
        break;
      case 13:
        chan = Channel_13;
        break;
      case 14:
        chan = Channel_14;
        break;
      case 15:
        chan = Channel_15;
        break;
      case 16:
        chan = Channel_16;
        break;
      default: break;
        
    }
    return chan;
  }

};

CCAbsoluteEncoder enc = {
  {6, 7},       // pins
  {10, Channel_1, Cable_1}, // MIDI address (CC number + optional channel)
  1,            // optional multiplier if the control isn't fast enough
};

int MidiChBtnCount = 1;
int BankBtnCount = 1;
int CcBtnCount = 1;
 
MyMIDIFilter filter;


void setup() {

  filter.starChan = 1;
  filter.midiTransposeVal = -24;
  enc.setAddress(1);

  // Loop back the MIDI input to the MIDI output, through the filter:
  // (MIDI input) -> (MIDI filter) -> (MIDI output)
  //  (sink)      <-    (pipe)     <-     (source)
      midiusb     <<    filter     <<     midiser;
      Control_Surface >> pipes >> midiusb;    // Output to usbmidi
  // midiusb >> pipes >> midiser; // all incoming midi from USB is sent to serial
  // midiusb << filter << midiser; // all incoming midi from Serial is sent to USB
  // midiusb >> pipes >> midiusb; // all incoming midi from USB is looped back
 	// initialize the LCD
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

  pushbutton1.begin();
  pushbutton2.begin();
  pushbutton3.begin();
  pushbutton4.begin();
  pushbutton5.begin();
  pushbutton6.begin();
  midiusb.begin();
  midiser.begin();
  Control_Surface.begin();

}

void loop() {
  
//********************************************************
// Midi Channel
  if (pushbutton1.getState() == Button::Falling){
    MidiChBtnCount++;
    if (MidiChBtnCount < 17){
      filter.starChan = MidiChBtnCount;
      enc.setAddress(changeCCchannel(MidiChBtnCount));
    } 
    else {
      filter.starChan = 1;
      enc.setAddress(Channel_1);
      MidiChBtnCount = 1;
    }
  }    // if the button is pressed

  if (pushbutton2.getState() == Button::Falling){
    MidiChBtnCount--;
    if ((MidiChBtnCount < 17) && (MidiChBtnCount > 0)){
      filter.starChan = MidiChBtnCount;
      enc.setAddress(changeCCchannel(MidiChBtnCount));
    } 
    else {
      filter.starChan = 16;
      enc.setAddress(Channel_16);
      MidiChBtnCount = 16;
    }
  }
//********************************************************
//********************************************************
// Bank Change
  if (pushbutton3.getState() == Button::Falling){
    BankBtnCount ++;
    CcBtnCount = 1;
    enc.setAddress(1);
    if (BankBtnCount >= 4){
      BankBtnCount = 1;
    }
    TransposeX(BankBtnCount);
  }   

  // if the button is pressed
  if (pushbutton4.getState() == Button::Falling){
    BankBtnCount --;
    CcBtnCount = 1;
    enc.setAddress(1);
    if (BankBtnCount <= 0) {
      BankBtnCount = 3;
    }
    TransposeX(BankBtnCount);
  }
//********************************************************

  switch(BankBtnCount) {
  case 1:
    manageCCchange(0);
    break;
  case 2:
    manageCCchange(16);
    break;
  case 3:
    manageCCchange(32);
    break;
  }



  midiusb.update();
  midiser.update();
  pushbutton1.update();
  pushbutton2.update();
  pushbutton3.update();
  pushbutton4.update();
  pushbutton5.update();
  pushbutton6.update();



  // Print the time to the LCD
  lcd.setCursor(4, 1);
  if(MidiChBtnCount < 10){
    lcd.print("0" + String(MidiChBtnCount));
  } 
  else{
    lcd.print(MidiChBtnCount);
  }

  lcd.setCursor(12, 1);
  lcd.print(BankBtnCount);

  lcd.setCursor(12, 0);
  if(CcBtnCount < 10){
    lcd.print("0" + String(CcBtnCount));
  } 
  else{
    lcd.print(CcBtnCount);
  }

  Control_Surface.loop();
}

void TransposeX(int x){

      switch (x){
      case 1:
        filter.midiTransposeVal = -24;
        break;
      case 2:
        filter.midiTransposeVal = 0;
        break;
      case 3:
        filter.midiTransposeVal = 24;
        break;
      default: break;

    }
}
//********************************************************
// CC Change
void manageCCchange(int offset){

  if (pushbutton5.getState() == Button::Falling){
    CcBtnCount++;
    if (CcBtnCount + offset < 17 + offset){
      enc.setAddress(CcBtnCount + offset);
    } 
    else {
       .setAddress(1 + offset);
      CcBtnCount = 1;
    }
  }    // if the button is pressed

  if (pushbutton6.getState() == Button::Falling){
    CcBtnCount--;
    if ((CcBtnCount + offset < 17 + offset) && (CcBtnCount + offset > 0 + offset)){
      enc.setAddress(CcBtnCount + offset);
    } 
    else {
      enc.setAddress(16 + offset);
      CcBtnCount = 16;
    }
  }
}
//********************************************************

Channel changeCCchannel(int channel) {
  Channel chan = Channel_1;

  switch (channel){
    case 1:
      chan = Channel_1;
      break;
    case 2:
      chan = Channel_2;
      break;
    case 3:
      chan = Channel_3;
      break;
    case 4:
      chan = Channel_4;
      break;
    case 5:
      chan = Channel_5;
      break;
    case 6:
      chan = Channel_6;
      break;
    case 7:
      chan = Channel_7;
      break;
    case 8:
      chan = Channel_8;
      break;
    case 9:
      chan = Channel_9;
      break;
    case 10:
      chan = Channel_10;
      break;
    case 11:
      chan = Channel_11;
      break;
    case 12:
      chan = Channel_12;
      break;
    case 13:
      chan = Channel_13;
      break;
    case 14:
      chan = Channel_14;
      break;
    case 15:
      chan = Channel_15;
      break;
    case 16:
      chan = Channel_16;
      break;
    default: break;
      
  }
  return chan;
}