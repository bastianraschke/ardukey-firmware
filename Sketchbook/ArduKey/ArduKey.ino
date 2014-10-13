#include <ArduKey.h>
#include <AES.h>

#define ARDUKEY_VERSION "1.0.0"

// Used AES mode
#define AES_CIPHER_BITS 128

// The size of the blocks
#define AES_BLOCKSIZE N_BLOCK

// We only work with one block
#define AES_CBC_BLOCKCOUNT 1

/*
 * The 16 bytes key (for one block).
 */
unsigned char key[] = 
{
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/*
 * The 16 bytes input text (one block).
 */
unsigned char plain[] =
{
  0xAA, 0x44, 0x81, 0xec, 0x3c, 0xc6, 0x27, 0xba, 0xcd, 0x5d, 0xc3, 0xfb, 0x08, 0xf2, 0x73, 0xe6,
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


unsigned int counter = eeprom_readCounter();
// unsigned int counter = 0x0000;





AES aes;

/*
 * The setup method.
 * 
 * @return void
 *
 */
void setup()
{
  Serial.begin(57600);

  Serial.print("Initialising ArduKey ");
  Serial.println(ARDUKEY_VERSION);

  // Sets specifications
  if ( aes.set_key(key, AES_CIPHER_BITS) != 0 )
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
  Serial.print("Counter: ");
  Serial.println(counter++);

  // Dumps plain text
  utilities_serialDump(plain, AES_BLOCKSIZE);

  // Copies original IV to temp iv
  memcpy(temp_iv, my_iv, 16);

  // Encrypting plain text
  if ( aes.cbc_encrypt(plain, cipher, AES_CBC_BLOCKCOUNT, temp_iv) != 0 )
  {
    Serial.println("Error: AES encryption process failed!");
  }
  utilities_serialDump(cipher, AES_BLOCKSIZE);

  // Copies original IV to temp iv
  memcpy(temp_iv, my_iv, 16);

  // Decrypting cipher text
  if ( aes.cbc_decrypt(cipher, new_plain, AES_CBC_BLOCKCOUNT, temp_iv) != 0 )
  {
    Serial.println("Error: AES decryption process failed!");
  }
  utilities_serialDump(new_plain, AES_BLOCKSIZE);


  Serial.println();
  Serial.println();
  delay(5000);
}
