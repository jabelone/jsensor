//Configure WiFi settings
#define wifissid "HSBNEWiFi" //Name of the WiFi network to connect to
#define wifipassword "HSBNEPortHack" //The password

//Configure MQTT settings
#define mqttclient "jsensor-bathroom" //The client name of this sensor
#define mqtttopic "jsensor/bathroom/volume" //The main topic of this sensor
#define mqttStatustopic "jsensor/bathroom/volume/status"
#define mqttserver "10.0.0.199" //IP address of the MQTT broker

//Sensor Settings
#define mintrigger 15 //Minimum number of seconds to stay triggered for
#define triggerpin D5 //Digital input the microphone sensor is connected to
#define averageseconds 10 //Seconds to keep the running average over
