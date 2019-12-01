#define DISPLAY_ENABLE

#define LED1_rain 	1 //Pin_num12 (BCM_GPIO 18)
#define LED2_snow 	4 //Pin_num16 (BCM_GPIO 23)
#define LED3_enter 	5 //Pin_num18 (BCM_GPIO 24)

#define Seg1_E 		15  //Pin_num8 (BCM_GPIO 14)
#define Seg2_D 		0  //Pin_num11 (BCM_GPIO 17)
#define Seg3_DP 	2  //Pin_num13 (BCM_GPIO 27)
#define Seg4_C 		3  //Pin_num15 (BCM_GPIO 22)
#define Seg5_G 		21 //Pin_num29 (BCM_GPIO 5)
#define Seg6_D4 	22 //Pin_num31 (BCM_GPIO 6)
#define Seg7_B 		23 //Pin_num33 (BCM_GPIO 13)
#define Seg8_D3 	24 //Pin_num35 (BCM_GPIO 19)
#define Seg9_D2 	25 //Pin_num37 (BCM_GPIO 26)
#define Seg1o_F 	27 //Pin_num36 (BCM_GPIO 16)
#define Seg11_A 	28 //Pin_num38 (BCM_GPIO 20)
#define Seg12_D1 	29 //Pin_num40 (BCM_GPIO 21)

#define MAX_DIGIT_COUNT 4
#define MAX_SEGMENT_PIN 12

enum{
	NONE = 0,
	RAIN,
	BOTH,
	SNOW,
};

extern void* display_func(void* arg);
