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

void setup() {
  pinMode(in, INPUT);
  digitalWrite(out, HIGH);
  
  
  Serial.begin(9600);

  // initialize SX1276
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin(915.0, 125.0, 9, 7, 'SX127X_SYNC_WORD', 10, 8, 0);  
  
  if (state == RADIOLIB_ERR_NONE) {
   Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  lis();
}

void sos(void * pvParameters){
  
}
