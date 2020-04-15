/**************************************************************
 *
 * For this example, you need to install PubSubClient library:
 *   https://github.com/knolleary/pubsubclient
 *   or from http://librarymanager/all#PubSubClient
 *
 * TinyGSM Getting Started guide:
 *   https://tiny.cc/tinygsm-readme
 *
 * For more MQTT examples, see PubSubClient library
 *
 **************************************************************
 * Use Mosquitto client tools to work with MQTT
 *   Ubuntu/Linux: sudo apt-get install mosquitto-clients
 *   Windows:      https://mosquitto.org/download/
 *
 * Subscribe for messages:
 *   mosquitto_sub -h test.mosquitto.org -t GsmClientTest/init -t GsmClientTest/ledStatus -q 1
 * Toggle led:
 *   mosquitto_pub -h test.mosquitto.org -t GsmClientTest/led -q 1 -m "toggle"
 *
 * You can use Node-RED for wiring together MQTT-enabled devices
 *   https://nodered.org/
 * Also, take a look at these additional Node-RED modules:
 *   node-red-contrib-blynk-ws
 *   node-red-dashboard
 *
 **************************************************************/

const char* MQTT_ClientID = "Vaca1";
const char* MQTT_User = "User";
 const char* MQTT_Pass = "L3TQCJAM35A8214L";
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



// modem:
#define TINY_GSM_MODEM_SIM800

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

// Define how you're planning to connect to the internet
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[] = "internet.itelcel.com";
const char gprsUser[] = "webgprs";
const char gprsPass[] = "webgprs2002";

// MQTT details
const char* broker = "mqtt.thingspeak.com";

const char* topicPing = "channels/1035049/publish/PKW6O1Z5NDUXYN4J";
const char* topicInit = "GsmClientTest/init";
//const char* topicLedStatus = "GsmClientTest/ledStatus";
//const char* topicLED = "GsmClientTest/ping";

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <Wire.h>

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

TwoWire I2CPower = TwoWire(0);

#define uS_TO_S_FACTOR 1000000     /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300        /* Time ESP32 will go to sleep (in seconds) 3600 seconds = 1 hour */

long lastReconnectAttempt = 0;
long lastPing = 0;
unsigned long t;

#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

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

//void mqttCallback(char* topic, byte* payload, unsigned int len) {
//  SerialMon.print("Message arrived [");
//  SerialMon.print(topic);
//  SerialMon.print("]: ");
//  SerialMon.write(payload, len);
//  SerialMon.println();
//
//  // Only proceed if incoming message's topic matches
//  if (String(topic) == topicLed) {
//    ledStatus = !ledStatus;
//    digitalWrite(LED_PIN, ledStatus);
//    mqtt.publish(topicLedStatus, ledStatus ? "1" : "0");
//  }
//}
boolean publishAll(){
  SerialMon.println("sending data to 5 topics in ThingSpeak");
  return mqtt.publish(topicPing, "field1=100&field2=1&field3=10&field4=100&field5=1000&status=MQTTPUBLISH");
}


//
//
//
//
//
boolean mqttConnect() {
  static uint8_t tries =0;
  SerialMon.print("Connecting to ");
  SerialMon.print(broker);

  // Connect to MQTT Broker
  //boolean status = mqtt.connect("GsmClientTest");

  // Or, if you want to authenticate MQTT:
//  boolean status = mqtt.connect(MQTT_ClientID, MQTT_User, MQTT_Pass);
    boolean status = mqtt.connect("Vaca1","user","L3TQCJAM35A8214L");

  if (status == false) {
    SerialMon.println(" fail");
    tries++;
    if(tries>=5){
      Serial.println("Failed to establish MQTT conexion rebooting system");
      delay(1000);
      ESP.restart();
    }
    return false;
  }
  tries=0;
  SerialMon.println(" success");
  mqtt.publish(topicInit, "Vaca1 started");
  //mqtt.subscribe(topicLed);
  return mqtt.connected();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // Set console baud rate
  SerialMon.begin(115200);
  //delay(10);

//  pinMode(LED_PIN, OUTPUT);

  // !!!!!!!!!!!
  // Set your reset, enable, power pins here
  // !!!!!!!!!!!
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);
  //I2CBME.begin(I2C_SDA_2, I2C_SCL_2, 400000);

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
  delay(1000);


  SerialMon.println("Wait...");

  // Set GSM module baud rate
  // TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);
  //SerialAT.begin(9600);
  //delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
   //modem.init();

  
#if TINY_GSM_USE_GPRS
  // Unlock your SIM card with a PIN if needed
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
  }
#endif

//#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
//  // The XBee must run the gprsConnect function BEFORE waiting for network!
  modem.gprsConnect(apn, gprsUser, gprsPass);
//#endif

  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    ESP.restart();
  }
  SerialMon.println(" success");

  if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
  }

//#if TINY_GSM_USE_GPRS
//  // GPRS connection parameters are usually set after network registration
//    SerialMon.print(F("Connecting to "));
//    SerialMon.print(apn);
//    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
//      SerialMon.println(" fail");
//      delay(10000);
//      ESP.restart();
//    }
//    SerialMon.println(" success");

  if (modem.isGprsConnected()) {
    SerialMon.println("GPRS connected");
  }
//#endif

  // MQTT Broker setup
  mqtt.setServer(broker, 1883);
  //mqtt.setCallback(mqttCallback);

   
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  //FALGS
  static bool dataSent =false;
  static unsigned long startMillis =0;
  static bool isMQTTConnected = true;
  static unsigned long lastReconnectedAttempt =0;
   
  ////////////////////////////////
  if (!modem.isGprsConnected()) {
    uint8_t GPRS_tries =0; //fail flags
    SerialMon.print(F("Gprs failed\n trying to connect to..."));
    SerialMon.print(apn);
    while (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
      if(GPRS_tries == 3){
        SerialMon.println(F("Initializing modem..."));
        modem.init();
      }else if(GPRS_tries >=5){
        SerialMon.println("Failed to stablish connection to"+String(apn)+"rebooting system in 3 seconds");
        SerialMon.flush();
        delay(3000);
        ESP.restart();
      }else{
        SerialMon.println("fail("+String(GPRS_tries)+") retrying in 3 sec");
        delay(3000);
        GPRS_tries++;
      }
    }
  }else
  {
    isMQTTConnected = mqtt.connected();
    if (!isMQTTConnected) 
    {
      // Reconnect every 10 seconds
      
      if((millis()-lastReconnectedAttempt >= 10000L))
      {
        SerialMon.println("MQTT not connected");
        lastReconnectedAttempt = millis();
        if (mqttConnect()) 
        {
          if(publishAll()){
            SerialMon.println("delivered\n System Entering Deep Sleep ");
            mqtt.disconnect();
            delay(3000);
            
            
            esp_deep_sleep_start();
        
          }else
          {
             SerialMon.println("Failed to deliver");
             delay(2000);
            
          }
        }
      }
      
      delay(100);
    }
  }
 mqtt.loop();
}
