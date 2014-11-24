<?php

/*
 * Example OTP:
 * clhbvedrgtnfubdnivcnblugglkrbcjdeuijivfjkddg
 *
 * Decoded OTP (Modhex -> hex):
 * Use https://demo.yubico.com/modhex.php for Modhex decoding
 * 0a61f32c5db4e12b7f0b1ae55a9c10823e787f489225
 *
 * Public id: 0a61f32c5db4
 * Encrypted token: e12b7f0b1ae55a9c10823e787f489225
 *
 */

// The 128 bit key in hexadecimal style:
$key_hexstring = "7A1858592FCB76BD5EB2685421AED45E";
$key = pack('H*', $key_hexstring);

echo "Key size: " . strlen($key) . "\n";

// The encrypted token in hexadecimal style:
$input = pack('H*', "e12b7f0b1ae55a9c10823e787f489225");
var_dump(unpack('H*', $input));

$output = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $input, MCRYPT_MODE_ECB);
var_dump(unpack('H*', $output));

?>
