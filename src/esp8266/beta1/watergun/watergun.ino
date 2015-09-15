/*
 * Firmware for an ESP powering a phyiscal watergun becoming a browsergame..
 * (c) by Christoph (doebi) Döberl
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>

String nodeName = "doebi/watergun/";

// pins
const int trigger  = 0;
const int vibrator = 5;

int triggerState = HIGH;
int vibratorState = LOW;

int vibratorStart = 0;
int vibratorTimeout = 1000;

int isAlive = millis();
int mil = 0;

ESP8266WiFiMulti wifiMulti;

IPAddress MQTTserver(158, 255, 212, 248);
WiFiClient wclient;
PubSubClient client(wclient, MQTTserver);

void mqtt_callback(const MQTT::Publish& pub) {
  String topic = pub.topic();
  String msg = pub.payload_string();
  if(topic == "doebi/watergun/vibrator") {
    if (msg == "ON") {
      vibratorStart = millis();
      vibratorState = HIGH;
      digitalWrite(vibrator, HIGH);
    }
    if (msg == "OFF") {
      vibratorState = LOW;
      digitalWrite(vibrator, LOW);
    }
  }
}

void setup() {
    Serial.begin(115200);
    delay(10);
	
    wifiMulti.addAP("/dev/lol", "4dprinter");
    wifiMulti.addAP("ESP-Spielwiese", "ovomaltine");
    
    if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("Wifi connected");
    }
    
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
  }

    // mqtt
    if (client.connected()) {
      client.loop();
    } else {
      if (client.connect("watergun", "doebi/watergun/online", 0, true, "false")) {
        client.publish("doebi/watergun/online", "true", true);
        Serial.println("MQTT connected");
	      client.set_callback(mqtt_callback);
	      client.subscribe("doebi/watergun/#");
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

    if(vibratorState == HIGH) {
      if(mil - vibratorStart >= vibratorTimeout) {
        digitalWrite(vibrator, LOW);
        vibratorState = LOW;
      }
    }
}
