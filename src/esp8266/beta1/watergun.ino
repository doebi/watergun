/*
 *  This sketch trys to Connect to the best AP based on a given list
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>

const String nodeName = "doebi/watergun/";

// pins
const int trigger  = 0;
const int vibrator = 5;

int triggerState = HIGH;

int isAlive = millis();
int mil = 0;

ESP8266WiFiMulti wifiMulti;

IPAddress MQTTserver(158, 255, 212, 248);
WiFiClient wclient;
PubSubClient client(wclient, MQTTserver);

void mqtt_callback(const MQTT::Publish& pub) {
  String action = pub.payload_string();
  if (action == "vibrate") {
    digitalWrite(vibrator, HIGH);
    delay(1000);
    digitalWrite(vibrator, LOW);
  }
}

void setup() {
    Serial.begin(115200);
    delay(10);
	
    wifiMulti.addAP("/dev/lol", "4dprinter");
    
    if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("Wifi connected");
    }
    
    client.publish(nodeName + "heartbeat", 0);
    
    pinMode(trigger, INPUT);
    pinMode(vibrator, OUTPUT);
}

void loop() {
  // heartbeat
  mil = millis();
  if(mil - isAlive >= 30000) {
    client.publish(nodeName + "heartbeat", (String)mil);
    isAlive = mil;  
  }
  
  // wifi
  if(WiFi.status() != WL_CONNECTED) {
    if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("Wifi connected");
    }
    delay(1000);
  }

    // mqtt
    if (client.connected()) {
      client.loop();
    } else {
      if (client.connect("node")) {
        Serial.println("MQTT connected");
	      client.set_callback(mqtt_callback);
	      client.subscribe("doebi/watergun/action");
      }
    }

    if(digitalRead(trigger) != triggerState) {
      if(triggerState) { 
        client.publish(nodeName + "button", "DOWN");
      } else {
        client.publish(nodeName + "button", "UP ");
      }
      triggerState = !triggerState;
    }

    delay(100);
}
