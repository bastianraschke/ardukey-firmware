#include <ArduKey.h>
#include <AES.h>

AES aes;
ardukey_token_t token;

/*
 * The setup method.
 * 
 * @return void
 *
 */
void setup()
{
    Serial.begin(57600);
    initializeArduKey();
}

/*
 * Contains all ArduKey initalizing processes.
 * 
 * @return void
 *
 */
void initializeArduKey()
{
    unsigned char buffer[AES_KEYSIZE] = {0};

    // Reads the current AES key from EEPROM and sets AES library preferences
    ArduKeyEEPROM::getAESKey(buffer);
    aes.set_key(buffer, AES_CIPHER_BITS);




    unsigned char secretId[ARDUKEY_SECRETID_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    memcpy(token.secretId, secretId, ARDUKEY_SECRETID_SIZE);

}


/*
 * Generates a new complete one time pad.
 * 
 * @return bool
 *
 */
bool generateOneTimePad(char resultBuffer[ARDUKEY_OTP_SIZE])
{
    /*
    unsigned char otpBody[16] = 
    {
        // The secret id
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        // The counter
        (counter >> 8) & 0xFF, // left byte
        (counter >> 0) & 0xFF, // right byte

        // Timestamp
        (timestamp >> 8) & 0xFF, // left byte
        (timestamp >> 0) & 0xFF, // right byte

        // Random entropy
        'a', 'b', 'c', 'd',

        // Checksum
        0x00, 0x00,
    };
    */


    unsigned int counter = ArduKeyEEPROM::getCounter();
    unsigned int timestamp = 65518; // = 0xFFEE
    unsigned int random = 0x0000;

    token.counter = counter;
    token.timestamp = timestamp;
    token.random = random;





    unsigned char buffer[ARDUKEY_BLOCKSIZE] = {0};

    // Encrypts the raw token
    if ( aes.encrypt(buffer, buffer) != 0 )
    {
        return false;
    }

    // DEBUG
    ArduKeyUtilities::serialDump(buffer, sizeof(buffer));






    // Converts the encrypted token to universal equal chars
    ArduKeyUtilities::convertToHex((char *) buffer, resultBuffer, ARDUKEY_BLOCKSIZE);


    // Increments counter
    // ArduKeyEEPROM::setCounter(counter + 1); // Note: counter++ does not work

    return true;
}


/*
 * The loop method.
 * 
 * @return void
 *
 */
void loop() 
{
    char buffer[ARDUKEY_OTP_SIZE] = "";

    // Generate one OTP per 4 seconds
    generateOneTimePad(buffer);

    Serial.println(buffer);

    Serial.println();
    delay(4000);
}
