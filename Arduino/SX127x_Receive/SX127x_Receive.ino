/*
   RadioLib SX127x Receive Example

   This example listens for LoRa transmissions using SX127x Lora modules.
   To successfully receive data, the following settings have to be the same
   on both transmitter and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word
    - preamble length

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1276 has the following connections:
// NSS pin:   5
// DIO0 pin:  15
// RESET pin: 4
// DIO1 pin:  2
SX1276 radio = new Module(5, 15, 4, 2);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;

bool flag = true;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  // initialize SX1276 with default settings
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin(915.0, 31.25, 10, 7, 'SX127X_SYNC_WORD', 10, 17, 0);  
  if (state == RADIOLIB_ERR_NONE) {
   Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  //Power Setup
  int pin_rx_enable = 22;
  int pin_tx_enable = 1;
  radio.setRfSwitchPins(pin_rx_enable, pin_tx_enable);

  // set output power to 10 dBm (accepted range is -3 - 17 dBm)
  // NOTE: 20 dBm value allows high power operation, but transmission
  //       duty cycle MUST NOT exceed 1%
  /*
  if (radio.setOutputPower(20) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("Selected output power is invalid for this module!"));
    while (true);
  }
  */
  
}

void loop() {
  Serial.print(F("[SX1276] Waiting for incoming transmission ... "));
  if (flag){
    digitalWrite(13, HIGH);
    flag = false;
  } else {
    digitalWrite(13, LOW);
    flag = true;
  }

  // you can receive data as an Arduino String
  // NOTE: receive() is a blocking method!
  //       See example ReceiveInterrupt for details
  //       on non-blocking reception method.
  String str;
  int state = radio.receive(str);

  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = radio.receive(byteArr, 8);
  */

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
