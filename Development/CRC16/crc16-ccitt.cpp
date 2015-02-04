/*
 * Calculate the CRC16 (CRC-CCITT "0xFFFF") checksum of given data.
 *
 * Thanks to: "Antonio Pires" on https://stackoverflow.com/questions/10564491
 * Thanks to: "ckielstra" on http://www.ccsinfo.com/forum/viewtopic.php?t=24977
 *
 * @args values: The array to calculate checksum of.
 * @args length: The length of processed data.
 * @return uint16_t
 *
 */
uint16_t calculateCRC16(const uint8_t values[], size_t length)
{
    uint16_t crc = 0xFFFF;

    // Pointer sanity check
    if ( !values || length == 0 )
    {
        return crc;
    }

    uint8_t x;

    for (int i = 0; i < length; i++)
    {
        x = (crc >> 8) ^ values[i];
        x = x ^ (x >> 4);

        crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
        crc = crc & 0xFFFF;
    }

    return crc;
}
