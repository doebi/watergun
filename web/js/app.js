var mqtt;
var reconnectTimeout = 2000;
var host = "158.255.212.248";
var port = 8080;
var topic = "doebi/#";

function vibrate() {
    mqtt.send("doebi/watergun/vibrator", "ON");
    window.setTimeout(function(){
        mqtt.send("doebi/watergun/vibrator", "OFF");
    }, 500);
}

function MQTTconnect() {
    console.log("connecting");
    mqtt = new Paho.MQTT.Client(
                    host,
                    port,
                    "web_" + parseInt(Math.random() * 100,
                    10));
    var options = {
        timeout: 3,
        onSuccess: onConnect,
        onFailure: function (message) {
            setTimeout(MQTTconnect, reconnectTimeout);
        }
    };

    mqtt.onConnectionLost = onConnectionLost;
    mqtt.onMessageArrived = onMessageArrived;

    console.log("Host="+ host + ", port=" + port);
    mqtt.connect(options);
}

function onConnect() {
    console.log('Connected to ' + host + ':' + port);
    mqtt.subscribe(topic, {qos: 0});
    console.log(topic);
}

function onConnectionLost(response) {
    setTimeout(MQTTconnect, reconnectTimeout);
    console.log("connection lost: " + response.errorMessage + ". Reconnecting");
};

function onMessageArrived(message) {

    var topic = message.destinationName;
    var payload = message.payloadString;

    console.log(topic + ' = ' + payload);

    switch (topic) {
        case 'doebi/watergun/heartbeat':
            document.getElementById("uptime").innerHTML = moment(moment().valueOf() - Number(payload)).toNow(true);            
            break;
        case 'doebi/watergun/button':
            document.getElementById("trigger").innerHTML = payload;            
            break;
        default:
            break;
    }
};


window.onload = function() {
    console.log("booting up");
    MQTTconnect();
};

