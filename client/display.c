#include <termio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wiringPi.h>
#include <pthread.h>
#include "client.h"
#include "display.h"

char seg[MAX_SEGMENT_PIN] = {Seg12_D1, Seg9_D2, Seg8_D3, Seg6_D4, Seg11_A, Seg7_B, Seg4_C, Seg2_D, Seg1_E, Seg1o_F, Seg5_G, Seg3_DP};
char seg_digit[MAX_DIGIT_COUNT][MAX_DIGIT_COUNT] = {	{0, 1, 1, 1},
							{1, 0, 1, 1},
							{1, 1, 0, 1},
							{1, 1, 1, 0} };
char seg_value[10][MAX_SEGMENT_PIN] = { {1,1,1,1,1,1,0,0},
					{0,1,1,0,0,0,0,0},
					{1,1,0,1,1,0,1,0},
					{1,1,1,1,0,0,1,0},
					{0,1,1,0,0,1,1,0},
					{1,0,1,1,0,1,1,0},
					{1,0,1,1,1,1,1,0},
					{1,1,1,0,0,0,0,0},
					{1,1,1,1,1,1,1,0},
					{1,1,1,1,0,1,1,0} };

//int main (void){
void* display_func(void* arg){
	HOMEDATA data;
	int digit[MAX_DIGIT_COUNT];
	int enter;
	int i = 0;
	int j = 0;

	if (wiringPiSetup () == -1)
		return (void*)1;

	pinMode (LED1_rain, OUTPUT);
	pinMode (LED2_snow, OUTPUT);
	pinMode (LED3_enter, OUTPUT);

	for(i = 0; i < MAX_SEGMENT_PIN; i++){
		pinMode (seg[i], OUTPUT);
	}

	while(1){
		get_homedata(&data);

		digitalWrite (LED1_rain, data.rain); // If 'rain' is 0 -> off, 1 -> on
		delay (3);

		digitalWrite (LED2_snow, data.snow); // If 'snow' is 0 -> off, 1 -> on
		delay (3);

		if(data.detection == 0)
			enter = 0;
		else
			enter = 1;
		digitalWrite (LED3_enter, enter); // If 'enter' is 0 -> off, 1 -> on
		delay (3);

		digit[0] = data.temperature/10;
		digit[1] = data.temperature%10;
		digit[2] = data.humidity/10;
		digit[3] = data.humidity%10;

		for(i = 0; i < MAX_DIGIT_COUNT; i++){
			for(j = 0; j < MAX_SEGMENT_PIN; j++){
				if(j < MAX_DIGIT_COUNT){
					digitalWrite (seg[j], seg_digit[i][j]);
				}
				else{
					digitalWrite (seg[j], seg_value[digit[i]][j-4]);
				}
			}
			delay (3);
		}
	}

	return (void*)NULL;
}
