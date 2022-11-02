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

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Mooa";    // your network SSID (name)
char pass[] = "okokokok";    // your network password (use for WPA, or use as key for WEP)



WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[]="10.42.0.1";
int        port     = 1883;
const char topic[]  = "#";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
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

String mqttClientread()
{
  String msg = "";
  int value = 1;
  while (mqttClient.available() && value !=0) {
      value=(int)mqttClient.read();
      if(value !=0)msg +=(char)value;
  }
  return msg;
  
}


void loop() {

  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    String topic = (String)mqttClient.messageTopic();
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");
    
    Serial.println(topic);
    String monmessageururu="";
    monmessageururu = mqttClientread();
    Serial.println("messageururu");    
    
    Serial.println(monmessageururu); 
    Serial.println("fin message");

    Serial.println();
  }
  delay(100);
}
