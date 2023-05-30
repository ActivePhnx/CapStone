#include "types.h"
#include "TinyGPS++.h";
#include "HardwareSerial.h";

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

void setup() {
        Serial.begin(115200); //Serial port of USB
        SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
}

void loop() {
    while (SerialGPS.available() >0) {
       gps.encode(SerialGPS.read());
    }

    Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
    Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
    Serial.print("ALT=");  Serial.println(gps.altitude.meters());
}
