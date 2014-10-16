#include <ArduKey.h>
#include <AES.h>


// Used AES mode
#define AES_CIPHER_BITS 128

// The size of the blocks
#define AES_BLOCKSIZE N_BLOCK

// We only work with one block
#define AES_CBC_BLOCKCOUNT 1


AES aes;








#include <EEPROM.h>

//
// Absolute min and max eeprom addresses.
// Actual values are hardware-dependent.
//
// These values can be changed e.g. to protect
// eeprom cells outside this range.
//
const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;


//
// Initialize eeprom memory with
// the specified byte.
// Default value is 0xFF.
//
void eeprom_erase_all(byte b = 0xFF) {
  int i;

  for (i = EEPROM_MIN_ADDR; i <= EEPROM_MAX_ADDR; i++) {
    EEPROM.write(i, b);
  }
}


//
// Dump eeprom memory contents over serial port.
// For each byte, address and value are written.
//
void eeprom_serial_dump_column() {
  // counter
  int i;

  // byte read from eeprom
  byte b;

  // buffer used by sprintf
  char buf[10];

  for (i = EEPROM_MIN_ADDR; i <= EEPROM_MAX_ADDR; i++) {
    b = EEPROM.read(i);
    sprintf(buf, "%03X: %02X", i, b);
    Serial.println(buf);
  }
}


//
// Dump eeprom memory contents over serial port in tabular form.
// Each printed row shows the value of bytesPerRow bytes
// (by default 16).
//
void eeprom_serial_dump_table(int bytesPerRow = 16) {
  // address counter
  int i;

  // row bytes counter
  int j;

  // byte read from eeprom
  byte b;

  // temporary buffer for sprintf
  char buf[10];


  // initialize row counter
  j = 0;

  // go from first to last eeprom address
  for (i = EEPROM_MIN_ADDR; i <= EEPROM_MAX_ADDR; i++) {

    // if this is the first byte of the row,
    // start row by printing the byte address
    if (j == 0) {
      sprintf(buf, "%03X: ", i);
      Serial.print(buf);
    }

    // read current byte from eeprom
    b = EEPROM.read(i);

    // write byte in hex form
    sprintf(buf, "%02X ", b);

    // increment row counter
    j++;

    // if this is the last byte of the row,
    // reset row counter and use println()
    // to start a new line
    if (j == bytesPerRow) {
      j = 0;
      Serial.println(buf);
    }
    // else just print the hex value with print()
    else {
      Serial.print(buf);
    }
  }
}


//
// Returns true if the address is between the
// minimum and maximum allowed values,
// false otherwise.
//
// This function is used by the other, higher-level functions
// to prevent bugs and runtime errors due to invalid addresses.
//
boolean eeprom_is_addr_ok(int addr) {
  return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}


//
// Writes a sequence of bytes to eeprom starting at the specified address.
// Returns true if the whole array is successfully written.
// Returns false if the start or end addresses aren't between
// the minimum and maximum allowed values.
// When returning false, nothing gets written to eeprom.
//
boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes) {
  // counter
  int i;

  // both first byte and last byte addresses must fall within
  // the allowed range  
  if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
    return false;
  }

  for (i = 0; i < numBytes; i++) {
    EEPROM.write(startAddr + i, array[i]);
  }

  return true;
}


//
// Reads the specified number of bytes from the specified address into the provided buffer.
// Returns true if all the bytes are successfully read.
// Returns false if the star or end addresses aren't between
// the minimum and maximum allowed values.
// When returning false, the provided array is untouched.
//
// Note: the caller must ensure that array[] has enough space
// to store at most numBytes bytes.
//
boolean eeprom_read_bytes(int startAddr, byte array[], int numBytes) {
  int i;

  // both first byte and last byte addresses must fall within
  // the allowed range  
  if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
    return false;
  }

  for (i = 0; i < numBytes; i++) {
    array[i] = EEPROM.read(startAddr + i);
  }

  return true;
}


