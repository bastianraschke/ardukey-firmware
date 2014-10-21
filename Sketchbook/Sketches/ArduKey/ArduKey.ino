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
















bool generateOneTimePad(unsigned char publicId[6], unsigned char secretId[6], unsigned char resultBuffer[16])
{
  // Clears operation buffer
  resultBuffer = {0};



  // Reads the current ArduKey counter value from EEPROM
  unsigned int counter = ArduKeyEEPROM::getCounter();


  resultBuffer[0] = counter << 8;





  // Increments counter
  ArduKeyEEPROM::setCounter(counter + 1); // Note: counter++ does not work

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

  // Temporary operation buffer
  unsigned char buffer[AES_BLOCKSIZE] = {0};



  memset(buffer, 0, 16);
  ArduKeyEEPROM::getAESKey(buffer);

  Serial.println("Current AES key:");
  ArduKeyUtilities::serialDump(buffer, 16);






  // Plain text example
  unsigned char plain[AES_BLOCKSIZE] =
  {
    0xAA, 0x44, 0x81, 0xEC, 0x3C, 0xC6, 0x27, 0xBA, 0xCD, 0x5D, 0xC3, 0xFB, 0x08, 0xF2, 0x73, 0xE6,
  };



  // Dumps plain text
  ArduKeyUtilities::serialDump(plain, AES_BLOCKSIZE);

  memset(plain, 0, 16);

  // Encrypting plain text
  if ( aes.encrypt(plain, buffer) != 0 )
  {
    Serial.println("Error: AES encryption process failed!");
  }
  ArduKeyUtilities::serialDump(buffer, AES_BLOCKSIZE);

  memset(plain, 0, 16);

  // Decrypting cipher text
  if ( aes.decrypt(buffer, buffer) != 0 )
  {
    Serial.println("Error: AES encryption process failed!");
  }
  ArduKeyUtilities::serialDump(buffer, AES_BLOCKSIZE);


  Serial.println();
  Serial.println();
  delay(4000);
}
