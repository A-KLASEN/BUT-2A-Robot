#include <FrequencyTimer2.h>

void setup() {
  
  pinMode(11,OUTPUT);
  FrequencyTimer2::setPeriod(28);
  FrequencyTimer2::enable();

}

void loop() {

}
