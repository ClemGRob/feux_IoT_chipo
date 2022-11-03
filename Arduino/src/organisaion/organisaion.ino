/**
 * @file organisaion.cpp
 * 
 * @author clement 
 * @brief 
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <Thread.h>
#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_NICLA_VISION) || defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
#endif

Thread send_sensor_value = Thread();
Thread send_light_value = Thread();
Thread get_value = Thread();


/***********PIN, light and sensor*******/
#define pirPin 2
#define vert_rgb_0 10 // vert_rgb_0
#define rouge_rgb_0 A1 // rouge_RGB_0
#define vert_rgb_1 9 // vert_rgb_1
#define rouge_rgb_1 A4 // rouge_RGB_1
#define echoPin1 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin1 5 //attach pin D3 Arduino to pin Trig of HC-SR04
#define echoPin2 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin2 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define pourcentageRapportCycliqueChoisi  30  

int Etat_Bouton = 0 ;
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int wait = 0; 
int clignotement = 0;





/*********MQTT common function********/

char ssid[] = "Mooa";    // your network SSID (name)
char pass[] = "okokokok";    // your network password (use for WPA, or use as key for WEP)



WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[]="10.42.0.1";
int        port     = 1883;
const char topic[]  = "#";

void wright_mqtt(char* topic, char* msg)
{
  
  mqttClient.beginMessage(topic);
  mqttClient.print(msg);
  mqttClient.endMessage();

}

String read_mqtt()
{
  String msg = "";
  int value = 1;
  while (mqttClient.available() && value !=0) {
      value=(int)mqttClient.read();
      if(value !=0)msg +=(char)value;
  }
  return msg;
  
}


/***************sensor***************/

int presence1() {
  digitalWrite(trigPin1, LOW);
  delay(2);
  digitalWrite(trigPin1, HIGH);
  delay(10);
  digitalWrite(trigPin1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin1, HIGH);
  // Calculating the distance
  
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  //Displays the distance on the Serial Monitor
  Serial.print("distance1 : ");
  Serial.println(distance);
  return (distance);
}
int presence2()
{
  digitalWrite(trigPin2, LOW);
  delay(2);
  digitalWrite(trigPin2, HIGH);
  delay(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin2, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  //Displays the distance on the Serial Monitor
  Serial.print("distance1 : ");
  Serial.println(distance);
  return (distance);
}

/****************light*****************/

int Traf_light_color(int couleur,int select_feu) {
  Serial.println(select_feu);
  Serial.println(couleur);
  if (select_feu == 0) {
    switch (couleur) {
    case 1: //rouge
      // feu rouge
      analogWrite(vert_rgb_0, 0);
      analogWrite(rouge_rgb_0, 255);
      break;

    case 2: //orange
      // feu orange
      analogWrite(vert_rgb_0, 20);
      analogWrite(rouge_rgb_0, 255);
      break;

    case 3: //vert
      // feu vert
      analogWrite(vert_rgb_0, 255);
      analogWrite(rouge_rgb_0, 0);
      break;

    case 4: //Eteint
      // feu vert
      analogWrite(vert_rgb_0, 0);
      analogWrite(rouge_rgb_0, 0);
      break;
    default:
    break;
    }
  }
  else {
    if( select_feu == 1){
      switch (couleur) {
      case 1: //rouge
       // feu rouge
        analogWrite(vert_rgb_1, 0);
        analogWrite(rouge_rgb_1, 255);
        break;

      case 2: //orange
        // feu orange
        analogWrite(vert_rgb_1, 20);
        analogWrite(rouge_rgb_1, 255);
        break;

      case 3: //vert
      // feu vert
        analogWrite(vert_rgb_1, 255);
        analogWrite(rouge_rgb_1, 0);
        break;

      case 4: //Eteint
        analogWrite(vert_rgb_1, 0);
        analogWrite(rouge_rgb_1, 0);
        break;

      default:
        break;
      }
    }
    else
    {
      Serial.print("erreur");
    }
  }
}


/***************thread*****************/

// get vultrasound sensor and wright mqtt
void function_sensor_value()
{
  int sens1 = presence1();
  if(sens1 <20)
  {
    wright_mqtt("car_position_info/light_1","presence");
    }
  int sens2 = presence2();
  if(sens2 <20)wright_mqtt("car_position_info/light_2", "presence");

}

//wright mqtt
void function_send_light_value()
{

//wright_mqtt(char* topic, char* msg);
//wright_mqtt(char* topic, char* msg);

delay(1000);

}

//read mqtt
void get_light_value()
{
    int messageSize = mqttClient.parseMessage();
    //Serial.print("Received a message with topic '");
    if (messageSize) {
        Serial.print("Received a message with topic '");
        String topic = (String)mqttClient.messageTopic();
        Serial.print("', length ");
        Serial.print(messageSize);
        Serial.println(" bytes:");
        
        Serial.println(topic);
        String msg="";
        msg = read_mqtt();

        int separator_topic = topic.indexOf("/");

        String main_topic = "";
      
        
        for(int i = 0; i<separator_topic; i++)main_topic+=topic[i];
        Serial.print("main_topic ");
        Serial.println(main_topic);
       
        if(main_topic.equals("traffic_lights_order"))
        {
          Serial.println("inside trafic\n\n\n");
            String secundary_topic = "";
            for(int i = separator_topic; i<topic.length(); i++)secundary_topic+=topic[i];
            if(secundary_topic.equals("light_1"))
            {   
                if(msg.equals("green"))
                {
                    Traf_light_color(1,3);
                }
                if(msg.equals("orange"))
                {
                    Traf_light_color(1,2);
                }
                if(msg.equals("red"))
                {
                    Traf_light_color(1,1);
                }
            }
            else
            {
                if(msg.equals("green"))
                {
                    Traf_light_color(2,3);
                }
                if(msg.equals("orange"))
                {
                    Traf_light_color(2,2);
                }
                if(msg.equals("red"))
                {
                    Traf_light_color(2,1);
                }
            }
        }
        

       }

}




void setup(){
	Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }





    int rapportCycliqueEntre0et255 = map(pourcentageRapportCycliqueChoisi, 0, 100, 0, 255);
    
    pinMode(pirPin, INPUT_PULLUP);
    pinMode(trigPin1, OUTPUT); // Sets the trigPin1 as an OUTPUT
    pinMode(echoPin1, INPUT); // Sets the echoPin as an INPUT
    pinMode(trigPin2, OUTPUT); // Sets the trigPin1 as an OUTPUT
    pinMode(echoPin2, INPUT); // Sets the echoPin as an INPUT
    
    pinMode(vert_rgb_0, OUTPUT);                            
    analogWrite(vert_rgb_0, rapportCycliqueEntre0et255);
    pinMode(vert_rgb_1, OUTPUT);
    analogWrite(vert_rgb_1, rapportCycliqueEntre0et255);
    pinMode(rouge_rgb_0, OUTPUT);
    pinMode(rouge_rgb_1, OUTPUT);
    



  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();


}

