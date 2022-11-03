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

int light_set=1;
int changeable = true;
int changeable_priority = 0;

int current_green_light = 0;
int next_green_light = 0;


/**********DONE****************/

void light_1_red(struct mosquitto *mosq)
{
    if(current_green_light ==1){
    wright_mqtt(mosq,"traffic_lights_order/light_1","orange");
    sleep(1);}
    wright_mqtt(mosq,"traffic_lights_order/light_1","red");
    light1_ordered_state = red;
}

void light_1_green(struct mosquitto *mosq)
{
    wright_mqtt(mosq,"traffic_lights_order/light_1","green");
    light1_ordered_state = green;
    current_green_light = 1;

}

void light_2_red(struct mosquitto *mosq)
{
   if(current_green_light ==2)
    {wright_mqtt(mosq,"traffic_lights_order/light_2","orange");
    sleep(1);}
    wright_mqtt(mosq,"traffic_lights_order/light_2","red");
    light1_ordered_state = red;
}

void light_2_green(struct mosquitto *mosq)
{
    wright_mqtt(mosq,"traffic_lights_order/light_2","green");
    light1_ordered_state = green;
    current_green_light = 2;
}


void switch_lights(struct mosquitto *mosq)
/*light goes from red to green or the oposit, same for light 2, but other collor*/
{
    if (light1_ordered_state == green)
    {
        light_1_red(mosq);
        light_2_green(mosq);
    }
    else
    {
        light_2_red(mosq);
        light_1_green(mosq);
        
    }
    current_green_light = next_green_light;
    next_green_light = 0;
  
}

void all_light_red(struct mosquitto *mosq)
/*all light goes red*/
{
    light_2_red(mosq);
    light_1_red(mosq);
    current_green_light = 0;
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
}



/***********TO DO************/

void restart_changeable(void* p)
{
    sleep(10);
    /*if(changeable_priority==0)
    {
        if(next_green_light == current_green_light ||current_green_light !=0) 
            {
                printf("\n\n\n\n\nprobleme\n\n\n\n\n\n");
                all_light_red(p);
            
            }
        
        else if (next_green_light == 0)
        {
            printf("\n\n!!!same value!!\n");
            all_light_red(p);
            next_green_light = 0;
            current_green_light = 0;


    }
    else
    {
        printf("\n\n!!!diff value!!\n");
    }
    }*/
    
    changeable = true;
}





void restart_timer(){}


void setup_mqtt(){}


void wright_mqtt(struct mosquitto * mosq,char* topic, char* info)
{
    mosquitto_publish(mosq, NULL, topic, 60, info, 0, false);

}

void manager(struct mosquitto *mosq,char* msg,char* topic)
{
    int separator_topic = strcspn(topic,"/");
    char delimiter[] = "/";
    char *main_topic = strtok(topic, delimiter);
    char *secundary_topic = strtok(NULL, delimiter);
    printf("main_topic : %s",main_topic);
    printf("secundary_topic : %s",secundary_topic);


    if(!strcmp(main_topic,  "car_position_info"))
    {
        
        if(!strcmp(secundary_topic,  "light_1"))
        {   
            if(current_green_light != 1)
            {
                if(changeable == true && changeable_priority == 0)
                {
                    light_2_red(mosq);
                    light_1_green(mosq);
                    

                    changeable = false;
                    pthread_t id;
                    int a = 0;
                    pthread_create(&id, NULL, restart_changeable, mosq);
                }
                else if(current_green_light==0)
                {
                    next_green_light = 1;
                }
            }
        }

        else
        {
            if(current_green_light != 2)
            {
                if(changeable == true && changeable_priority == 0)
                {
                    light_1_red(mosq);
                    light_2_green(mosq);
                    

                    changeable = false;
                    pthread_t id;
                    int a = 0;
                    pthread_create(&id, NULL, restart_changeable, mosq);
                }
                else if(next_green_light==0)
                {
                    next_green_light = 2;
                }
            }
        }
            



    }      
        
    
    else if(!strcmp(topic,  "traffic_lights_priority_command"))
    {
       
        if(!strcmp(secundary_topic,  "light_1"))
        {
            

            if(!strcmp(msg,  "demand"))
            {
                if(changeable_priority == 0)
                {
                    printf("\n\n\n\naaaaa\n");
                    light_2_red(mosq);
                    light_1_green(mosq);
                    
                    changeable_priority = 1;
                }
            }
            else if(!strcmp(msg,  "release") && changeable_priority == 1)
            {
                changeable_priority = 0;
                all_light_red(mosq);
            }
            
        }
        else
        {
            if(!strcmp(msg,  "demand"))
            {
                if(changeable_priority == 0)
                {
                    light_1_red(mosq);
                    light_2_green(mosq);
                    
                    changeable_priority = 2;
                }
            }
            else if(!strcmp(msg,  "release") && changeable_priority == 2)
            {
                changeable_priority = 0;
                all_light_red(mosq);
            }
            
        }
        
    }    
}



/***********MQTT***************/

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "#", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
    manager(mosq,(char *) msg->payload,msg->topic);
	printf("\n\n--%s--\n\n", (char *) msg->payload);
}

int main(int argc, char *argv[])
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