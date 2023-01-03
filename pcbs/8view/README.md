## 8view

8bit bus bin/hex viewer

![8view](../doc/th-8view-1.0.jpg)

### Features
* 8 LEDs viewing bits
* 2 7-segment displays for hex driven by attiny828 used mostly as LUT

### Flashing

```
$ avr-gcc -mmcu=attiny828 8view.c -Os -o 8view.o && avr-objcopy -j .text -j .data -O ihex 8view.o 8view.hex

# Works with arduino isp
$ avrdude -p t828 -P /dev/ttyXX -c avrisp -b 19200 -U flash:w:8view.hex -F
```

### TODO
* Make high-impedance
* Implement snapshot / record mode