#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
#define TIME_TO_SLEEP 302 /* Time ESP32 will go to sleep (in seconds) = 5min*/ 

RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
 esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
 Serial.flush(); 
  esp_deep_sleep_start();
 
}

void loop() {
  // put your main code here, to run repeatedly:

}
