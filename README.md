What is it?
  -----------


	This is fast implementation for SPI to mcp3008 Analog to Digital Convertor on Raspberry Pi 3.
	Was able to reach 7 micro seconds per channel ( 7e-6 s)

	you simply need 3 functions:

	1- to initiate :

		int mcp3008LiteOpen(int Device,
					uint32_t speed, 
		                        SPI *spi)
	
		Device:
			only called once, Device is for Chip Select CS on RPi3,
			for CS0 → 0
			for CS1 → 1
	
		speed:
			was able to get 3.2 MHz working with out reading bad values form a 50 Hz function generator and comparing it to Oscilloscope.

		spi:
			just need to declare it and pass it’s address around.

		Returns:
			if successful with return the SPI file id that will be used later.


	2- Read the ADC 10 bit value as fast as possible:
		uint16_t mcp3008LiteRead(int spifID,
						SPI *spi,
		                                const char SigDiff,
		                                const char channel)

		spifID:
			passed form return of  mcp3008LiteOpen.
	
		spi:
			was initiated in  mcp3008LiteOpen, and just pass the address of if.
	
		SigDiff:
			to select the channel 
			single-ended → 1
			differential → 0
			In case of differential:
			channel setting will read as following:
			channel → 0  →ADCvalue =  (CH0-CH1)
			channel → 1  →ADCvalue =  (CH1-CH0)
			channel → 2  →ADCvalue =  (CH2-CH3)
			channel → 3  →ADCvalue =  (CH3-CH2)
			channel → 4  →ADCvalue =  (CH4-CH5)
			channel → 5  →ADCvalue =  (CH5-CH4)
			channel → 6  →ADCvalue =  (CH6-CH7)
			channel → 7  →ADCvalue =  (CH7-CH6)
	
			where CH0 to CH7 are pins 1 to 8 respectively on MCP3008.

		 
		channel:
			channels to be read form MCP3008, can be from 0 to 7.

		Returns:
			10 bit value of the ADC conversion.




	3- Closing the SPI:
		 
		int mcp3008LiteClose(int spifID);

		spiID:
			file that has opened with.
	
		Returns:
			Upon successful completion, 0 shall be returned; otherwise, -1 shall be returned and errno set to indicate the error.



  Documentation
  -------------

	schematic is available in the git folder, but make sure 
	you connected the following for High frequencey operations:

	MCP3008 VDD to Raspberry Pi 5.0V [[[NOT 3.3V]]]
	MCP3008 VREF to Raspberry Pi 5.0V [[[NOT 3.3V]]]
	MCP3008 AGND to Raspberry Pi GND
	MCP3008 DGND to Raspberry Pi GND
	MCP3008 CLK to Raspberry Pi SCLK
	[[[[[[          CAREFULL NEXT STEP   ]]]]]]]
	MCP3008 DOUT to Raspberry [[[VOLTAGE DIVIDER]]] then to Pi MISO
	MCP3008 DIN to Raspberry Pi MOSI
	MCP3008 CS/SHDN to Raspberry Pi CE0

  Installation
  ------------
	To compile on Raspberry Pi 3: 
	as destination folder:
	      gcc-4.9 -Wall mcp3008Lite.c dataloger.c -std=gnu11 -o dataLoger

	make sure the following are enabled on RPi3 to use the SPI
	(Serial Peripheral Interface)

	1-
		sudo raspi-config
 
		advanced Options → AS SPI → YES → OK → REBOOT

	 2-
		sudo nano /boot/config.txt

		dtparam=spi=on

		reboot

	 3- 
		to check SPI available: 
		lsmod | grep spi_



  Contacts
  --------


     o If you want support for running please contact me at 
	<hasan@sigmakw.com>,	
	<http://www.sigmakw.com>



  Cryptographic Software Notice
  -----------------------------
 Copyright 2016 Hasan IAA Mohammad
		   hasan@sigmaKW.com
		   www.sigmakw.com


    This file is part of MCP3008LITE.

    mcp3008Lite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    mcp3008Lite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with mcp3008Lite.  If not, see <http://www.gnu.org/licenses/>.
