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

#if ARDUKEY_ENABLE_DEFAULT_KEYBOARD_LIBRARY == 1
    #include <VUSBHIDKeyboardMouse.h>
#endif

AES aes;

ardukey_token_t token;
ardukey_otp_t otp;

/*
 * Increment the non-volatile counter value.
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
 * Increment the session counter value.
 *
 * @return void
 *
 */
void incrementSessionCounter()
{
    // Check if the session counter reached the maximum possible value (255)
    if ( token.sessionCounter == 0xFF )
    {
        token.sessionCounter = 0x00;

        // Increment the non-volatile counter instead to indicate change
        incrementCounter();
    }
    else
    {
        token.sessionCounter += 1;
    }
}

/*
 * Increment the timestamp value.
 *
 * @return void
 *
 */
void incrementTimestamp()
{
    // Combine high (0xHH) and low part (0xLLLL) to 24 Byte integer (0xHHLLLL)
    uint32_t timestamp = (token.timestamp_h << 16) | token.timestamp_l;
    timestamp += 1;

    token.timestamp_h = timestamp >> 16; // High (left) part
    token.timestamp_l = timestamp & 0x00FFFF; // Low (right) part
}

/*
 * Initialize the ArduKey.
 *
 * @return void
 *
 */
void initializeArduKey()
{
    uint8_t aesKey[AES_KEYSIZE] = {0};
    uint8_t publicId[ARDUKEY_PUBLICID_SIZE] = {0};
    uint8_t secretId[ARDUKEY_SECRETID_SIZE] = {0};

    // Read the AES key from EEPROM and set AES library preferences
    ArduKeyEEPROM::getAESKey(aesKey);
    aes.set_key(aesKey, AES_CIPHER_BITS);

    // Read the public id from EEPROM and set to OTP struct
    ArduKeyEEPROM::getPublicId(publicId);
    memcpy(otp.publicId, publicId, sizeof(publicId));

    // Read the secret id from EEPROM and set to token struct
    ArduKeyEEPROM::getSecretId(secretId);
    memcpy(token.secretId, secretId, sizeof(secretId));

    // Get current counter value
    token.counter = ArduKeyEEPROM::getCounter();

    // Initialize session counter
    token.sessionCounter = 0x00;

    // Initialize timestamp
    token.timestamp_h = 0x00;
    token.timestamp_l = 0x0000;

    // Initialize pseudo random number generator with "random" analog pin noise
    uint16_t randomSeedValue = analogRead(0);
    randomSeed(randomSeedValue);

    // Increment counter on startup
    incrementCounter();

    // Initialize timer library for updating timestamp
    Timer1.initialize();
    Timer1.attachInterrupt(incrementTimestamp, TIMERONE_TIMESTAMPUPDATE);

    #if ARDUKEY_ENABLE_DEFAULT_KEYBOARD_LIBRARY == 1
        // Important for USB:
        // Disable default millisecond counter of Arduino (it disturbs the USB otherwise)
        #ifdef TIMSK
          // Older ATmega
          TIMSK &= ~(_BV(TOIE0));
        #else
          // Newer ATmega
          TIMSK0 &= ~(_BV(TOIE0));
        #endif

    #elif ARDUKEY_ENABLE_ARDUINO_KEYBOARD_LIBRARY == 1
        // Initializes Arduino keyboard library
        Keyboard.begin();
    #endif
}

/*
 * Generate a new ready-to-output OTP.
 *
 * @args result The OTP.
 * @return bool
 *
 */
bool generateOneTimePad(char result[ARDUKEY_OTP_SIZE])
{
    // Get 2 bytes pseudo random entropy (range 0..66535)
    token.random = random(0xFFFF);

    // Calculate CRC16 checksum of raw token (just first 14 bytes to exclude checksum itself):
    // Note: The CRC16 checksum must be flipped: see http://forum.yubico.com/viewtopic.php?f=2&t=69
    token.crc = ~ArduKeyUtilities::calculateCRC16((uint8_t*) &token, ARDUKEY_TOKEN_SIZE - 2);

    #if ARDUKEY_DEBUG == 1
        Serial.println("Raw token:");
        ArduKeyUtilities::serialDump((uint8_t*) &token, sizeof(token));

        char buffer[128];
        sprintf(buffer,
            "(counter = 0x%04X; timestamp_h = 0x%02X; timestamp_l = 0x%04X; session = 0x%02X; random = 0x%04X; crc = 0x%04X)",
            token.counter, token.timestamp_h, token.timestamp_l, token.sessionCounter, token.random, token.crc
        );
        Serial.println(buffer);
    #endif

    // The buffer for encrypted raw token
    uint8_t cipher[AES_BLOCKSIZE] = {0};

    // Encrypt the raw token
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

    // Convert OTP (public id + encrypted raw token) to arduhex encoding
    ArduKeyUtilities::encodeArduHex((char *) &otp, result,
        ARDUKEY_PUBLICID_SIZE + ARDUKEY_TOKEN_SIZE);

    // Increment session counter
    incrementSessionCounter();

    return true;
}

/*
 * The Arduino setup function.
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

    // Configure button pin as an input with internal pull-up resistor
    pinMode(ARDUKEY_PIN_BUTTON, INPUT_PULLUP);

    // Additionally set pin default state
    digitalWrite(ARDUKEY_PIN_BUTTON, HIGH);
}

int previousButtonState = HIGH;

/*
 * The Arduino loop function.
 *
 * @return void
 *
 */
void loop()
{
    #if ARDUKEY_ENABLE_DEFAULT_KEYBOARD_LIBRARY == 1
        UsbKeyboard.update(0);
    #endif

    char otp[ARDUKEY_OTP_SIZE] = "";

    // Get current button state
    int buttonState = digitalRead(ARDUKEY_PIN_BUTTON);

    // Check if button state changed (polling)
    if ( buttonState != previousButtonState && buttonState == LOW )
    {
        generateOneTimePad(otp);

        #if ARDUKEY_DEBUG == 1
            Serial.println("Output OTP:");
            Serial.println(otp);
            Serial.println();
        #endif

        #if ARDUKEY_ENABLE_DEFAULT_KEYBOARD_LIBRARY == 1
            for (int i = 0; i < sizeof(otp); i++)
            {
                UsbKeyboard.sendKey(otp[i]);
            }

            UsbKeyboard.sendKeyStroke(KEY_ENTER);

        #elif ARDUKEY_ENABLE_ARDUINO_KEYBOARD_LIBRARY == 1
            Keyboard.print(otp);
            Keyboard.write(KEY_RETURN);
        #endif
    }

    previousButtonState = buttonState;
}
