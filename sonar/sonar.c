#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define SONAR1_DEVICE "/dev/sensor"
#define SONAR2_DEVICE "/dev/sensor2"
#define MAX_DISTANCE 300
#define DETECT_LENGTH 100
#define LOOP_TIME 50*1000 //usec
#define MAX_NAME_LENGTH 16
#define RESET_COUNT 100 //RESET_COUNT*LOOP_TIME sec
enum {
	SONAR_ID0 = 0,
	SONAR_ID1,
};
typedef struct tSONAR{
	int id;
	char name[MAX_NAME_LENGTH];
} SONAR;
int flag[2] = {0, };
int g_count = 0;
void* sonar_thread(void* arg){
	SONAR* sonar = (SONAR*)arg;
	printf("[%s(%d)] id=%d, name=%s\n", __FUNCTION__, __LINE__, sonar->id, sonar->name);
	int fd = -1;
	int prev_data = 0;
	int data = 0;
	int diff = 0;
	int count = 0;
	int is_first = 1;
	int loop_count = 0;
	fd = open(sonar->name, O_RDWR);
	if(0 >= fd)
	{
		printf("Open fail!\n");
		exit(0);
	}
	while(1){
		ioctl(fd, NULL, &data);
		if(0 >= data || MAX_DISTANCE <= data){
			continue;
		}
		
		if(is_first){
			prev_data = data;
			is_first = 0;
		}
		diff = prev_data - data;
		if(diff > DETECT_LENGTH)
		{
			flag[sonar->id] = 1;
			printf("[%s%d(%d)] Movement detected!!! data=%d, prev=%d\n", 
				__FUNCTION__, sonar->id, __LINE__, data, prev_data);
			loop_count = 0;
		}
		prev_data = data;
		if(RESET_COUNT < loop_count && flag[sonar->id]){
			loop_count = 0;
			flag[sonar->id] = 0;
			printf("[%s%d(%d)] Reset Flags.... data=%d, prev=%d\n", 
				__FUNCTION__, sonar->id, __LINE__, data, prev_data);
		}
		loop_count++;
		//printf("[%s%d(%d)] data=%d, prev=%d\n", __FUNCTION__, sonar->id, __LINE__, data, prev_data);
		usleep(LOOP_TIME);
	}
	return (void*)NULL;
}void* detect_thread(void* arg){
	int bflag = 0;
	int sign = 0;
	while(1){
		bflag = (flag[SONAR_ID1]<<1) | (flag[SONAR_ID0]);
		switch(bflag){
			case 1:
				sign = 1;
			break;
			case 2:
				sign = -1;
			break;
			case 3:
				if(sign < 0 && g_count == 0){
					sign = 0;
				}
				g_count += sign;
				printf("[%s(%d)] g_count=%d, sign=%d\n", __FUNCTION__, __LINE__, g_count, sign);
				sign = 0;
				flag[SONAR_ID0] = 0;
				flag[SONAR_ID1] = 0;
			break;
			default:
			break;
		}
		usleep(LOOP_TIME);
	}
}void set_sonar_arg(SONAR* sonar, int id, char* name){
	sonar->id = id;
	strcpy(sonar->name, name);
}int main() 
{	pthread_t pthd_sonar1;
	pthread_t pthd_sonar2;
	pthread_t pthd_detect;
	SONAR sonar1;
	SONAR sonar2;
	set_sonar_arg(&sonar1, SONAR_ID0, SONAR1_DEVICE);
	set_sonar_arg(&sonar2, SONAR_ID1, SONAR2_DEVICE);
	pthread_create(&pthd_sonar1, NULL, sonar_thread, &sonar1);
	pthread_create(&pthd_sonar2, NULL, sonar_thread, &sonar2);
	pthread_create(&pthd_detect, NULL, detect_thread, NULL);
	pthread_join(pthd_detect, NULL);
	pthread_join(pthd_sonar2, NULL);
	pthread_join(pthd_sonar1, NULL);
	return 0;
}
