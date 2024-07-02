# ivl275_clock

![alt tag](https://raw.githubusercontent.com/so1der/ivl275_clock/main/pictures/main.jpg "Main photo")

So this is a simple clock I made a while ago. It combines some bad engineering decisions, but still it works. Main IC's are:

- ATmega8A - MCU to drive all together
- DS1307 - RTC to count passed time
- MC34063A - DC-DC to power VFD anodes/grids
- ULN2803A - Darlington transistor array to drive VFD segments

The modularity of the circuit allows to change some blocks - You can change MCU, change RTC, or DC-DC converter - The idea stays the same. Heck, You can even change VFD Display! But in this case You need to change filament voltage and etc. 

It would be better to use specialized VFD drives IC's, and AC voltage on filament, but because of the complexity of such decisions (and difficulty in acquiring such IC's), I ended up with following design:

![alt tag](https://raw.githubusercontent.com/so1der/ivl275_clock/main/pictures/schematic.png "Schematic")

## DC-DC Step Up 5V to 24V

It's just a simple step up converter on the MC34063A IC. You can use [THIS](http://www.nomad.ee/micros/mc34063a/) online calculator to recalculate schematic. This voltage affect brightness of the VFD tube, and You can change brightness by changing voltage - for example put there a variable resistor, or even photoresistor! But make sure that by doing so, You won't exceed maximum voltage of the capacitor, VFD, IC, and etc.

## Real Time Clock Module

In my case, I assembled this module myself, but no one stops You from using completed modules such as **HW-111** for example. You can even use different IC (e.g. DS3231), but in this case You need to rewrite firmware's code. In this project I didn't use RTC libraries (such as **RTCLib**) - I wanted to drive RTC directly, so I put all necessary code into main cpp file.

DS1307 also generates 1 Hz squared waveform that drives blinking of the DP dots.

## Filament voltage

As I said before - it would be better to use AC for the filament. But in my case, I simply used voltage regulator to drop 5V to approximately 1.8V. Cons of this decision - uneven brightness of the display, and maybe shorter period of display life.

## ULN2803A

This IC allows us to pull VFD segments to the ground, so we can "shut" them. So yeah, by default all segments are emitting light, and by "turning off" various segments, we can display certain numbers. Kind of a "reversed" control. R2-R8 are used to prevent short circuit when we are pulling segments to the ground.
