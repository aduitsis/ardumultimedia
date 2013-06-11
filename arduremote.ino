/* 
 
 USB HID Generic Consumer Device Proof of Concept for the Arduino platform.
 
 Athanasios Douitsis, June 2013.
 
 This program has been tested on an Arduino Leonardo. 
 
 Uses parts from the examples coming with the Arduino dev enironment.
 
 Code bellow assumes that there is an IR receiver connected at pin 11 of the device.
 
 Tested with a cheap IR remote from an old DVD player using a NEC IR chipset.
 
 
 
 */


struct kr {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
};

struct kr r1;

struct cr {
  uint16_t code;
  uint8_t data[5];
};

struct cr r2;

#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;


void setup() {
  // open the serial port:
  Serial.begin(9600);
  // initialize control over the keyboard:
  Keyboard.begin();
  irrecv.enableIRIn(); // Start the receiver
  
  r2.code=0x0000;
  for(int j=0;j<5;j++) r2.data[j] = 0x00;
  
  r1.modifiers=0x00;
  for(int j=0;j<6;j++) r1.keys[j] = 0x00;
}


#define HID_STOP        0x00B7

#define HID_VOLUP       0x00E9
#define HID_VOLDOWN     0x00EA
#define HID_MUTE        0x00E2
#define HID_PLAY        0x00CD
#define HID_CALC        0x0192
#define HID_MAIL        0x018A

#define HID_WWW_SEARCH  0x0221

#define KEY_UP          0x52
#define KEY_DOWN        0x51
#define KEY_LEFT        0x50
#define KEY_RIGHT       0x4F      

uint16_t consumer = 0;
uint16_t command = 0;
uint16_t last = 0;
uint32_t received_code = 0;
uint32_t previous_received_code = 0;


void loop() {
  
  if (irrecv.decode(&results)) {
    
    command = 0;
    
    //unless proven otherwise, we assume a keyboard command
    consumer = 0;
    
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
    
    //save the previous value 
    previous_received_code = received_code;
    //and get the value from the results
    if (results.decode_type == NEC) {
      received_code = results.value;
    }  
    
    
    if( received_code == 0xFFFFFFFF ) {
        command = 0;
        if( previous_received_code == 0xFFFFFFFF ) {
          Serial.println("repeating last command");
          command = last;
        }
      }  
      else if( received_code == 0xFF0AF5 ){
        command = HID_VOLUP;
        consumer = 1;
      }
      else if( received_code == 0xFF8A75 ) {
        command = HID_VOLDOWN;
        consumer = 1;
      }
      else if( received_code == 0xFF926D ) {
        command = HID_MUTE;
        consumer = 1;
      }
      else if( received_code == 0xFF22DD ) {
        command = KEY_UP;
      }
      else if( received_code == 0xFF12ED ) {
        command = KEY_DOWN;
      } 
      else if( received_code == 0xFF32CD ) {
        command = KEY_LEFT;
      }
      else if( received_code == 0xFF02FD) {
        command = KEY_RIGHT;
      }
      
        
    if( command != 0 ) {
      last = command;
      if( consumer == 1 ){
        r2.code = command;
        Serial.print("sending hid command ");
        Serial.println(r2.code,HEX);
    
        HID_SendReport(3,&r2,sizeof(struct cr));
        //r2.code = 0;
      }
      else {
        Serial.print("sending keystroke ");
        Serial.println(command,HEX);
        r1.keys[0] = command;
        HID_SendReport(2,&r1,sizeof(struct kr));
      }
      
    }
  }
  else {
    if(r2.code != 0x0000 ) {
      r2.code = 0x0000;
      Serial.println("key lifted");
      HID_SendReport(3,&r2,sizeof(struct cr));
    }
    if(r1.keys[0] != 0) {
      r1.keys[0] = 0x00;
      HID_SendReport(2,&r1,sizeof(struct kr));
    }
  }
}


