#ifndef __ARDUKEY_EEPROM_H__
#define __ARDUKEY_EEPROM_H__

// Limit for Arduino EEPROM (may be different on the Arduino models).
#define EEPROM_MIN_ADDRESS	0
#define EEPROM_MAX_ADDRESS	511


#define EEPROM_AESKEY_POS		0
#define EEPROM_AESKEY_LEN		16

#define EEPROM_COUNTER_POS	(EEPROM_AESKEY_POS + EEPROM_AESKEY_LEN)
#define EEPROM_COUNTER_LEN	2


class ArduKeyEEPROM
{
  public:

  	static void		dumpTable();

    static bool		getBytes(int startAddress, unsigned char* ptr, int length);
    static bool		setBytes(int startAddress, unsigned char* ptr, int length);

    static bool		getAESKey(unsigned char buffer[]);
    static bool		setAESKey(unsigned char values[]);

    static unsigned int		getCounter();
    static void				setCounter(unsigned int);

  private:
    static bool isAddressOkay(int address);
};


#endif