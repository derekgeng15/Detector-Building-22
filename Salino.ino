#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RGB_LED.h"
#include "KalmanFilter.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define rPin 8
#define gPin 9
#define bPin 10
#define ppmPin A0
#define VREF 3.3

#define blueThreshStart 3500
#define blueThreshEnd 5000
#define greenThreshStart 1500
#define greenThreshEnd 3500
#define redThreshStart 0
#define redThreshEnd 1500
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

RGB_LED rgbLED(rPin, gPin, bPin);
KalmanFilter ppmSensor(ppmPin, VREF);
void setup() {
  // put your setup code here, to run once:
//  analogReference(AR_INTERNAL);
  analogReadResolution(10);
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display Failed!");
    while (1);
  }
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  ppmSensor.burn();
}
void oledReset() {
  oled.clearDisplay();
  oled.setCursor(0, 0);
}
void oledPrint(double ppm, double voltage, bool inRange) {
  //celcius
  oled.setTextSize(2);
  oled.print(ppm);
//  oled.setTextSize(1);
////  oled.print(" o");
//  oled.setTextSize(2);
  oled.println(" PPM");
  oled.print(voltage);
  oled.setTextSize(2);
  oled.println(" mV");
  oled.println();
  oled.display();
}

double ppm;
double roundD(double n, int p) {
  return (double)(round(n * p * 10) / ((float)p * 10.0));
}
void loop() {
  // put your main code here, to run repeatedly:
  oledReset();
  ppm = (int)ppmSensor.getPPM();
  //LED CODE
  if (ppm > blueThreshStart && ppm <= blueThreshEnd)
    rgbLED.setColor(RGB_LED::COLOR::BLUE);
  if (ppm > greenThreshStart && ppm <= greenThreshEnd)
    rgbLED.setColor(RGB_LED::COLOR::GREEN);
  if (ppm >= redThreshStart && ppm <= redThreshEnd)
    rgbLED.setColor(RGB_LED::COLOR::RED);

  oledPrint(ppm, round(ppmSensor.voltage), ppmSensor.inRange(1.5));
}
