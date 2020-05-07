#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ToggleButton.cpp"
#include <QC3Control.h>
#include <Fonts/FreeMono9pt7b.h>

Adafruit_SSD1306 oledik(128, 32, &Wire, 4);

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int voltageSensor1 = 0;  // variable to store the value coming from the sensor
int currentSensor1 = 0;

QC3Control quickCharge(4, 5);


enum Mode {QC2 = 0, QC3 = 1};
Mode mode = Mode::QC3;
void toggle_mode (ToggleButton *btn) {
  if (mode == Mode::QC2) {
    mode = Mode::QC3;
  } else if (mode == Mode::QC3) {
    mode = Mode::QC2;
  }
  Serial.println("in toggle_mode, Mode: " + String(mode));
}

int index = 0;
int currentSetVoltage;
void up (ToggleButton *btn) {
  currentSetVoltage = quickCharge.getMilliVoltage();
  if (mode == Mode::QC2) {
    if        (currentSetVoltage < 5000) {
       quickCharge.set5V();
    } else if (currentSetVoltage < 9000) {
       quickCharge.set9V();
    } else if (currentSetVoltage < 12000) {
       quickCharge.set12V();
    } else if (currentSetVoltage < 20000) {
       quickCharge.set20V();
    } else {
       quickCharge.set5V();
    }
  } else if (mode == Mode::QC3) {
    if (currentSetVoltage < 22000) {
      quickCharge.incrementVoltage();
    }
  }
  currentSetVoltage = quickCharge.getMilliVoltage();
  Serial.println("in up, currentSetVoltage: " + String(currentSetVoltage));
}

void down (ToggleButton *btn) {
  currentSetVoltage = quickCharge.getMilliVoltage();
  if (mode == Mode::QC2) {
    if        (currentSetVoltage > 20000) {
       quickCharge.set20V();
    } else if (currentSetVoltage > 12000) {
       quickCharge.set12V();
    } else if (currentSetVoltage > 9000) {
       quickCharge.set9V();
    } else if (currentSetVoltage > 5000) {
       quickCharge.set5V();
    } else {
       quickCharge.set5V();
    }
  } else if (mode == Mode::QC3) {
    if (currentSetVoltage > 3600) {
      quickCharge.decrementVoltage();
    }
  }
  currentSetVoltage = quickCharge.getMilliVoltage();
  Serial.println("in down, currentSetVoltage: " + String(currentSetVoltage));
}


ToggleButton button1(7,toggle_mode);
ToggleButton button2(8,down);
ToggleButton button3(9,up);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting usb-tester:");
  
  analogReference(INTERNAL);
  
  quickCharge.begin(true);
  mode = Mode::QC2;
  quickCharge.set5V();
  delay(1000);
  
  if (!oledik.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }
  oledik.display();
  oledik.clearDisplay();
  

//  quickCharge.setMilliVoltage(5200);
  Serial.println("Starting QC: " + String(quickCharge.getMilliVoltage()));
}

double convert2voltage(int sensor, float from0, float from1, float to0, float to1) {
  return to0 + (to1 - to0) / (from1 - from0) * sensor;
}


void loop() {
  String line1 = "" + String(mode?"QC3":"QC2") + "  " + String(quickCharge.getMilliVoltage()/1000.0) + "V";
  
  voltageSensor1 = analogRead(A1);
  float voltage1 = convert2voltage(voltageSensor1, 0, 1023, 0, 1.1 * 20.3);
  String line2 = String(voltage1) + "V ";
  
  currentSensor1 = analogRead(A0);
  double current1 = convert2voltage(currentSensor1, 0, 1023, 0, 1.1 * 3.386);
  line2 += String(current1) + "A";
  
  Serial.print(line1 + " ");
  Serial.println(line2);

  oledik.setFont(&FreeMono9pt7b`);
  oledik.clearDisplay();
  oledik.setTextSize(1);      // Normal 1:1 pixel scale
  oledik.setTextColor(SSD1306_WHITE); // Draw white text
  oledik.setCursor(0, 15);     // Start at top-left corner
//  oledik.cp437(true);         // Use full 256 char 'Code Page 437' font
  oledik.println(line1);
  oledik.setCursor(0, 31);     // Start at top-left corner
  oledik.println(line2);
  oledik.display();

  button1.read();
  button2.read();
  button3.read();
  delay(20);

}



/*
   voltageSensor1
  20k 1k  21/1 = 21
  0->21 should be mapped to 0->1.1   19.09 ratio for voltage
  0->1.1 maps to 0->1023

  currentSensor1
  2.8k 1k 3.8/1 = 3.8 is opamp gain

  1   -> 1.1 V
  3.8 -> 4.18 V is max
  value/3.8/0.06 = 4.386
  0->3.8  should be mapped to 0->1.1
*/
