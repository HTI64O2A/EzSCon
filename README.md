# EzSCon

small message connection library between Node.js and ESP devices for small or prototype IoT related project.

## Requirement

* Arduino (IDE or cli) for easy compiling.

  * [ArduinoJson 5.13.3](https://arduinojson.org/v5/doc/installation/)

  * [arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets)

* Node.js (Server)

* ESP8266 or ESP32

##

Event based code.

### example

Node.js
```JS
const PORT = 8080;
const ezscon = new (require('ezscon'))(PORT);

ezscon.on('temperature', data => {
    console.log(`Current temperature: ${data}`)
});
```

Arduino
```C++
#include "ezscon.h"
const char[] host = "192.168.0.1";
const int port = 8080;

void recive (String m) {
  Serial.print("Button toggle");
}

EZSCON net;
WiFiMulti WiFiMulti;

void setup() {
  net.setup(host, port);
  net.addEvent("button", recive); // hook event
  WiFiMulti.addAP("SSID", "PASSWORD"); // Connect wifi
}

void loop() {
  auto temp = sensor.getTemp(); // Get data...
  net.sendMessage("temperature", temp); // Sending message
  net.loop(); // loop
}
```

