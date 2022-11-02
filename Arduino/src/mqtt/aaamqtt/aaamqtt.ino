#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Mooa";        // your network SSID (name)
char pass[] = "okokokok";    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// const char broker[] = "192.168.206.146"; // android
//const char broker[] = "192.168.206.146"; //
const char broker[]="10.42.0.1";
//const char broker[] = "192.168.206.15";// android co, PC broker
//const char broker[] = "192.168.163.15";
int        port     = 1883;
const char topic[]  = "feux_tricolor";

//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;



int count = 0;
void wright(char* topic, char* msg)
{
  mqttClient.beginMessage(topic);
  mqttClient.print(msg);
  mqttClient.endMessage();
  
}

void read()
{}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  delay(50);
  mqttClient.poll();

  unsigned long currentMillis = millis();
  int i = 0;

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;
    for(int i = 0; i<50; i++)

    //record random value from A0, A1 and A2
    {wright("/test/", "prout");
    wright("/test/", "prout2");
    delay(5000);
    

    // send message, the Print interface can be used to set the message contents
    Serial.println("was?\n");
    Serial.println(i);
    }
  }
}
