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




void convertIntToEqlString(unsigned char dst[], const unsigned char src[], int srcSize)
{
  // The transforming table
  const char trans[16] = { 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n', 'r', 't', 'u', 'v' };

  // Source field counter
  int a = 0;

  // Destination field counter
  int b = 0;

  for (int i = 0; i < srcSize; i++)
  {
    const unsigned char currentSrcByte = src[a++];

    dst[b++] = trans[ (currentSrcByte & 0xf0) >> 4 ];
    dst[b++] = trans[ (currentSrcByte & 0x0f) >> 0 ];
  }

  dst[b] = '\0';
}




bool generateOneTimePad(/*unsigned char publicId[6], unsigned char secretId[6], unsigned char resultBuffer[16]*/)
{
  unsigned int counter = ArduKeyEEPROM::getCounter();
  unsigned int timestamp = 7;

  unsigned char otpBody[ARDUKEY_BLOCKSIZE] = 
  {
    // The secret id
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // The counter
    (counter >> 8) & 0xFF,
    (counter >> 0) & 0xFF,

    // Timestamp
    (timestamp >> 8) & 0xFF,
    (timestamp >> 0) & 0xFF,

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
    Serial.println("Error: AES encryption process failed!");
  }

  // DEBUG
  ArduKeyUtilities::serialDump(buffer, ARDUKEY_BLOCKSIZE);





  unsigned char resultBuffer[32 + 1] = "";

  convertIntToEqlString(resultBuffer, buffer, ARDUKEY_BLOCKSIZE);
  
  /*
  for (int i = 0; i < sizeof(resultBuffer); i++)
  {
    Serial.print(resultBuffer[i]);
  }
  */

  Serial.println(resultBuffer);

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
  generateOneTimePad();

  Serial.println();
  Serial.println();
  delay(4000);
}
