/***********************************************************************
   Copyright 2016 Hasan IAA Mohammad
		   hasan@sigmaKW.com
		   www.sigmakw.com


    This file is part of MCP3008LITE.

    MCP3008LITE is free software: you can redistribute it and/or modify
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

static void pabort(const char *s)
{
	perror(s);
	abort();
}

// Device either 0 or 1
int mcp3008LiteOpen(int Device,uint32_t speed, SPI *spi)
{
	int ret = 0;
	int spifID = 0;
	char device[16] = {'\0'};
	uint8_t mode =  SPI_MODE_0;
	uint8_t bits = 8;


	sprintf(device, "%s%i", SPIDEVIE,Device);

	memset(spi,0,sizeof(SPI));

#if _SPIDEBUG_
	printf("opening SPI device = %s\n",device); 
#endif

	spi->delay_usecs = 0;
	spi->speed_hz = speed; 
	spi->bits_per_word = 8;
	spi->len = 3;
	spi->cs_change = 0;
	
	spifID = open(device, O_RDWR);


	if (spifID < 0)
		pabort("can't open device");

#if _SPIDEBUG_
	if (spifID !=0)
	printf("Device opened with spiID = %i\n",spifID);
#endif

	/*
	 * spi mode
	 */
	ret = ioctl(spifID, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(spifID, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(spifID, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(spifID, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(spifID, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(spifID, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");
	else
		printf("SPI_IOC_RD_MAX_SPEED_HZ : %i\n",speed);



	return spifID;
}


int mcp3008LiteClose(int spifID)
{
	int status = close(spifID);
	return status;
}


uint16_t mcp3008LiteRead(int spifID,SPI *spi,const char SigDiff,const char chanel)
{
	static uint16_t value;
	static unsigned char data[3];
	
	value = 0;

	data[0]  = 1;
	data[1]  = (SigDiff<<7) + (chanel<<4); // we configuring the chanel every time we are reading the ADC value.
	data[2] = 0;

	spi->tx_buf = (unsigned long)data;
	spi->rx_buf = (unsigned long)data;

#if _SPIDEBUG_
printf(" ---------------Before Send ---------------\n");
	printspi(spi);
#endif

	ioctl (spifID, SPI_IOC_MESSAGE(1), spi) ;
	
	value =((uint16_t)data[1]&3) <<8 | data[2];

#if _SPIDEBUG_	
printf(" ---------------After Send ---------------\n");
	printspi(spi);
	for (int i =0;i<3;i++)
	printf("Output%i: %i [%s]\n",i,data[i], byte_to_binary(data[i]));

	printf("10bit Output : %i\n",value);
#endif

	

	return value;
}


#if _SPIDEBUG_

void printspi(SPI *spi)
{
	printf("SPI data: \n");
	printf("  delay_usec = %i\n",spi->delay_usecs);
	printf("  speed_hz = %i \n",spi->speed_hz);
	printf("  bits_per_word = %i \n",spi->bits_per_word);
	printf("  len = %i \n",spi->len);
	printf("  CS_Change = %i \n",spi->cs_change);
	printf("  data tx_buf: \n");
	for(int i = 0;i<spi->len;i++)
		printf("    b%i = %d [%s]\n",i,((char*)spi->tx_buf)[i], byte_to_binary(((char*)spi->tx_buf)[i])); 
		//warning: cast to pointer from integer of different size [-Wint-to-pointer-cast], but will work
	printf("  data rx_buf: \n");
	for(int i = 0;i<spi->len;i++)
		printf("    b%i = %d [%s]\n",i,((char*)spi->rx_buf)[i], byte_to_binary(((char*)spi->rx_buf)[i]));
	

}

const char *byte_to_binary(int x)
{
     static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}


#endif



