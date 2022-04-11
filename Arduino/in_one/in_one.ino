#include <RadioLib.h>

// SX1276 has the following connections:
// NSS pin:   5
// DIO0 pin:  15
// RESET pin: 4
// DIO1 pin:  2
SX1276 radio = new Module(5, 15, 4, 2);

//Button
int in = 26;
int out = 14;
// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

// Device Specific Rescue
String rescue = "Module ID \ Current Time \ GPS Location";

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}

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
  //delay(15000);
}

void beacon(String info){
  // Testing Beacon
  int offset = 1;
  int count = 0;
  
  while (count < 100){
    mit(info);
    delay(120000 * offset);
    offset += 2;
  }

  // Final Beacon
  //   int offset = 1;
  //   int count = 0;
      // About 3 days of beacon
      // Starting at 6 min to 66 min intervals
  //    while (count < 68){
  //      mit(info);
  //      delay(360000 * offset);
  //      if (offset < 10) offset += 2;
  //    }
     
   
}


void setup() {
  Serial.begin(9600);
  
  //Button Setup
  pinMode(in, INPUT);
  digitalWrite(out, HIGH);
  pinMode(13, OUTPUT);

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
  // NOTE: 20 dBm value allows high power operation, but transmission
  //       duty cycle MUST NOT exceed 1%
  int pin_rx_enable = 22;
  int pin_tx_enable = 1;
 if (radio.setOutputPower(20) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("Selected output power is invalid for this module!"));
//    while (true);
  }

  // listen interups setup
  radio.setDio0Action(setFlag);
  state = radio.startReceive();

}
bool light = true;

void loop() {
  // put your main code here, to run repeatedly:
  if (light){
    digitalWrite(13, HIGH);
    light = false;
  } else {
    digitalWrite(13, LOW);
    light = true;
  }

  // Buton detection
   buttonState = digitalRead(in);
   if (buttonPushCounter >= 20){
    beacon(rescue);
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

  if(receivedFlag) {
    // disable the interrupt service routine while
    // processing the data
    enableInterrupt = false;

    // reset flag
    receivedFlag = false;

    // you can read received data as an Arduino String
    String str;
    int state = radio.readData(str);

    if (state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      Serial.println(F("[SX1278] Received packet!"));

      // print data of the packet
      Serial.print(F("[SX1278] Data:\t\t"));
      Serial.println(str);

      // print RSSI (Received Signal Strength Indicator)
      Serial.print(F("[SX1278] RSSI:\t\t"));
      Serial.print(radio.getRSSI());
      Serial.println(F(" dBm"));

      // print SNR (Signal-to-Noise Ratio)
      Serial.print(F("[SX1278] SNR:\t\t"));
      Serial.print(radio.getSNR());
      Serial.println(F(" dB"));

      // print frequency error
      Serial.print(F("[SX1278] Frequency error:\t"));
      Serial.print(radio.getFrequencyError());
      Serial.println(F(" Hz"));
      beacon(str);

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println(F("[SX1278] CRC error!"));

    } else {
      // some other error occurred
      Serial.print(F("[SX1278] Failed, code "));
      Serial.println(state);

    }

    // put module back to listen mode
    radio.startReceive();

    // we're ready to receive more packets,
    // enable interrupt service routine
    enableInterrupt = true;
  }
  delay(250);
}
