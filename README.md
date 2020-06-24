

# Apple Macintosh Plus M0100 Mouse USB converter using Arduino UNO R3

by Bradley Van Aardt (https://github.com/TheMiniDriver/mac-plus-mouse-usb)

23 June 2020

Adapted from https://geekhack.org/index.php?topic=74340.0  by GuilleAcoustic and Johan Berglund. Using USB code from https://anhnguyen.me/2014/01/turn-arduino-uno-r3-into-a-mouse/ by Darran Hunt, Anh Nguyen

I adapted the GuilleAcoustic and Johan Berglund code to run on an Arduino UNO, instead of the Sparkfun Pro Micro. This was purely because I only had an UNO. This meant that another method of USB communication needed to be found, as the UNO does not support the library calls used in the orignal code. I found a solution by Darran Hunt in a good tutorial by Anh Nguyen, and updated the code with that. 

Note that it requires the UNO firmware to be changed while running the application. 

The other significant change was to move to a polling method for the rotary encoders from the mouse, as the UNO only has 2 interrupt pins, where 4 are required for both axes. I thought this would severely affect the usability of the mouse, but it works quite well.   

## Wiring

Connection to M0100 DB9: (M0100 info from https://support.apple.com/kb/TA29935)

| M0100 DB9                     | Arduino UNO |
| ----------------------------- | ----------- |
| 1 GND                         | GND         |
| 2 VCC                         | VCC         |
| 3 GND                         | GND         |
| 4 X2 Horizontal movement line | PIN4        |
| 5 X1 Horizontal movement line | PIN5        |
| 6 Not used                    | -           |
| 7 SW- Mouse button line       | PIN7        |
| 8 Y2 Vertical movement line   | PIN8        |
| 9 Y1 Vertical movement line   | PIN9        |



## Using the software

The software requires changing the Arduino firmware, using a DFU (Device Firmware Update) programmer. Follow the instructions here (https://www.arduino.cc/en/Hacking/DFUProgramming8U2) to get the right DFU programmer for your system

The procedure to use the mouse driver code is then as follows:

1. Upload the sketch in the `src` folder in the repo to the Arduino UNO using the normal IDE
2. Power cycle the Arduino UNO
3. Reset the 16u2 chip by shorting the reset pins 
4. Upload the [Arduino-mouse-0.1.hex](https://github.com/TheMiniDriver/mac-plus-mouse-usb/blob/master/arduino-firmware/Arduino-mouse-0.1.hex) file using your DFU programmer. If you are using macOS, you can use the shell script [hid-mode.sh](https://github.com/TheMiniDriver/mac-plus-mouse-usb/blob/master/arduino-firmware/hid-mode.sh) to upload the new firmware.
5. Power cycle the Arduino UNO again. The sketch should be running, and the Arduino will appear as a mouse to your system, and translate the movement from the M0100 mouse. 

The procedure to revert to standard Arduino firmware (so that you can re-program etc) is as follows:

1. Reset the 16u2 chip by shorting the reset pins 
2. Upload the file [Arduino-usbserial-uno.hex](https://github.com/TheMiniDriver/mac-plus-mouse-usb/blob/master/arduino-firmware/Arduino-usbserial-uno.hex)  using your DFU programmer. If you are using macOS, you can use the shell script [prog-mode.sh](https://github.com/TheMiniDriver/mac-plus-mouse-usb/blob/master/arduino-firmware/prog-mode.sh) to upload the firmware. 
3. Power cycle the Arduino UNO. You will now be able to access it as usual. 

