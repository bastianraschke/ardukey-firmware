# ArduKey firmware

This firmware provides an one-time password (OTP) generation device based on the
Arduino platform. For example, cou can use it for two-factor authentication.

The firmware consists of two Arduino sketches:

## 1) Setup sketch

This sketch is just used for initialization of the device. It writes the
configured cryptographic information (public ID, secret ID and the symmetric AES
key) to the EEPROM of the device. Change these values before flashing to avoid
using public available cryptographic information!

**Important:** After flashing, the sketch must be run once to write the values to
the EEPROM. You can verify the changed values in EEPROM with any desired method
if you want (for example with the program `avrdude`).

## 2) Production sketch

This sketch is the "real" sketch used for productive application. Before you can
use this sketch, you have to initialize the device with the Setup sketch!

By default, the production sketch is intended for using with the "ArduKey". This
is a custom circuit that is compatible to the Arduino platform and based on the
[TinyUSBBoard by Stefan Bärwolf](http://matrixstorm.com/avr/tinyusbboard). It
uses an improved USB-HID library (based on V-USB by Objective Development) for
the Arduino platform, written by Stefan Bärwolf as well.

Nevertheless you can use it on a normal Arduino Leonardo. Than, the official
[Arduino USB-HID library](https://www.arduino.cc/en/Reference/MouseKeyboard) will
be used. If you want to use the firmware with an Arduino Leonardo, you have to
change two lines in the file `./src/libraries/ArduKey/ArduKey.h`:

Disable default keyboard library:

    - #define ARDUKEY_ENABLE_DEFAULT_KEYBOARD_LIBRARY 1
    + #define ARDUKEY_ENABLE_DEFAULT_KEYBOARD_LIBRARY 0

Enable the library for Arduino Leonardo:

    - #define ARDUKEY_ENABLE_ARDUINO_KEYBOARD_LIBRARY 0
    + #define ARDUKEY_ENABLE_ARDUINO_KEYBOARD_LIBRARY 1

After you did these changes, you can flash the sketch to the Arduino Leonardo.
If successful, you can connect a button between digital pin 12 and ground. To
generate an OTP you just press the button so the OTP will be typed automatically
to the connected computer (try it in a text editor for example).

## Further information

Additionally you can check out [this article](https://sicherheitskritisch.de/2015/06/ardukey-otp-generator-fuer-zweifaktor-authentifizierung-2fa-mit-arduino/), which explains the complete ArduKey infrastructure in detail (the article is in German):

