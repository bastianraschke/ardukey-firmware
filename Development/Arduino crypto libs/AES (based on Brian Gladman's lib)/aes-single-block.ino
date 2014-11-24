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
  serialDump(plain, N_BLOCK);


  // Encrypting plain text
  if ( aes.encrypt(plain, cipher) != 0 )
  {
    Serial.println("Error: AES encryption process failed!");
  }
  serialDump(cipher, N_BLOCK);

  // Decrypting cipher text
  if ( aes.decrypt(cipher, new_plain) != 0 )
  {
    Serial.println("Error: AES decryption process failed!");
  }
  serialDump(new_plain, N_BLOCK);


  Serial.println();
  Serial.println();
  delay(5000);
}
