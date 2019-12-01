#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "homeapp.h"
#include "temperature.h"
int dht11_dat[5] = { 0, 0, 0, 0, 0 };
void read_dht11_dat()
{	HOMEDATA data;
	unsigned char laststate	= HIGH;
	unsigned char counter		= 0;
	unsigned char j		= 0, i;
	float	f; /* fahrenheit */
	memset(dht11_dat, 0, sizeof(dht11_dat));
	//dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
	/* pull pin down for 18 milliseconds */
	pinMode( DHTPIN, OUTPUT );
	digitalWrite( DHTPIN, LOW );
	delay( PIN_DOWN_DELAY );
	/* then pull it up for 40 microseconds */
	digitalWrite( DHTPIN, HIGH );
	delayMicroseconds( PIN_UP_DELAY );
	/* prepare to read the pin */
	pinMode( DHTPIN, INPUT );
	/* detect change and read data */
	for ( i = 0; i < MAXTIMINGS; i++ )//MAXTIMINGS=85
	{
		counter = 0;
		while ( digitalRead( DHTPIN ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( DHTPIN );
		if ( counter == 255 ){
			printf("[%s(%d)] Time Out!!!!!! i=%d, j=%d\n", __FUNCTION__, __LINE__, i, j);
			break;
		}
		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			dht11_dat[j / 8] <<= 1;
			if ( counter > 50 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) && 
		(j >= 40) && (dht11_dat[4] != 0) )
	{
		f = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
		get_homedata(&data);
		data.temperature = dht11_dat[2];
		data.humidity = dht11_dat[0];
		set_homedata(&data);
	}else  {
		printf( "Data not good, skip\n" );
	}
}//int main( void )
void* temp_func(void* arg)
{	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
	if ( wiringPiSetup() == -1 ){
		printf("[%s(%d)] Failed to setup wiringPi!\n", __FUNCTION__, __LINE__);
		return (void*)-1;
	}
	while ( 1 )
	{
		read_dht11_dat();
		sleep( READ_DELAY );
	}
	return (void*)NULL;
//	return 0;
}
