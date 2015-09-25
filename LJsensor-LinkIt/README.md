
#List of the sensors used in this project

1. Grove - LCD RGB Backlight
  * http://www.seeedstudio.com/wiki/Grove_-_LCD_RGB_Backlight
2. Grove - Barometer (High-Accuracy)
  * http://www.seeedstudio.com/wiki/Grove_-_Barometer_(High-Accuracy)
3. Grove - Temperature and Humidity Sensor Pro
  * http://www.seeedstudio.com/wiki/Grove_-_Temperature_and_Humidity_Sensor_Pro
  * The original document mentions that this sensor has to be plugged on the analog input socket. However, this sensor is in fact compatible to the digital input. Please refer to this document: https://github.com/Seeed-Studio/Grove_Starter_Kit_For_LinkIt/tree/master/libraries/Humidity_Temperature_Sensor
  * In this project, this sensor is connected to D2 socket.
4. Grove - Digital Light Sensor
  * http://www.seeedstudio.com/wiki/Grove_-_Digital_Light_Sensor
  * It is planned to be replaced by the other sensors soon

#Example of My_Password.h
```javascript
  #ifndef _THINGSPEAK_API_KEY_
	#define _THINGSPEAK_API_KEY_
	#define THINGSPEAK_API_KEY "1234567890"
  #endif

  #ifndef _WIFI_
	#define _WIFI_

  #define WIFI_AP "MY_WIFI_AP"            // replace your WiFi AP SSID
  #define WIFI_PASSWORD "MY_WIFI_PASSWD"
  #define WIFI_AUTH LWIFI_WPA           // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP according to your AP
  #endif
```
