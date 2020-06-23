sudo dfu-programmer atmega16u2 erase
sudo dfu-programmer atmega16u2 flash --debug 1 Arduino-mouse-0.1.hex
sudo dfu-programmer atmega16u2 reset