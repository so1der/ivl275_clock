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
