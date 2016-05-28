/***********************************************************************
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

  to compile:
    gcc-4.9 -Wall mcp3008Lite.c dataloger.c -std=gnu11 -o dataLoger

    special thanks to :

          Anton Vorontsov, from  MontaVista Software, Inc.
          http://armbedded.eu/node/318
	 
	  And
	
	  Hussam Al-Hertani
          http://hertaville.com/2013/07/24/interfacing-an-spi-adc-mcp3008-chip-to-the-raspberry-pi-using-c/
	  
	
	
****************************************************************************/

#ifndef MCP3008LITE_H
#define MCP3008LITE_H
     
#include <stdint.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/ioctl.h>
#include <linux/spi/spidev.h> // for spi_ioc_transfer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPIDEVIE "/dev/spidev0." 

#define _SPIDEBUG_ 0
// if DEBUG is 1, you will get warning: cast to pointer from integer of different size [-Wint-to-pointer-cast], but will work
 
typedef struct spi_ioc_transfer SPI;

// Device either 0 or 1
int mcp3008LiteOpen(int Device,uint32_t speed,SPI *spi);

uint16_t mcp3008LiteRead(int spifID,SPI* spi,const char SigDiff,const char chanel);

int mcp3008LiteClose(int spifID);


// for debuging
#if _SPIDEBUG_
void printspi(SPI *spi);
const char *byte_to_binary(int x); 
#endif


#endif
