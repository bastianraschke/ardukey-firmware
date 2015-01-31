/*
 * ArduKey - A simple OTP device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2015 Bastian Raschke
 * All rights reserved.
 *
 */

#include <AES.h>
#include <ArduKey.h>
#include <TimerOne.h>

#if ARDUKEY_ENABLE_KEYBOARD == 1
    #include <VUSBHIDKeyboardMouse.h>
#endif

AES aes;

ardukey_token_t token;
ardukey_otp_t otp;

/*
 * Increments the non-volatile counter value.
 *
 * @return void
 *
 */
void incrementCounter()
{
    token.counter += 1;
    ArduKeyEEPROM::setCounter(token.counter);
}

/*
 * Increments the volatile session counter value.
 *
 * @return void
 *
 */
void incrementSessionCounter()
{
    // Checks if the session counter reached the maximum possible value (255)
    if ( token.session == 0xFF )
    {
        token.session = 0x00;

        // Increments the non-volatile counter instead to indicate change
        incrementCounter();
    }
    else
    {
        token.session += 1;
    }
}

/*
 * Increments the timestamp value.
 *
 * @return void
 *
 */
void incrementTimestamp()
{
    // Combines high and low part to 24 Byte integer
    uint32_t timestamp = (token.timestamp_h << 8) | (token.timestamp_l << 0);
    timestamp += 1;

    token.timestamp_h = timestamp >> 8; // High (left) part
    token.timestamp_l = timestamp >> 0; // Low (right) part
}

/*
 * Contains all ArduKey initalizing processes.
 *
 * @return void
 *
 */
void initializeArduKey()
{
    uint8_t aesKey[AES_KEYSIZE] = {0};
    uint8_t publicId[ARDUKEY_PUBLICID_SIZE] = {0};
    uint8_t secretId[ARDUKEY_SECRETID_SIZE] = {0};

    // Reads the AES key from EEPROM and sets AES library preferences
    ArduKeyEEPROM::getAESKey(aesKey);
    aes.set_key(aesKey, AES_CIPHER_BITS);

    // Reads the public id from EEPROM and sets to OTP struct
    ArduKeyEEPROM::getPublicId(publicId);
    memcpy(otp.publicId, publicId, sizeof(publicId));

    // Reads the secret id from EEPROM and sets to token struct
    ArduKeyEEPROM::getSecretId(secretId);
    memcpy(token.secretId, secretId, sizeof(secretId));

    // Gets current counter value
    token.counter = ArduKeyEEPROM::getCounter();

    // Initializes session counter
    token.session = 0x00;

    // Initializes timestamp
    token.timestamp_h = 0x0000;
    token.timestamp_l = 0x00;

    // Initializes pseudo random number generator with "random" analog pin noise
    uint16_t randomSeedValue = analogRead(0);
    randomSeed(randomSeedValue);

    // Increments counter on startup
    incrementCounter();

    // Initializes timer library for updating timestamp
    Timer1.initialize();
    Timer1.attachInterrupt(incrementTimestamp, TIMERONE_TIMESTAMPUPDATE);

    #if ARDUKEY_ENABLE_KEYBOARD == 1
        // Important for USB:
        // Disables default millisecond counter of Arduino (it disturbs the USB otherwise)
        #ifdef TIMSK
          // Older ATmega
          TIMSK &= ~(_BV(TOIE0));
        #else
          // Newer ATmega
          TIMSK0 &= ~(_BV(TOIE0));
        #endif
    #endif
}

/*
 * Generates a new ready-to-output OTP.
 *
 * @args result The OTP.
 * @return bool
 *
 */
