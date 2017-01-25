#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "HSBNEWiFi";
const char* password = "HSBNEPortHack";

String topic = "jsensor/bathroom/volume";
char* server = "10.0.0.199";

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

#define signalPin D5
int threstholdTime = 5000;
bool triggered = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Generate client name based on MAC address and last 8 bits of microsecond counter
  String clientName;
  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);

  Serial.print("Connecting to ");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(clientName);
  
  if (client.connect((char*) clientName.c_str())) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(topic);
    
      String temptopic = topic + "/status";
      client.publish(temptopic.c_str(), "boot complete");
      client.publish(topic.c_str(), "0");
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }

  pinMode(signalPin, INPUT);
}

void loop() {

  if (!triggered && digitalRead(signalPin)) {
    delay(threstholdTime);
    if(digitalRead(signalPin)) {
      triggered = 1;
      Serial.println("triggered");
      client.publish(topic.c_str(), "1");
    }
  }
  else if (triggered && !digitalRead(signalPin)) {
    delay(threstholdTime);
    if(!digitalRead(signalPin)) {
      triggered = 0;
      Serial.println("not triggered");
      client.publish(topic.c_str(), "0");
    }
  }
}
