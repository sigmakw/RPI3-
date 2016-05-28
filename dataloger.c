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




#include "mcp3008Lite.h"
#include <time.h>
#include <stdio.h>

#define dataSZ 1000     // reading sample size

int main()
{
	clock_t start_t;
	FILE *fp;
	SPI spi; // need it for sending configurrations to spidev
	int spiID;

	uint16_t data0[dataSZ] = {0};
	uint16_t data1[dataSZ] = {0};
	clock_t dataTime[dataSZ] = {0};



	spiID =  mcp3008LiteOpen(1, 3200000, &spi);
	// Note: 3.2MHz is the fastes I was able to reach without getting bad signal.
        // at 3.2Mhz, was getting around 140 KSPS, for 1 channel. 	

	fp=fopen("cData.txt", "w");


	start_t = clock();
	for(int i =0;i<dataSZ;i++)
	{
		data0[i] = mcp3008LiteRead(spiID,&spi,1,0);// single-ended, chanel 0
		data1[i] = mcp3008LiteRead(spiID,&spi,1,1);// single-ended, chanel 1
		dataTime[i] = clock() - start_t;


	}



	for (int i = 0;i<dataSZ;i++)
	fprintf(fp, "%i,%lf,%i,%i\n",i,(double) dataTime[i]/ CLOCKS_PER_SEC,data0[i],data1[i]);
	


	mcp3008LiteClose(spiID);
	fclose(fp);



	return 0;



}

