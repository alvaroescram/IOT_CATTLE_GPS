#include <Wire.h>
#include <SparkFunMLX90614.h>



IRTherm sensor;


float tempAmbiente;
float tempObjeto;

void setup()
{
  Serial.begin(115200);
  sensor.begin();

  sensor.setUnit(TEMP_C);
}

void loop() {
  if (sensor.read())
  {
    //recupera a leitura da temperatura do ambiente
    tempAmbiente = sensor.ambient();

    //recupera a leitura da temperatura do objeto apontado pelo sensor
    tempObjeto = sensor.object();
    Serial.printf("%f\n%f",tempAmbiente,tempObjeto);
    Serial.println();
delay(1000);
    
  }

  delay(1000);

}
