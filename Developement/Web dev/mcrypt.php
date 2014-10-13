<?php

# convert a string into a key
# key is specified using hexadecimal
$key = pack('H*', "01000000000000000000000000000000");

$key_size =  strlen($key);
echo "Key size: " . $key_size . "\n";


$plaintext = pack('H*', "AA4481ec3cc627bacd5dc3fb08f273e6");
var_dump(unpack('H*', $plaintext));



$iv = pack('H*', "00000000000000000000000000000000");


$ciphertext = mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key, $plaintext, MCRYPT_MODE_CBC, $iv);
var_dump(unpack('H*', $ciphertext));


$plaintext_dec = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $ciphertext, MCRYPT_MODE_CBC, $iv);
var_dump(unpack('H*', $plaintext_dec));


?>