<?php

# convert a string into a key
# key is specified using hexadecimal
$key_hexstring = "06000000000000000000000000000006";
$key = pack('H*', $key_hexstring);


$key_size = strlen($key);
echo "Key size: " . $key_size . "\n";


$plaintext = pack('H*', "AA4481EC3CC627BACD5DC3FB08F273E6");
var_dump(unpack('H*', $plaintext));




$ciphertext = mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key, $plaintext, MCRYPT_MODE_ECB);
var_dump(unpack('H*', $ciphertext));


$plaintext_dec = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $ciphertext, MCRYPT_MODE_ECB);
var_dump(unpack('H*', $plaintext_dec));


?>