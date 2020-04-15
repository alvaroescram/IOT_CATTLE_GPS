//deepSleep
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
#define TIME_TO_SLEEP 302 /* Time ESP32 will go to sleep (in seconds) = 5min*/ 


// Your GPRS credentials (leave empty, if not needed)
const char apn[]      = "internet.itelcel.com"; // APN (example: internet.vodafone.pt) use https://wiki.apnchanger.org
const char gprsUser[] = "webgprs"; // GPRS User
const char gprsPass[] = "webgprs2002"; // GPRS Password

// SIM card PIN (leave empty, if not defined)
const char simPIN[]   = ""; 

// TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22
// BME280 pins
#define I2C_SDA_2            18
#define I2C_SCL_2            19


// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
//#define DUMP_AT_COMMANDS

#include <Wire.h>
#include <TinyGsmClient.h>
#include <PubSubClient.h>




// I2C for SIM800 (to keep it running when powered from battery)
TwoWire I2CPower = TwoWire(0);

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
//PubSubClient mqtt(client);


#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00



/////mqtt
char mqttUserName[] = "Alvaro";  // Use any name.
char mqttPass[] = "L3TQCJAM35A8214L";      // Change to your MQTT API Key from Account > MyProfile.   
char writeAPIKey[] = "PKW6O1Z5NDUXYN4J";    // Change to your channel write API key.
long channelID = 1035049;                    // Change to your channel ID.
int fieldNumber = 4;                        // Field to use is publishing to only one field.

static const char alphanum[] ="0123456789"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz";  // For random generation of client ID.

PubSubClient mqttClient(client); // Initialize the PubSubClient library.
const char* server = "mqtt.thingspeak.com"; 

unsigned long lastConnectionTime = 0; 
const unsigned long postingInterval = 20L * 1000L; // Post data every 20 seconds.



bool setPowerBoostKeepOn(int en){
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35); // 0x37 is default reg value
  }
  return I2CPower.endTransmission() == 0;
}

void reconnect() 
{
  char clientID[] = "f71abecb1b6f4c0c8b083155b2e7491e";

  // Loop until reconnected.
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Generate ClientID

    // Connect to the MQTT broker.
    if (mqttClient.connect(clientID,mqttUserName,mqttPass)) 
    {
      Serial.println("connected");
    } else 
    {
      Serial.print("failed, rc=");
      // Print reason the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttPublishFeed() {
  
  //float t = dht.readTemperature(true); // Read temperature from DHT sensor.
  //float h = dht.readHumidity();  // Read humidity from DHT sensor.
  //int lightLevel = analogRead(LIGHTPIN); // Read voltage from light sensor.
  
  // Create data string to send to ThingSpeak.
  //String data = String("field1=") + String(1,DEC) + "&field2=" + String(1.2, 2) + "&field3=" + String(3.9, 2);
  String data ="field1=1.2&field2=2.2&field3=3.3&field4=4.4&field5=5.5&status=MQTTPUBLISH";
  int length = data.length();
  const char *msgBuffer;
  msgBuffer=data.c_str();
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  const char* topicString = "channels/1035049/publish/PKW6O1Z5NDUXYN4J";
  
  
  mqttClient.publish( topicString, msgBuffer );
  lastConnectionTime = millis();
}

void setup() {
  // put your setup code here, to run once:
  SerialMon.begin(115200);

  // Start I2C communication
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);
  // Keep power when running from battery
  bool isOk = setPowerBoostKeepOn(1);
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));


  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
   // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX); 
  delay(3000);
    

  
 SerialMon.println("Initializing modem...");
  modem.restart();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
  }
  // GPRS connection parameters are usually set after network registration
    SerialMon.print(F("Connecting to "));
    SerialMon.print(apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
      SerialMon.println(" fail");
      delay(10000);
      return;
    }
    SerialMon.println(" success");

  if (modem.isGprsConnected()) {
    SerialMon.println("GPRS connected");
  }

  

  

mqttClient.setServer(server, 1883);   // Set the MQTT broker details.
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
}






void loop() {
  if (!mqttClient.connected()) 
  {
    reconnect();
  }

  mqttClient.loop();   // Call the loop continuously to establish connection to the server.

  // If interval time has passed since the last connection, publish data to ThingSpeak.

   mqttPublishFeed(); // Publish three values simultaneously.
    // mqttPublishField(fieldNumber); // Use this function to publish to a single field

    
  
  //if evrything ok and published
  mqttClient.disconnect();
  modem.gprsDisconnect();
  esp_deep_sleep_start();
  }
