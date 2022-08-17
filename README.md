# Robotic IoT starter kit
MQTT Tank Robot

This is the MQTT Tank Robot firmware and it's part of the Robotic IoT starter kit.

Clone this repository and edit src/cloud.c with your personal mqtt broker hostname and port on line:

    #define ADDRESS     "tcp://test.mosquitto.org:1883"
    
It will use by default the Mosquitto free server, which works well, but there is no support or guarantee on it.
