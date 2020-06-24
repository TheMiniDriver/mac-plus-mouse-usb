# mac-plus-mouse-usb

Apple Macintosh Plus M0100 Mouse USB converter using Arduino UNO
by Bradley Van Aardt (https://github.com/TheMiniDriver/mac-plus-mouse-usb)
23 June 2020
  
Adapted from https://geekhack.org/index.php?topic=74340.0  by GuilleAcoustic and Johan Berglund. 

Using code from https://anhnguyen.me/2014/01/turn-arduino-uno-r3-into-a-mouse/ by Darran Hunt, Anh Nguyen

I adapted the GuilleAcoustic and Johan Berglund code to run on an Arduino UNO, instead of the Pro Micro. This was purely because I only had an UNO. 
This meant that another method of USB communication needed to be found, as the UNO does not support the libary calls used in the orignal code. 
I found a solution by Darran Hunt in a good tutorial by Anh Nguyen, and updated the code with that. 
Note that it requires the Uno firmware to be changed while running the application. 

The other significant change was to move to a polling method for the rotary encoders from the mouse, as the UNO only has 2 interrupt pins, where 4 are required for both axis.
I thought this would severely affect the behaviour of the mouse, but it works quite well.   


Connection to M0100 DB9: 

DB9     Arduino UNO
1       GND
2       VCC
3       GND
4       PIN4     
5       PIN5      
6       -       (not connected)
7       PIN7     
8       PIN8      
9       PIN9  



