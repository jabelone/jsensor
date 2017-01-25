//Note that when the esp comes out of deep sleep mode it will begin executing code like it was first powered on.
//Therefore, loop() should never be reached but it will continue "looping" through setup().

const int sleepTimeS = 10;
#define SECONDS_DS(seconds) ((seconds)*1000000UL)

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP is about to go into deep sleep mode");
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop() {
  
  
  }
