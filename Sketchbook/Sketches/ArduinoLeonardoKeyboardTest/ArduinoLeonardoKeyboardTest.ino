int buttonPin = 2;
int previousButtonState = HIGH;

void setup()
{
  // Starts serial connection
  Serial.begin(9600);

  // Configures pin as an input and enable the internal 20K Ohm pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Initializes control over the keyboard
  Keyboard.begin();
}

void loop()
{
  // Gets current button state
  int buttonState = digitalRead(buttonPin);

  if ( buttonState != previousButtonState && buttonState == HIGH )
  {
    Keyboard.print("ArduKey");
    // Keyboard.write(KEY_RETURN);
  }
  
  previousButtonState = buttonState;
}