bool generateOneTimePad(char result[ARDUKEY_OTP_SIZE])
{
    // Gets 2 bytes pseudo random entropy (range 0..66535)
    token.random = random(0xFFFF);

    // Calculates CRC16 checksum of raw token
    // (only the first 14 Bytes to exclude the checksum itself)
    token.crc = ArduKeyUtilities::calculateCRC16((uint8_t*) &token,
        ARDUKEY_TOKEN_SIZE - 2);

    #if ARDUKEY_DEBUG == 1
        Serial.println("Raw token:");
        ArduKeyUtilities::serialDump((uint8_t*) &token, sizeof(token));

        char buffer[128];
        sprintf(buffer,
            "(counter = 0x%04X; session = 0x%02X; timestamp = 0x%04X%02X; random = 0x%04X; crc = 0x%04X)",
            token.counter, token.session, token.timestamp_h, token.timestamp_l, token.random, token.crc
        );
        Serial.println(buffer);
    #endif

    // The buffer for encrypted raw token
    uint8_t cipher[AES_BLOCKSIZE] = {0};

    // Encrypts the raw token
    if ( aes.encrypt((uint8_t*) &token, cipher) != 0 )
    {
        return false;
    }

    // Copy encrypted raw token to OTP struct
    memcpy(otp.encryptedRawToken, cipher, sizeof(cipher));

    #if ARDUKEY_DEBUG == 1
        Serial.println("Complete OTP:");
        ArduKeyUtilities::serialDump((uint8_t*) &otp, sizeof(otp));
    #endif

    // Converts OTP (public id + encrypted raw token) to arduhex encoding
    ArduKeyUtilities::encodeArduHex((char *) &otp, result,
        ARDUKEY_PUBLICID_SIZE + ARDUKEY_TOKEN_SIZE);

    // Increments session counter
    incrementSessionCounter();

    return true;
}

/*
 * The Arduino setup method.
 *
 * @return void
 *
 */
void setup()
{
    #if ARDUKEY_DEBUG == 1
        Serial.begin(9600);
        Serial.println("Initializing ArduKey...");
    #endif

    initializeArduKey();

    // Configures button pin as an input with internal pull-up resistor
    //pinMode(ARDUKEY_PIN_BUTTON, INPUT_PULLUP);

    // Additionally sets pin default state
    //digitalWrite(ARDUKEY_PIN_BUTTON, HIGH);
}

int previousButtonState = HIGH;



uint8_t readCapacitivePin(int pinToMeasure)
{
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  uint8_t SREG_old = SREG; //back up the AVR Status Register
  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}

  // End of timing-critical section; turn interrupts back on if they were on before, or leave them off if they were off before
  SREG = SREG_old;

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}


/*
 * The Arduino loop method.
 *
 * @return void
 *
 */
void loop()
{
    #if ARDUKEY_ENABLE_KEYBOARD == 1
        UsbKeyboard.update(0);
    #endif

    char otp[ARDUKEY_OTP_SIZE] = "";

     /*
    // Gets current button state
    int buttonState = digitalRead(ARDUKEY_PIN_BUTTON);

    // Checks if button state changed (polling)
    if ( buttonState != previousButtonState && buttonState == LOW )
    {
        generateOneTimePad(otp);

        #if ARDUKEY_DEBUG == 1
            Serial.println("Output OTP:");
            Serial.println(otp);
            Serial.println();
        #endif

        #if ARDUKEY_ENABLE_KEYBOARD == 1
            for (int i = 0; i < sizeof(otp); i++)
            {
                UsbKeyboard.sendKey(otp[i]);
            }

            UsbKeyboard.sendKeyStroke(KEY_ENTER);
        #endif
    }
    */

    // Gets current button state
    int buttonState = readCapacitivePin(ARDUKEY_PIN_CAPACITIVE_SENSOR);

    // Checks if button state changed (polling)
    if ( buttonState > 1 && previousButtonState == 1 )
    {
        generateOneTimePad(otp);

        #if ARDUKEY_DEBUG == 1
            Serial.println("Output OTP:");
            Serial.println(otp);
            Serial.println();
        #endif

        #if ARDUKEY_ENABLE_KEYBOARD == 1
            for (int i = 0; i < sizeof(otp); i++)
            {
                UsbKeyboard.sendKey(otp[i]);
            }

            UsbKeyboard.sendKeyStroke(KEY_ENTER);
        #endif
    }

    previousButtonState = buttonState;
}
