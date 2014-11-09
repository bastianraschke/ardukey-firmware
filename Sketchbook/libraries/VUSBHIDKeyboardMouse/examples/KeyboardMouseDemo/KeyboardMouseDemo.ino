
#include "VUSBHIDKeyboardMouse.h"

#define KLICK_LED 3
#define BLINK_LED 5
#define BUTTON    6

prog_uchar message[] PROGMEM  = {
"tinyUSBboard\n"
"http://matrixstorm.com/avr/tinyusbboard/\n"
"\n"
"\n"
"In case you encouter problems, have any questions or in case\n"
"you just have remarks, please do not hesitate to contact:\n"
"Stephan Baerwolf  - stephan@matrixstorm.com\n"
"matrixstorm@gmx.de, or stephan.baerwolf@tu-ilmenau.de\n"
"\n"
"PGP-KeyID\t: 0xFB9B12DB\n"
"Fingerprint\t: 2252 B41E 12DF 41AD 476F 7F36 084A 0911 FB9B 12DB\n"
"\n"
"\n"
"\n"
};

void setup() {
  randomSeed(analogRead(0));

  pinMode(KLICK_LED, OUTPUT);
  pinMode(BLINK_LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void delayMs(unsigned int ms) {
  for (int i = 0; i < 10*ms; i++) {
    delayMicroseconds(100);
    VUSBHIDKeyboardMouse.update();
  }
}

static byte    i              = 0;
static byte    delayCounter   = 0; /* start delay */
static size_t  charPosition   = sizeof(message); /* message symbol */

void loop() {
  i++;
  VUSBHIDKeyboardMouse.update(1);
  if (i>16) {
    uint8_t button = 0;
    i=0;
    if (delayCounter>=32) {
      charPosition++;
      if (charPosition>=sizeof(message)) charPosition=0;
      UsbKeyboard.sendKey(pgm_read_byte_near(message + charPosition));
    } else delayCounter++;

    // wiggle the mouse
    if (!digitalRead(BUTTON)) button|=MOD_MOUSE_LEFT;
    UsbMouse.sendMouseEvent(random(-8,8), random(-8,8), button);
    digitalWrite(KLICK_LED, button);

    digitalWrite(BLINK_LED, !digitalRead(BLINK_LED));
  }
  delayMs(4);
}