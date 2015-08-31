# watergun

Create a self-contained browsergame within a physical watergun acting as wifi hotspot, server and controller.

## steps to succeed

* find suitable watergun
* fit components in it
* write code for interpreting gyro, accelero, etc..
* write code for serving web files from sd card
* communicate with browser over websockets
* create a fun game
* graphix
* soundz

## components

* water gun
* ESP8266 (webserver, sd-card reader, ..)
* Teensy 3.1 (processing unit)
* GY-80 (movement detection)
* click button (for trigger)
* LiPo

## history

* 13.08.15: (CCCamp15) First working prototype. Trigger is pimped with a pushbutton and sends messages via mqtt. We used this as trigger for a warnlight.

* 31.08.15: (beta1) Better logic and use of mqtt topics. Also added a vibration motor as feedback channel. Now 2-way communication works. \o/ Next step: fancy webinterface with control panel.
