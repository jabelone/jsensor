//Note that when the esp comes out of deep sleep mode it will begin executing code like it was first powered on.
//Therefore, loop() should never be reached but it will continue "looping" through setup().

extern "C" {
  #include "user_interface.h"
}

const int sleepTimeS = 10;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP is about to go into deep sleep mode");
  system_deep_sleep(sleepTimeS * 1000000);
}

void loop() {
  
  
  }
