#include "VUSBHIDKeyboardMouse.h"

void delayMs(unsigned int ms)
{
    for (int i = 0; i < (10 * ms); i++)
    {
        delayMicroseconds(100);
        VUSBHIDKeyboardMouse.update();
    }
}

// LED 3, 5 not working - 8, 9 works
int ledNumber = 9;

void setup()
{
    Serial.begin(9600);

    // Initialize the digital pin as an output:
    pinMode(ledNumber, OUTPUT);
}

void loop()
{
    VUSBHIDKeyboardMouse.update(1);

    char message[] = "Hallo";

    for (int i = 0; i < sizeof(message); i++)
    {
        UsbKeyboard.sendKey(message[i]);
    }

    digitalWrite(ledNumber, HIGH);
    Serial.println("LED is high");
    delayMs(1000);

    digitalWrite(ledNumber, LOW);
    Serial.println("LED is low");
    delayMs(1000);
}
