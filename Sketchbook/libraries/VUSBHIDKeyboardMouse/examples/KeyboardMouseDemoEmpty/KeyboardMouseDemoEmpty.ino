#include "VUSBHIDKeyboardMouse.h"

#define LED_NUMLOCK    8
#define LED_CAPSLLOCK  9
#define LED_SCROLLLOCK 5
#define LED_BLINK      3

void setup() {
  pinMode(LED_NUMLOCK, OUTPUT);
  pinMode(LED_CAPSLLOCK, OUTPUT);
  pinMode(LED_SCROLLLOCK, OUTPUT);
  pinMode(LED_BLINK, OUTPUT);
}

void delayMs(unsigned int ms) {
  for (int i = 0; i < 100*ms; i++) {
    delayMicroseconds(10);
    VUSBHIDKeyboardMouse.update();
  }
}

byte counter = 0;

void loop() {
  VUSBHIDKeyboardMouse.update(1);
  
  counter++;
  if (counter >= 250) {
    counter=0;
    digitalWrite(LED_BLINK, !digitalRead(LED_BLINK));
  }
  
  delayMs(4);
}



#ifndef USB_CFG_HID_NOKEYBOARD
#ifdef __cplusplus
extern "C"{
#endif
void EVENT_CHANGE_LED_state (void) {
  digitalWrite(LED_NUMLOCK, current_LED_state & _BV(HIDKEYBOARD_LEDBIT_NUM_LOCK));
  digitalWrite(LED_CAPSLLOCK, current_LED_state & _BV(HIDKEYBOARD_LEDBIT_CAPS_LOCK));
  digitalWrite(LED_SCROLLLOCK, current_LED_state & _BV(HIDKEYBOARD_LEDBIT_SCROLL_LOCK));
}
#ifdef __cplusplus
}
#endif
#endif
