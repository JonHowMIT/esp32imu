float clamp(float x, float lowerlimit, float upperlimit) {
  if (x < lowerlimit)
    x = lowerlimit;
  if (x > upperlimit)
    x = upperlimit;
  return x;
}

float smootherstep(float edge0, float edge1, float x) {
  // Scale, and clamp x to 0..1 range
  x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
  // Evaluate polynomial
  return x * x * x * (x * (x * 6 - 15) + 10);
}

void allStop() {
  ledcWrite(ledChannel0, MIN_PULSE_LENGTH);
  ledcWrite(ledChannel1, MIN_PULSE_LENGTH);
}

void MotorInitialize(){
  // configure LED PWM functionalitites
  ledcSetup(ledChannel0, freq, resolution);
  ledcSetup(ledChannel1, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(M[0], ledChannel0);
  ledcAttachPin(M[1], ledChannel1);
  allStop();
  Serial.println("Init");
}

void ramp_up_motors(int step){
  for (int ii = MIN_PULSE_LENGTH; ii <= AVG_PULSE_LENGTH; ii+=step) { // ramp up
    Serial.println(ii);
    ledcWrite(ledChannel0, ii);
    ledcWrite(ledChannel1, ii);
    delay(200);
    //imu->getAGT();
    //accX = imu->accX();
    //accZ = imu->accZ();

    //float temp_angle = asin(accX/sqrt(accX*accX+accZ*accZ));
    //if (abs(temp_angle) > 60*DEG2RAD) {                         // safety??
    //  allStop();
    //  //digitalWrite(A9, 1);                            // LED High Once Armed
    //  //abort();
    //}
  }
}