void loop()
{
    /*if(sensor_value.shouldRun())
		sensor_value.run();
    if(send_light_value.shouldRun())
		send_light_value.run();*/
for(int main_iterator = 0; main_iterator<6; main_iterator++) {
    /**receiv info**/
    int messageSize = mqttClient.parseMessage();
  if (messageSize) { 
    // we received a message, print out the topic and contents
    
    String topic = (String)mqttClient.messageTopic();

    
    
    String msg="";
    msg = read_mqtt();

        int separator_topic = topic.indexOf("/");

        String main_topic = "";
        Serial.print("Topic : ");
        Serial.println(topic);
        Serial.print("msg : ");
        Serial.println(msg);
      
        
        for(int i = 0; i<separator_topic; i++)main_topic+=topic[i];
        Serial.print("main_topic ");
        Serial.println(main_topic);
        if(main_topic.equals("traffic_lights_order"))
        {
          Serial.print("je suis la ");
            String secundary_topic = "";
            for(int i = separator_topic+1; i<topic.length(); i++)secundary_topic+=topic[i];
            if(secundary_topic.equals("light_1"))
            {  
              Serial.print("je suis la 1");
                if(msg.equals("green"))
                {
                    Traf_light_color(3,0);
                }
                if(msg.equals("orange"))
                {
                    Traf_light_color(2,0);
                }
                if(msg.equals("red"))
                {
                    Traf_light_color(1,0);
                }
            }
            else
            {
              Serial.print("je suis la 2 ");
                 if(msg.equals("green"))
                {
                    Traf_light_color(3,1);
                }
                if(msg.equals("orange"))
                {
                    Traf_light_color(2,1);
                }
                if(msg.equals("red"))
                {
                    Traf_light_color(1,1);
                }
            }
        }
  }
  delay(100);
    /**send info**/
}
function_sensor_value();
}

