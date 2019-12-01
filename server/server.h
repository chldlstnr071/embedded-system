typedef struct tHOMEDATA{
	int temperature;
	int humidity;
	int rain;
	int snow;
	int detection;
} HOMEDATA;
void print_homedata(HOMEDATA* data);
int set_homedata(HOMEDATA*);
int get_homedata(HOMEDATA*);
