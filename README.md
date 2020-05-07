# QC3-trigger-usb-tester

  This is a simple project for a simple device which combines QC3 trigger and USB tester functionality in one unit.
It can be used as a variable voltage power supply by plugging to a power bank or QC3 charger with ability to measure actual current and voltage.
I wanted to have a portable power supply for my TS100 solder so I can power it with my QC3 power bank.

All the USB pins (except ground, which connected via current shunt for an ampermeter) pass throug the device.
DP and DM pins are also connected to arduino pins 4 and 5 to be able to set Vbas voltage according to  [Quick Charge 3.0](https://www.qualcomm.com/products/features/quick-charge) technology form [Qualcomm](https://www.qualcomm.com/). see details in  [vdeconinck/QC3Control](https://github.com/vdeconinck/QC3Control). And As it mentioned in the [QC3Control](https://github.com/vdeconinck/QC3Control) project it is also QC2.0 compatible.

The Arduino code implementation is mainly based on [QC3Control](https://github.com/vdeconinck/QC3Control) and Adafruit_SSD1306 OLED display Arduino libraries.

## Prototype photo #1
![Prototype photo #1:](https://github.com/ankapPost/QC3-trigger-usb-tester/blob/master/pics/20200508_003848.jpg)

## Prototype photo #2
![Prototype photo #2:](https://github.com/ankapPost/QC3-trigger-usb-tester/blob/master/pics/20200508_003818.jpg)

