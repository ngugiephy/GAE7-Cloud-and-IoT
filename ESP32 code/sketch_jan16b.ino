#include <WiFi.h>
#include <PubSubClient.h>

const char ssid[] = "GEARBOX MEMBERS";
const char pass[] = "Members@Gearbox";

#define mqtt_server "192.168.0.86"
WiFiClient espClient;
PubSubClient client(espClient);

// Topics
#define mqtt_motor_control "esp1/runmotor"
#define mqttdistance "esp1/distance"
#define mqttduration "esp1/duration"
#define mqttvolume "esp1/volume"

// Motor control pins
#define enablePin 14
#define highPin 26
#define lowPin 27
#define trigPin 5
#define echoPin 18



float duration,distance,volume,newVol;
int Avgvol;
float area =2550.336;
void ultrasonic_control(){
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    duration =pulseIn(echoPin,HIGH);
    distance = (duration/2) * 0.0343;
    volume =distance *area;
    newVol =(204026.88-volume);
    Avgvol =int(newVol/1000);


        if(distance >10.0){
            //TURN ON MOTOR
            digitalWrite(highPin,HIGH);
            digitalWrite(lowPin,LOW);
            analogWrite(enablePin,100);
        }else{
            //TURN OFF MOTOR
            digitalWrite(highPin,LOW);
            digitalWrite(lowPin,LOW);
            analogWrite(enablePin,0);
        }
        if(distance <=400 || distance >=2){
      Serial.println("Distance = " + String(distance) + " cm");
      Serial.println("Duration" + String(duration) + " us.");
      client.publish(mqttdistance, String(distance).c_str());
      client.publish(mqttduration, String(duration).c_str());
      client.publish(mqttvolume,String(Avgvol).c_str());
} else{
     Serial.println("Out of range.");
}delay(500);
}



void callback(const MQTT::Publish& pub) {
    String payload_received = pub.payload_string();
    if (payload_received == "on") {
        digitalWrite(highPin,HIGH);
        digitalWrite(lowPin,LOW);
        analogWrite(enablePin,100);
        delay(5000);
        Serial.println("Switch is on");
        
    }
    else {
      digitalWrite(highPin,LOW);
      digitalWrite(lowPin,LOW);
      analogWrite(enablePin,0);
      Serial.println("Switch is off");
    }
  
    
}


void reconnect() {
    while (!client.connected()) {
        if (client.connect("My esp")) {
            Serial.println("Connected to MQTT broker");
           
        } else {
            Serial.print("Failed to connect, try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(enablePin, OUTPUT);
    pinMode(highPin,OUTPUT);
    pinMode(lowPin,OUTPUT);
    pinMode(trigPin,OUTPUT);
    pinMode(echoPin,INPUT);
    digitalWrite(enablePin, LOW);
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    client.set_server(mqtt_server);
    client.set_callback(callback);
    reconnect();
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    //client.loop();
    client.subscribe(mqtt_motor_control);
    ultrasonic_control();

    

}