//
// Writes an int variable at the specified address.
// Returns true if the variable value is successfully written.
// Returns false if the specified address is outside the
// allowed range or too close to the maximum value
// to store all of the bytes (an int variable requires
// more than one byte).
//
boolean eeprom_write_int(int addr, int value) {
  byte *ptr;

  ptr = (byte*)&value;
  return eeprom_write_bytes(addr, ptr, sizeof(value));
}


//
// Reads an integer value at the specified address.
// Returns true if the variable is successfully read.
// Returns false if the specified address is outside the
// allowed range or too close to the maximum vlaue
// to hold all of the bytes (an int variable requires
// more than one byte).
//
boolean eeprom_read_int(int addr, int* value) {
  return eeprom_read_bytes(addr, (byte*)value, sizeof(int));
}


//
// Writes a string starting at the specified address.
// Returns true if the whole string is successfully written.
// Returns false if the address of one or more bytes
// fall outside the allowed range.
// If false is returned, nothing gets written to the eeprom.
//
boolean eeprom_write_string(int addr, const char* string) {
  // actual number of bytes to be written
  int numBytes;

  // we'll need to write the string contents
  // plus the string terminator byte (0x00)
  numBytes = strlen(string) + 1;

  return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}


//
// Reads a string starting from the specified address.
// Returns true if at least one byte (even only the
// string terminator one) is read.
// Returns false if the start address falls outside
// or declare buffer size os zero.
// the allowed range.
// The reading might stop for several reasons:
// - no more space in the provided buffer
// - last eeprom address reached
// - string terminator byte (0x00) encountered.
// The last condition is what should normally occur.
//
boolean eeprom_read_string(int addr, char* buffer, int bufSize) {
  // byte read from eeprom
  byte ch;

  // number of bytes read so far
  int bytesRead;

  // check start address
  if (!eeprom_is_addr_ok(addr)) {
    return false;
  }

  // how can we store bytes in an empty buffer ?
  if (bufSize == 0) {
    return false;
  }

  // is there is room for the string terminator only,
  // no reason to go further
  if (bufSize == 1) {
    buffer[0] = 0;
    return true;
  }

  // initialize byte counter
  bytesRead = 0;

  // read next byte from eeprom
  ch = EEPROM.read(addr + bytesRead);

  // store it into the user buffer
  buffer[bytesRead] = ch;

  // increment byte counter
  bytesRead++;

  // stop conditions:
  // - the character just read is the string terminator one (0x00)
  // - we have filled the user buffer
  // - we have reached the last eeprom address
  while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) ) {
    // if no stop condition is met, read the next byte from eeprom
    ch = EEPROM.read(addr + bytesRead);

    // store it into the user buffer
    buffer[bytesRead] = ch;

    // increment byte counter
    bytesRead++;
  }

  // make sure the user buffer has a string terminator
  // (0x00) as its last byte
  if ((ch != 0x00) && (bytesRead >= 1)) {
    buffer[bytesRead - 1] = 0;
  }

  return true;
}











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

  // Reads the current AES key from EEPROM
  unsigned char buffer[16];
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
  Serial.println("Dumping eeprom contents...");
  eeprom_serial_dump_table();

  Serial.print("Current counter value: ");

  // Reads the current ArduKey counter value from EEPROM
  unsigned int counter = ArduKeyEEPROM::getCounter();
  Serial.println(counter);




  //unsigned char* buffer = NULL; // WARUM geht das nicht?
  unsigned char buffer[16];


  ArduKeyEEPROM::getAESKey(buffer);

  // Hier ok:
  Serial.println(buffer[0], HEX);
  Serial.println(buffer[1], HEX);
  Serial.println(buffer[2], HEX);
  Serial.println(buffer[3], HEX);

  // Hier nicht:
  for (int i = 0; i < 16; i++)
  {
    Serial.print("0x");
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();


  ArduKeyUtilities::serialDump(buffer, 16);






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
