#include <ArduKey.h>
#include <AES.h>

AES aes;

/*
 * The setup method.
 * 
 * @return void
 *
 */
void setup()
{
  // Initialising ArduKey debugging
  Serial.begin(57600);

  // Initialized buffer array
  unsigned char buffer[16] = {0};

  // Reads the current AES key from EEPROM
  ArduKeyEEPROM::getAESKey(buffer);

  // Sets AES library preferences
  if ( aes.set_key(buffer, AES_CIPHER_BITS) != 0 )
  {
    Serial.print("Error: AES configuration could not be set!");
  }
}








bool generateOneTimePad(/*unsigned char publicId[6], unsigned char secretId[6], unsigned char resultBuffer[16]*/)
{
  unsigned int counter = ArduKeyEEPROM::getCounter();
  unsigned int timestamp = 65518; // = 0xFFEE

  unsigned char otpBody[ARDUKEY_BLOCKSIZE] = 
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

  // DEBUG
  ArduKeyUtilities::serialDump(otpBody, ARDUKEY_BLOCKSIZE);



  // General operation buffer
  unsigned char buffer[ARDUKEY_BLOCKSIZE] = {0};

  // Encrypts the plain OTP body
  if ( aes.encrypt(otpBody, buffer) != 0 )
  {
    // AES encryption process failed
    return false;
  }

  // DEBUG
  ArduKeyUtilities::serialDump(buffer, ARDUKEY_BLOCKSIZE);




  // Length = 32 chars + '\0' byte
  char resultBuffer[32 + 1] = "";

  ArduKeyUtilities::convertToHex((char *) buffer, resultBuffer, ARDUKEY_BLOCKSIZE);
  Serial.println(resultBuffer);




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
  generateOneTimePad();

  // Increments counter
  // ArduKeyEEPROM::setCounter(counter + 1); // Note: counter++ does not work

  Serial.println();
  Serial.println();
  delay(4000);
}
