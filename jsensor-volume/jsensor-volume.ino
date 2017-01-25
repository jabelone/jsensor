#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "RunningAverage.h"

//WiFi Settings
const char* WiFiSSID = "HSBNEWiFi";
const char* WiFiPassword = "HSBNEPortHack";

//MQTT Settings
String MQTTclientName = "jsensor-bathroom";
String MQTTtopic = "jsensor/bathroom/volume";
char* MQTTserver = "10.0.0.199";

//Other Settings
const int minTrigger = 15;
const int signalPin = D5;
const int threstholdSeconds = 10;
RunningAverage myRA(10); //Samples to store for average

//Program variables
bool triggered = 0;
int samples = 0;

//Called when MQTT topic is received
void callback(char* topic, byte* payload, unsigned int length) {
}

WiFiClient wifiClient;
PubSubClient client(MQTTserver, 1883, callback, wifiClient);

void setup() {
  pinMode(signalPin, INPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Attemtping WiFi connection to ");
  Serial.println(WiFiSSID);

  WiFi.begin(WiFiSSID, WiFiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to " + String(WiFiSSID));
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Attemtping MQTT connection to ");
  Serial.print(MQTTserver);
  Serial.print(" as ");
  Serial.println(MQTTclientName);

  if (client.connect((char*) MQTTclientName.c_str())) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Will publish to topic: ");
    Serial.println(MQTTtopic);

    String temptopic = MQTTtopic + "/status";
    client.publish(temptopic.c_str(), "boot complete");
    client.publish(MQTTtopic.c_str(), "0");
  }
  else {
    Serial.println("Couldn't connect to MQTT Broker");
    Serial.println("Will reset and try again...");
    abort();
  }
  myRA.clear(); // explicitly start clean
}

void loop() {

  if (millis() % 10 == 0) {
    myRA.addValue(digitalRead(signalPin));
    Serial.println(MQTTtopic + myRA.getAverage());
    samples++;
    if (samples >= threstholdSeconds * 100) {
      samples = 0;
      myRA.clear();
    }
  }
  if (!triggered && myRA.getAverage() > 0.5) {
    triggered = 1;
    client.publish(MQTTtopic.c_str(), "1");
    delay(minTrigger * 1000);
  }
  else if (triggered && myRA.getAverage() < 0.5) {
    triggered = 0;
    client.publish(MQTTtopic.c_str(), "0");
  }
}
