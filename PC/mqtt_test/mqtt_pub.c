#include <stdio.h>
#include <unistd.h>
#include <mosquitto.h>

void setup()
{
	
}

int main(int argc, char *argv[]){
	int rc;
	struct mosquitto * mosq;

	mosquitto_lib_init();

	mosq = mosquitto_new("publisher-test", true, NULL);

	rc = mosquitto_connect(mosq, argv[1], 1883, 60);
	if(rc != 0){
		printf("Client could not connect to broker! Error Code: %d\n", rc);
		mosquitto_destroy(mosq);
		return -1;
	}
	printf("We are now connected to the broker!\n");
	

	while(1){
		mosquitto_publish(mosq, NULL, argv[3], 60, argv[2], 0, false);
		sleep(1);
		
	}

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);

	mosquitto_lib_cleanup();
	return 0;
}