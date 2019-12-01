#define SERVER_PORT 9734
#define SERVER_IP "192.168.1.9"
typedef struct tHOMEDATA{
	int temperature;
	int humidity;
	int rain;
	int snow;
	int detection;
} HOMEDATA;
void print_homedata(HOMEDATA* data);
int get_homedata(HOMEDATA* data);
