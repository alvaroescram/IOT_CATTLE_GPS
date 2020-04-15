//#includes
//gy-906
#include <Wire.h>
#include <Adafruit_MLX90614.h>
//DHT11
#include <DHT.h>
//GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//#defines
//gy-906
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//DHT11
#define DHT11_PIN 13
#define DHTTYPE    DHT11     // DHT 11
DHT dht(DHT11_PIN, DHTTYPE);
//NEO-6M
#define GPSTX_PIN 0
#define GPSRX_PIN 4
TinyGPSPlus gps;
SoftwareSerial  SerialGPS(GPSRX_PIN, GPSTX_PIN);



void setup() {
 
  Serial.begin(115200);

  dht.begin();
  mlx.begin();
  SerialGPS.begin(9600);
  delay(250);
   
}

void loop() {
  static bool GPS_CHANGED = false;

  while (SerialGPS.available() > 0){
    gps.encode(SerialGPS.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= ");   
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
      GPS_CHANGED =true;
    }
    
  }
  if(GPS_CHANGED){
  // put your main code here, to run repeatedly:
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
   Serial.println("Temperature:" + String(dht.readTemperature()) +"*C\t" + String(dht.readHumidity()) + "% humidity");
  delay(2500);
  GPS_CHANGED = false;
  }

}
