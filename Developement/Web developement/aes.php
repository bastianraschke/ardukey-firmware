<?php











include('aes-cbc.php');
$aes = new SH_AES_Cipher();


$key = '0011223344556677';
$plaintext = 'The quick brown fox jumped over the lazy cat';
$iv =''; //Random (unpredictable!) IV 128 bits.

$ciphertext = $aes->encrypt($plaintext,$key,$iv,'');
$original =$aes->decrypt($ciphertext,$key,$iv);

var_dump($ciphertext);
var_dump($original);



/*
$aes->setup($key);

//Pad plaintext so it fills block
//$plaintext = $this->pad($plaintext);

$ciphertext = '';

//Split the plaintext into blocks
$blocks = str_split($plaintext, $this->block_size);

$key = array_values(unpack('C*', $key)); //Array of bytes



$this->key_expansion($key);

$block = array_values(unpack('C*', $block));
$block = $this->encryptBlock($block);

$xor = array_values(unpack('C*',$iv));
foreach ($blocks as $block){
    $block = array_values(unpack('C*', $block));

    for($i=0; $i< count($block); $i++){
        $block[$i] = $xor[$i]^$block[$i];
    }

    $block = $this->encryptBlock($block);
    foreach($block as $byte )
        $ciphertext .=pack('C', $byte);

    $xor =  array_values($block);
}

*/





















/*

function strtohex($x)
{
    $s='';
    foreach (str_split($x) as $c) $s.=sprintf("0x%02X ",ord($c));
    return($s);
}

#129

$k = 0x0;

var_dump(strtohex($k));

$offset = 0;

var_dump(unpack("@$offset/N4", "1234"));

echo bin2hex(base64_decode(openssl_encrypt(1, 'aes-128', 0xa, OPENSSL_ZERO_PADDING, '')));

exit;


$source = 'It works !';

$iv = "1234567812345678";
$pass = '1234567812345678';
$method = 'aes-128-cbc';



echo "\niv in hex to use: ".strtohex ($iv);
echo "\nkey in hex to use: ".strtohex ($pass);
echo "\n";

file_put_contents ('./file.encrypted',openssl_encrypt ($source, $method, $pass, true, $iv));

$exec = "openssl enc -".$method." -d -in file.encrypted -nosalt -nopad -K ".strtohex($pass)." -iv ".strtohex($iv);

echo 'executing: '.$exec."\n\n";
echo exec ($exec);
echo "\n";

*/