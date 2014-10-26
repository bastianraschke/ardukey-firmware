#include <ArduKey.h>
#include <AES.h>

AES aes;

ardukey_token_t token;
ardukey_otp_t otp;

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
    // Reads the AES key from EEPROM and sets AES library preferences
    unsigned char aesKey[AES_KEYSIZE] = {0};
    ArduKeyEEPROM::getAESKey(aesKey);
    aes.set_key(aesKey, AES_CIPHER_BITS);

    // Reads the public id from EEPROM and sets to OTP struct
    unsigned char publicId[ARDUKEY_PUBLICID_SIZE] = {0};
    ArduKeyEEPROM::getPublicId(publicId);
    memcpy(otp.publicId, publicId, sizeof(publicId));

    // Reads the secret id from EEPROM and sets to token struct
    unsigned char secretId[ARDUKEY_SECRETID_SIZE] = {0};
    ArduKeyEEPROM::getSecretId(secretId);
    memcpy(token.secretId, secretId, sizeof(secretId));

    // Gets current counter value
    token.counter = ArduKeyEEPROM::getCounter();

    // Initialize session counter
    token.session = 0;

    // Initialize timestamp
    token.timestamp = 0x0000;

    // Initialize pseudo random number generator
    // TODO

    // Increments counter
    // ArduKeyEEPROM::setCounter(token.counter + 1);
}

/*
 * Generates a new ready-to-output OTP.
 *
 * @args result: The OTP.
 * @return bool
 *
 */
bool generateOneTimePad(char result[ARDUKEY_OTP_SIZE])
{
    // Sets current timestamp
    // TODO
    token.timestamp = 0xFFEE;

    // Sets some random entropy
    // TODO
    token.random = 0xAABB;

    // Calculates CRC16 checksum of raw token
    // (only the first 14 Bytes cause we do not want to include the checksum itself)
    token.crc = ArduKeyUtilities::CRC16((unsigned char*) &token, ARDUKEY_BLOCKSIZE - 2);

    // DEBUG: Print raw token
    ArduKeyUtilities::serialDump((unsigned char*) &token, sizeof(token));

    // The buffer for encrypted raw token
    unsigned char cipher[AES_BLOCKSIZE] = {0};

    // Encrypts the raw token
    if ( aes.encrypt((unsigned char*) &token, cipher) != 0 )
    {
        return false;
    }

    // Copy encrypted raw token to OTP struct
    memcpy(otp.encryptedRawToken, cipher, sizeof(cipher));

    // DEBUG: Print OTP struct
    ArduKeyUtilities::serialDump((unsigned char*) &otp, sizeof(otp));

    // Converts full OTP (public id + encrypted raw token) to modhex
    // TODO
    ArduKeyUtilities::convertToHex((char *) &otp, result, ARDUKEY_PUBLICID_SIZE + ARDUKEY_BLOCKSIZE);

    // Increments session counter
    token.session++;

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
    // Serial.println("DUMP:");
    // ArduKeyEEPROM::dumpEEPROM();

    char otp[ARDUKEY_OTP_SIZE] = "";

    // Generate one OTP per 4 seconds
    generateOneTimePad(otp);
    Serial.println(otp);

    Serial.println();
    delay(4000);
}
