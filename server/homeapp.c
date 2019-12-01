#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "homeapp.h"
#include "temperature.h"
#include "server.h"
#include "sonar.h"
HOMEDATA g_data;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void print_homedata(HOMEDATA* data){
	printf("temperature=%d\n", data->temperature);
	printf("humidity=%d\n", data->humidity);
	printf("snow=%d\n", data->snow);
	printf("rain=%d\n", data->rain);
	printf("detection=%d\n", data->detection);
}int get_homedata(HOMEDATA* data){
	if(data == NULL){
		return -1;
	}
	pthread_mutex_lock(&mutex);
	memcpy(data, &g_data, sizeof(HOMEDATA));
	pthread_mutex_unlock(&mutex);
	return 0;
}int set_homedata(HOMEDATA* data){
	if(data == NULL){
		return -1;
	}
	pthread_mutex_lock(&mutex);
	memcpy(&g_data, data, sizeof(HOMEDATA));
	pthread_mutex_unlock(&mutex);
	return 0;
}int main(){
	pthread_t temp_thd;
	pthread_t server_thd;
	pthread_t sonar_thd;
	memset(&g_data, 0, sizeof(HOMEDATA));
	pthread_create(&temp_thd, NULL, &temp_func, NULL);
	pthread_create(&server_thd, NULL, &server_func, NULL);
#ifdef SONAR_ENABLE
	pthread_create(&sonar_thd, NULL, &sonar_func, NULL);
#endif// SONAR_ENABLE
#ifdef SONAR_ENABLE
	pthread_join(sonar_thd, NULL);
#endif// SONAR_ENABLE
	pthread_join(temp_thd, NULL);
	pthread_join(server_thd, NULL);
	pthread_mutex_destroy(&mutex);
	return 0;
}
