# multiuser_energymeter

This is a firmware for a multi user energy meter. The firmware was developed using arduino IDE. Three independent users can use a single meter and be charged independently.

Atmega 328 micrcontroller was used. It was interfaced with a GSM module using UART. The UART pins were connected to PD 2 and PD 3 of the atmega 328 microcontroller.

Interrupt used ensured that the message received by the GSM was quickly acknowleged and deparsed by the microcontoller.
Other components interfaced were relays, ACS 712 current sensor, LCD screen, etc.

Serial communication protocols like I2C, UART were used. 
