#include <WiFi.h>
#include <MQTT.h>
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>


const char ssid[] ="GEARBOX MEMBERS";
const char pass[] ="Members@Gearbox";
//include mqtt client
#include <PubSubClient.h>
#define mqtt_server "192.168.0.226" // ip address of the server
WiFiClient espClient;
PubSubClient client(espClient);
//defining the topics that are gonna be sent
#define mqttdistance "esp1/distance"
#define mqttduration "esp1/duration"
//MQTT setup end

//define input and output pin
int trigPin = 5;
int echoPin =18;
float duration,distance;

void reconnect(){
  //loop till we are reconnected
  int counter =0;
  while(!client.connected()){
    if(counter == 5){
      ESP.restart();
    }
    counter += 1;
    Serial.print("Attempting internect connection...");
    //attempting mqtt connection
    if(client.connect("My esp")){
      Serial.println("connected");
    }else{
      Serial.print("failed,rc=");
     // Serial.print(client.state());
      Serial.println("Try again in 5 seconds");
      //try again after 5s
      delay(5000);
    }
  }

}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  //begin WiFi connect
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(2000);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.println("WiFi connected");
  Serial.println("Ip address");
  Serial.println(WiFi.localIP());
  //end wifi connect
client.set_server(mqtt_server);



}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  // put your main code here, to run repeatedly:
  //supply a 10 microsecond pulse to the trigpin
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  //getting data from the echo pin
  duration =pulseIn(echoPin,HIGH);

  // calculating the distance 

  distance = (duration/2) * 0.0343;
  //publishing to the server
  client.publish(mqttdistance,String(distance).c_str());
  client.publish(mqttduration,String(duration).c_str());

  //display the result 
  Serial.print("Distance =");
  if(distance >=400 || distance <=2){
    Serial.print("Out of range");
    Serial.println();
  }else{
    Serial.print(distance);
    Serial.print("cm");
    delay(500);
    Serial.println();
  }
  delay(500);


}
