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

/*
 * The loop method.
 * 
 * @return void
 *
 */
void loop() 
{
  ArduKeyEEPROM::dumpTable();

  // Reads the current ArduKey counter value from EEPROM
  unsigned int counter = ArduKeyEEPROM::getCounter();

  Serial.print("Current counter value: ");
  Serial.println(counter);



  // ArduKeyEEPROM::setCounter(counter + 1);






  unsigned char buffer[16] = {0};
  ArduKeyEEPROM::getAESKey(buffer);

  Serial.println("Current AES key:");
  ArduKeyUtilities::serialDump(buffer, 16);






  /*
   * The 16 bytes input text (one block).
   */
  unsigned char plain[] =
  {
    0xAA, 0x44, 0x81, 0xEC, 0x3C, 0xC6, 0x27, 0xBA, 0xCD, 0x5D, 0xC3, 0xFB, 0x08, 0xF2, 0x73, 0xE6,
  };

  /*
   * The 16 bytes initialation vector (one block).
   */
  unsigned char my_iv[] =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

  // 1 block output
  unsigned char cipher[N_BLOCK];
  unsigned char new_plain[N_BLOCK];
  unsigned char temp_iv[N_BLOCK];

  // Dumps plain text
  ArduKeyUtilities::serialDump(plain, AES_BLOCKSIZE);

  // Copies original IV to temp iv
  memcpy(temp_iv, my_iv, 16);

  // Encrypting plain text
  if ( aes.cbc_encrypt(plain, cipher, AES_CBC_BLOCKCOUNT, temp_iv) != 0 )
  {
    Serial.println("Error: AES encryption process failed!");
  }
  ArduKeyUtilities::serialDump(cipher, AES_BLOCKSIZE);

  // Copies original IV to temp iv
  memcpy(temp_iv, my_iv, 16);

  // Decrypting cipher text
  if ( aes.cbc_decrypt(cipher, new_plain, AES_CBC_BLOCKCOUNT, temp_iv) != 0 )
  {
    Serial.println("Error: AES decryption process failed!");
  }
  ArduKeyUtilities::serialDump(new_plain, AES_BLOCKSIZE);


  Serial.println();
  Serial.println();
  delay(4000);
}
