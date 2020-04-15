#include <TinyGPS++.h>
#include <SoftwareSerial.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

#define GPSTX_PIN 0
#define GPSRX_PIN 4


TinyGPSPlus gps;



SoftwareSerial  SerialGPS(GPSRX_PIN, GPSTX_PIN);

void setup()
{
  Serial.begin(115200);
  SerialGPS.begin(9600);
  
  Serial.println();
}

void loop()
{
 // This sketch displays information every time a new sentence is correctly encoded.
  while (SerialGPS.available() > 0){
    byte rawData = SerialGPS.read();
    Serial.write(rawData);
    /*
    gps.encode(SerialGPS.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= ");   
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
    }
    */
  }
 
  
}
