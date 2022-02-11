#include <RadioLib.h>

// SX1276 has the following connections:
// NSS pin:   5
// DIO0 pin:  15
// RESET pin: 4
// DIO1 pin:  2
SX1276 radio = new Module(5, 15, 4, 2);

// Task Handelers
//TaskHandle_t Task1;
//TaskHandle_t Task2;

//Button
int in = 26;
int out = 14;

String rescue = "Module ID \ Current Time";

void mit(String info){
  Serial.print(F("[SX1276] Transmitting packet ... "));

  Serial.print(info);
  int state = radio.transmit(info);

  // Testing
  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F(" success!"));

    // print measured data rate
    Serial.print(F("[SX1276] Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(F(" bps"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // timeout occurred while transmitting packet
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  //Temp Testing Delay
  delay(1000);
}

void lis(){
  Serial.print(F("[SX1276] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
  // NOTE: receive() is a blocking method!
  //       See example ReceiveInterrupt for details
  //       on non-blocking reception method.
  String str;
  int state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[SX1276] Data:\t\t\t"));
    Serial.println(str);

    // print the RSSI (Received Signal Strength Indicator)
    // of the last received packet
    Serial.print(F("[SX1276] RSSI:\t\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print the SNR (Signal-to-Noise Ratio)
    // of the last received packet
    Serial.print(F("[SX1276] SNR:\t\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error
    // of the last received packet
    Serial.print(F("[SX1276] Frequency error:\t"));
    Serial.print(radio.getFrequencyError());
    Serial.println(F(" Hz"));

  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));

  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
}

void butPress(){
  int buttonPushCounter = 0;   // counter for the number of button presses
  int buttonState = 0;         // current state of the button
  int lastButtonState = 0;     // previous state of the button

  while (true){
    buttonState = digitalRead(in);

    if (buttonPushCounter >= 20){
      mit(rescue);
    }
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      buttonPushCounter = 0;
      lastButtonState = 0;
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  
}

void setup() {
  Serial.begin(9600);
  //Button Setup
  pinMode(in, INPUT);
  digitalWrite(out, HIGH);

  // initialize SX1276
  Serial.print(F("[SX1276] Initializing ... "));
  // (frequency, Bandwidth, Spreading Factor, coding rate, sync word, power, preamble length, gain) 
  int state = radio.begin(915.0, 31.25, 10, 7, 'SX127X_SYNC_WORD', 10, 17, 0);  
  
  if (state == RADIOLIB_ERR_NONE) {
   Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
  

  //Set To High Power Output
//  int pin_rx_enable = 22;
//  int pin_tx_enable = 1;
// if (radio.setOutputPower(20) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
//    Serial.println(F("Selected output power is invalid for this module!"));
//    while (true);
//  }
}

void loop() {
  // put your main code here, to run repeatedly:
  butPress();
}
