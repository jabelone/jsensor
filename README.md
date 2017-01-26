# jsensor
Open source sensor platform based on ESP8266 and compatible with MQTT.

# Usage
### jsensor-deepsleep-example
This demonstrates how you can use the deep sleep function to save a lot of power between measurements and transmissions.

### jsensor-volume
This is a volume based sensor.  It uses a microphone sensor with a digital (on/off) output.  It takes the running average of the last x seconds.  If the running average is more than 0.5 it sends ```jsensor/bathroom/volume 1``` to the configured MQTT broker.  There's a minimum "on" time to prevent rapid oscillation and when it turns off, it will send ```jsensor/bathroom/volume 0```.  It's simple to configure.  Just specify the WiFi, MQTT and sensor settings in the user_config file.
