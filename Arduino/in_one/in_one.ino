#include <RadioLib.h>

// SX1276 has the following connections:
// NSS pin:   5
// DIO0 pin:  15
// RESET pin: 4
// DIO1 pin:  2
SX1276 radio = new Module(5, 15, 4, 2);

// Task Handelers
TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
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

  //Setup Both Cores
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    sos,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

void loop() {
  // put your main code here, to run repeatedly:

}

void sos(void * pvParameters){
  Serial.print(F("[SX1276] Transmitting packet ... "));
  
  char output[50];
  sprintf(output, "Counter: %d", counter++);
  Serial.print(output);
  int state = radio.transmit(output);

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

  // wait for a second before transmitting again
  delay(1000);
}
