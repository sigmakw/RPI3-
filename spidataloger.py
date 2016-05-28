import spidev
import time


myfile = open('data.txt','w')

spi = spidev.SpiDev()

spi.open(0,1) # CS 1


# read SPI data from MCP3008 chip, 8 possible adc's (0 thru 7)
def readadc(adcnum):
        if ((adcnum > 7) or (adcnum < 0)):
                return -1
        r = spi.xfer2([1,(8+adcnum)<<4,0])
        adcout = ((r[1]&3) << 8) + r[2]
        return adcout



spi.max_speed_hz =1500000 # SPS = 19 065
#spi.max_speed_hz =3500000  # SPS  = 29 388
#spi.max_speed_hz =3000000  # SPS  = 29 388
 


datasz = 1000
data0 = [None]*datasz
data1 = [None]*datasz
datatime = [None]*datasz

i = 0

startT  = time.time()
while(i<datasz):
	data0[i] = readadc(0)
	data1[i] = readadc(1)
	datatime[i]=time.time() -startT
	i +=1 

spi.close()
i = 0
while (i<datasz):
	myfile.write( str(i) +',' +str(datatime[i])+','+ str(data0[i]) + ',' + str(data1[i]) +'\n')
	i +=1


myfile.close()





