#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "client.h"
#include "display.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

HOMEDATA g_data;

void print_homedata(HOMEDATA* data){
	printf("temperature=%d\n", data->temperature);
	printf("humidity=%d\n", data->humidity);
	printf("snow=%d\n", data->snow);
	printf("rain=%d\n", data->rain);
	printf("detection=%d\n", data->detection);
}

int get_homedata(HOMEDATA* data){
	if(data == NULL){
		return -1;
	}
	pthread_mutex_lock(&mutex);
	memcpy(data, &g_data, sizeof(HOMEDATA));
	pthread_mutex_unlock(&mutex);
	return 0;
}

int weather_value(int weather)
{
    FILE *fp = fopen("weather.txt", "r");

    if (fp == NULL) {
        printf("Failed open file");
        weather = 0;
    }
    else {
		fscanf(fp, "%d", &weather);
    }
    
    fclose(fp);

    return weather;
}

int main(){
	pthread_t display_thd;
	int sockfd = 0;
	int len = 0;
	struct sockaddr_in address;
	int result = 0;
	int weather = 0;
	memset(&g_data, 0, sizeof(HOMEDATA));
#ifdef DISPLAY_ENABLE
	pthread_create(&display_thd, NULL, &display_func, NULL);
#endif// DISPLAY_ENABLE

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(SERVER_IP);
	address.sin_port = htons(SERVER_PORT);
	len = sizeof(address);
	connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1){
		perror("opps: client3\n");
		exit(1);
	}
	while(1){
		write(sockfd, &g_data, sizeof(HOMEDATA));
		read(sockfd, &g_data, sizeof(HOMEDATA));
        weather = weather_value(weather);
		switch(weather){
			case RAIN:
				g_data.rain = 1;
				g_data.snow = 0;
			break;
			case SNOW:
				g_data.rain = 0;
				g_data.snow = 1;
			break;
			case BOTH:
				g_data.rain = 1;
				g_data.snow = 1;
			break;
			default:
				g_data.rain = 0;
				g_data.snow = 0;
			break;
		}
		print_homedata(&g_data);
		sleep(1);
	}
	close(sockfd);
#ifdef DISPLAY_ENABLE
	pthread_join(display_thd, NULL);
#endif// DISPLAY_ENABLE
	exit(0);
}
