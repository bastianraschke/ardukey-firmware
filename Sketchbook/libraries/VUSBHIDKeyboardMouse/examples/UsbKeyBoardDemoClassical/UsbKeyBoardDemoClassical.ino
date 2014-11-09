#include "VUSBHIDKeyboardMouse.h"

#define BUTTON_PIN 6

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BUTTON_PIN, HIGH);

}

void delayMs(unsigned int ms) {
  for (int i = 0; i < 10*ms; i++) {
    delayMicroseconds(100);
    VUSBHIDKeyboardMouse.update();
  }
}

void loop() {
  
  UsbKeyboard.update();

  digitalWrite(13, !digitalRead(13));

  if (digitalRead(BUTTON_PIN) == 0) {
    
    //UsbKeyboard.sendKeyStroke(KEY_B, MOD_GUI_LEFT);
    
    UsbKeyboard.sendKeyStroke(KEY_H);
    UsbKeyboard.sendKeyStroke(KEY_E);
    UsbKeyboard.sendKeyStroke(KEY_L);
    UsbKeyboard.sendKeyStroke(KEY_L);
    UsbKeyboard.sendKeyStroke(KEY_O);

    UsbKeyboard.sendKeyStroke(KEY_SPACE);

    UsbKeyboard.sendKeyStroke(KEY_W);
    UsbKeyboard.sendKeyStroke(KEY_O);
    UsbKeyboard.sendKeyStroke(KEY_R);
    UsbKeyboard.sendKeyStroke(KEY_L);
    UsbKeyboard.sendKeyStroke(KEY_D);
    //UsbKeyboard.sendKeyStroke(KEY_B, MOD_GUI_LEFT);

    UsbKeyboard.sendKeyStroke(KEY_ENTER);
    delayMs(4);
    UsbKeyboard.update(4);
   }
}