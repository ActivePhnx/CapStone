// COM 8

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // print out the state of the button:
  Serial.println("This is on Com 8");
  delay(500);        // delay in between reads for stability
}
