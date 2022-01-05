#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(uint8_t p, float v): pin(p), vref(v){};
void KalmanFilter::burn(){
   for(int i = 0; i < 5; i++){
    prior = analogRead(pin) * vref * 1000 / bitres;
    delay(200);
  }
  post = prior;
  delay(500);
}

double KalmanFilter::getPPM(){
   //predict step
  prior = post + Q;//compute prior
  P = P + Q;//adjust belief  
  //update step
  rawVoltage = analogRead(pin) * vref * 1000 / bitres;//get sensor reading
  
  residual = rawVoltage - prior;//compute residual
  K = P / (P + R);//compute Kalman Gain
  post = prior + K * residual;//compute post
  P = (1 - K) * P;//adjust belief
  Serial.print(rawVoltage);
  voltage = post;
  Serial.print(" ");
  Serial.println(voltage);
  return 328782 * exp(-4.73E-3 * voltage);//apply quadratic regression line
}
bool KalmanFilter::inRange(double r){
  return (abs(rawVoltage - voltage) <= r)?true:false;
}
