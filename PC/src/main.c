#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <mosquitto.h>

char* light1_current_state;
char* light2_current_state;

char* light1_ordered_state;
char* light2_ordered_state;

float* timer;


const char* red = "red";
const char* green="green";
const char* red_to_green="red_to_green";
const char* green_to_red="green_to_red";



/**********DONE****************/

void light_1_red()
{
    wright_mqtt("/traffic_lights_order/light_1","red");
    light1_ordered_state = red
}

void light_1_green()
{
    wright_mqtt("/traffic_lights_order/light_1","red");
    light1_ordered_state = green
}

void light_2_red()
{
    wright_mqtt("/traffic_lights_order/light_2","red");
    light1_ordered_state = red
}

void light_2_green()
{
    wright_mqtt("/traffic_lights_order/light_2","red");
    light1_ordered_state = green
}


void switch_lights()
/*light goes from red to green or the oposit, same for light 2, but other collor*/
{
    if (light1_ordered_state == green)
        light_1_red()
        light_1_green()
    else
        light_2_green()
        light_2_red()
  
}

void all_light_red()
/*all light goes red*/
{
    wright_mqtt("/traffic_lights_order/light_1","red");
    wright_mqtt("/traffic_lights_order/light_2","red");
}

void deal_with_command(short light_id)
{   
    if(light_id == 1)
    {
        if(!strcmp(light1_ordered_state,  green))
        exit(0);
        else
        {
            
        }
    }
    if(light_id == 2)
    {
        if(!strcmp(light2_ordered_state,  green))
        exit(0);
    }
    if
}



/***********TO DO************/

void thread_function()
{
    while (1)
    {
        if(timer > 30.0)
        {
            restart_timer();
            all_light_red();
        }

        // case not correct light
        if(timer > 10.0)
        {
            restart_timer();
            all_light_red();
        }
    }
}
void restart_timer(){}


void setup_mqtt(){}


void wright_mqtt(char* topic, char* info)
{

}

void manager(char* msg)
{
    if

    
}



/***********MQTT***************/

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "/test/#", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
    manager((char *) msg->payload,msg->topic);
	printf("\n\n--%s--\n\n", (char *) msg->payload);
}

int main()
{
    int rc, id=12;

	mosquitto_lib_init();

	struct mosquitto *mosq;

	mosq = mosquitto_new("subscribe-test", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	
	rc = mosquitto_connect(mosq, argv[1], 1883, 10);
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}

	mosquitto_loop_start(mosq);
	printf("Press Enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;

}