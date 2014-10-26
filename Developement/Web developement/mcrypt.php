<?php

# convert a string into a key
# key is specified using hexadecimal
$key_hexstring = "06000000000000000000000000000006";
$key = pack('H*', $key_hexstring);

$key_size = strlen($key);
echo "Key size: " . $key_size . "\n";


/*
$input = pack('H*', "CCCCCCCCCCCC07000000EEFFBBAA0A86");
var_dump(unpack('H*', $input));

$output = mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key, $input, MCRYPT_MODE_ECB);
var_dump(unpack('H*', $output));

$input = $output;

*/

$input = pack('H*', "bfb60ed19ed7f2ed644563dd3bbb952b");
var_dump(unpack('H*', $input));


$output = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $input, MCRYPT_MODE_ECB);
var_dump(unpack('H*', $output));





?>