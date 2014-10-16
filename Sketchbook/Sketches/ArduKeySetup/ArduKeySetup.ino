#include <ArduKey.h>


/*
 * The setup method.
 * 
 * @return void
 *
 */
void setup()
{
  unsigned char buffer[];

  /*
   * The new AES key.
   *
   */
  buffer[] = 
  {
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
  };
  ArduKeyEEPROM::setAESKey(buffer);

  // Resets counter value
  ArduKeyEEPROM::setCounter(0);
}

/*
 * The loop method.
 * 
 * @return void
 *
 */
void loop() 
{
}
