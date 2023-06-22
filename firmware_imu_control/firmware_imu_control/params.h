// computed constants
static constexpr double DEG2RAD = M_PI/180.;
static constexpr float g = 9.80665f;

//=============================================================================
// initialize
//=============================================================================
//esp32 PWM check
const int M[5] = {27, 26, 25, 33, 32}; // 35?
// setting PWM properties
const int freq = 500;
const int ledChannel0 = 0;
const int ledChannel1 = 1;
const int resolution = 12;
const int Nscale = pow(2,resolution);

#define MIN_PULSE_LENGTH int(0.5*Nscale) // Minimum duty cycle
#define MAX_PULSE_LENGTH int(Nscale) // Maximum duty cycle
int AVG_PULSE_LENGTH = int((MIN_PULSE_LENGTH + MAX_PULSE_LENGTH)/2.0);

// sensor polling interval (micros)
uint32_t SENSOR_POLL_INTERVAL_US = 2500; // default, can be changed online
float dt = 1; // actual time step beign used - changed in the loop to what set by SENSOR_POLL_INTERVAL_US
// timing
uint32_t sensor_poll_previous_us = 0;
uint32_t start_time_us = 0;

// PID control
float error = 0.0, int_error = 0.0;
float Kp = 200;                                             // 400 pos gain
float Kd = 1.5*Kp/5.0;                                             // 400 pos gain
float U = 0.0;
float Ki = 30;                                              // integ gain
float KK = 0.99; // sensor mixing param
float MPC_value = 0;

int Ncalib = 1000; // calibration counter
float theta_initial = 0.0, theta_est =0.0, gyro_bias = 0.0, theta_meas = 0.;
float accX = 0.0, accZ = 0.0, gyrY = 0.0;
float Ignore_Int_Angle = 25*DEG2RAD;
float Ignore_Accel_Angle = 45*DEG2RAD;

float Ref = 0.0; // will change
float Ref_start_delay = 0.0; // time when button pushed 
// Ramp params
float T_ramp = 0.1; // time for ramp in seconds
float T_ramp_start = 0.005; // delay how long after button push
float edge0 = Ref_start_delay + T_ramp_start;
float edge01 = edge0 + T_ramp;         
float edge1 = edge01 + 2.0;         
float edge11 = edge1 + T_ramp;         
float edge2 = edge11 + 2.0;         
float edge21 = edge2 + T_ramp;         

unsigned ARMValue = 0; // arms motors to spin
unsigned ARMtol = 1800; // arms motors to spin
unsigned CLValue = 0; // closes loop
unsigned CLtol = 1800; // closes loop
unsigned RAMPtol = 1300; // starts ramp

float OL_amp = 20.0;
float OL_freq = 13.5; // rad/sec