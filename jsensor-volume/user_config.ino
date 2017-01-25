#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "RunningAverage.h"

//Other Settings
const int minTrigger = mintrigger;
const int threstholdSeconds = 10;
RunningAverage myRA(10); //Samples to store for average

//Program variables
bool triggered = 0;
int samples = 0;

//Called when MQTT topic is received
void callback(char* topic, byte* payload, unsigned int length) {
}

WiFiClient wifiClient;
PubSubClient client(mqttserver, 1883, callback, wifiClient);

void setup() {
  pinMode(triggerpin, INPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Attemtping WiFi connection to ");
  Serial.println(wifissid);

  WiFi.begin(wifissid, wifipassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to " + String(wifissid));
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Attemtping MQTT connection to ");
  Serial.print(mqttserver);
  Serial.print(" as ");
  Serial.println(mqttclient);

  if (client.connect((char*) mqttclient)) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Will publish to topic: ");
    Serial.println(mqtttopic);

    //String temptopic = mqtttopic + "/status"; 
    client.publish(mqttStatustopic, "boot complete");
    client.publish(mqtttopic, "0");
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
    myRA.addValue(digitalRead(triggerpin));
    Serial.println(mqtttopic + int(myRA.getAverage()));
    samples++;
    if (samples >= threstholdSeconds * 100) {
      samples = 0;
      myRA.clear();
    }
  }
  if (!triggered && myRA.getAverage() > 0.5) {
    triggered = 1;
    client.publish(mqtttopic, "1");
    delay(minTrigger * 1000);
  }
  else if (triggered && myRA.getAverage() < 0.5) {
    triggered = 0;
    client.publish(mqtttopic, "0");
  }
}
