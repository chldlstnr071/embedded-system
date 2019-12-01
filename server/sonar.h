#define SONAR_ENABLE
#define SONAR1_DEVICE "/dev/sensor"
#define SONAR2_DEVICE "/dev/sensor2"
#define MAX_DISTANCE 300
#define DETECT_LENGTH 100
#define LOOP_TIME 50*1000 //usec
#define DETECTION_INTERVAL 450*1000 //usec
#define MAX_NAME_LENGTH 16
#define MAX_DETECTION_COUNT 10
#define RESET_COUNT 20 //RESET_COUNT*LOOP_TIME sec
enum {
	SONAR_ID0 = 0,
	SONAR_ID1,
};
typedef struct tSONAR{
	int id;
	char name[MAX_NAME_LENGTH];
} SONAR;
extern void* sonar_func(void* arg);
