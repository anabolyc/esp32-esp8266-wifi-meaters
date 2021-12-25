# Signal Strength Meter

Measure the signal strength of your router around the house with this Signal Strength Meter. Once the WiFi is configured by joining the "Home Network Study" network and setting the credentials via the captive portal, the analogue meter will reflect the signal strength (RSSI) of the router at that location.

## Fork notice

This is a project made by [David Chatting](https://github.com/davidchatting/ThreeWiFiMeters) that displays Wifi signal strength. David attached it to actial analog gauge. My implementation draws gauge on the TFT screen. Additional changes I've made:

- I replaced YoYoWifiManager with basic Wifi setup, since I never made YoYoWifiManager work reliably
- I changed the structure of the project to be run as Platformio project, because
  - I want to run is on both esp8266 and esp32
  - I want to run it with multiple screen options
 
### Why

I'm working on my own development boards for both listed MCUs and this project is a good demonstration of kind of projects they would happily support

### Configuration

Specify your wifi credentials at the top of [platformio.ini](/platformio.ini) file

### Demo

#### ESP8266

![esp8266](/doc/demo/VID_20211225_224932.gif)

#### ESP32

![esp32](/doc/demo/VID_20211220_220307.gif)


## Author

The Three WiFi Meters were created by David Chatting ([@davidchatting](https://twitter.com/davidchatting)) as part of the [A Network of One's Own](http://davidchatting.com/nooo/) project. This code is licensed under the [MIT License](LICENSE.txt).

This adaptation is prepared by [Andriy Malyshenko](https://github.com/anabolyc).