# SOMA TANK ESP32 SKETCH(hardware team)
## What it does
-Monitors the water/liquid level in a tank based on data obtained from an ultrasonic sensor connected to the esp32 microcontroller.
-Controls the liquid level in the tank by use of a pump, either automatically or through a switch on a web app.
## MQTT communication 
-The sketch includes a PubSubClient library which enables the microcontroller to communicate over WiFi via the MQTT protocol.
-The information to be displayed on the web app is published under the topics esp1/distance, esp1/duration and esp1/volume.
-The control signal to be received from the switch in the web app is subscribed to under the esp1/runmotor topic.


