//esp32 PWM check
const int M[5] = {27, 26, 25, 33, 32}; // 35?

// the number of the LED pin
//const int ledPin = 16;  // 16 corresponds to GPIO16
//const int ledPin2 = 17; // 17 corresponds to GPIO17
//const int ledPin3 = 5;  // 5 corresponds to GPIO5

// setting PWM properties
const int freq = 500;
const int ledChannel0 = 0;
const int ledChannel1 = 1;
const int resolution = 12;
 
void setup(){
  Serial.begin(115200);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel0, freq, resolution);
  ledcSetup(ledChannel1, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(M[0], ledChannel0);
  ledcAttachPin(M[1], ledChannel1);
  ledcWrite(ledChannel0, 2048);
  ledcWrite(ledChannel1, 2048);
  delay(1000);
}
 
void loop(){
  // increase the LED brightness
  for(int dutyCycle = 2048; dutyCycle <= 2800; dutyCycle++){   
    Serial.println(dutyCycle);
    // changing the LED brightness with PWM
    ledcWrite(ledChannel0, dutyCycle);
    ledcWrite(ledChannel1, dutyCycle);
    delay(15);
  }
}