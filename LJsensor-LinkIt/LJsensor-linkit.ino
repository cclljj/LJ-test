#include <LWiFi.h>
#include <LWiFiClient.h>
//#include <LWiFiServer.h>
//#include <LWiFiUdp.h>



#include <DHT_linkit.h>
#include <Digital_Light_TSL2561.h>
#include <HP20x_dev.h>
#include <KalmanFilter.h>
#include <rgb_lcd.h>
#include <Wire.h>
#include <Arduino.h>

//#include <SPI.h>
//#include <WiFi.h>

// Grove Kit Sensor: temperature 
//const int pinTemp = A1;.
//const int B = 3975;

// Grove Kit Sensor: light 
//const int pinLight = A0;

// Grove - Temperature and Humidity Sensor Pro
// Interface: Analog
// http://www.seeedstudio.com/wiki/Grove_-_Temperature_and_Humidity_Sensor_Pro
// for LinkIt One, use the library: https://github.com/Seeed-Studio/Grove_Starter_Kit_For_LinkIt/tree/master/libraries/Humidity_Temperature_Sensor
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT_linkit dht(DHTPIN, DHTTYPE);


// Grove - Barometer (High-Accuracy)
// Interface: I2C
// http://www.seeedstudio.com/wiki/Grove_-_Barometer_(High-Accuracy)

unsigned char ret = 0;
/* Instance */
KalmanFilter t_filter;    //temperature filter
KalmanFilter p_filter;    //pressure filter
KalmanFilter a_filter;    //altitude filter

//====================
// Grove - LCD RGB Backlight
// Interface: I2C
// http://www.seeedstudio.com/wiki/Grove_-_LCD_RGB_Backlight
rgb_lcd lcd;



// LinkIt One WiFi
#define WIFI_AP "mymaktub"            // replace your WiFi AP SSID
#define WIFI_PASSWORD "27858757"  // replace your WiFi AP password
#define WIFI_AUTH LWIFI_WPA           // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP according to your AP

LWiFiClient c;


void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    // Configure the serial communication line at 9600 baud (bits per second.)

    // Grove - Barometer (High-Accuracy)
    // Power up,delay 150ms,until voltage is stable
    delay(150);
    // Reset HP20x_dev
    HP20x.begin();
    delay(100);
  
    /* Determine HP20x_dev is available or not */
    ret = HP20x.isAvailable();
    if(OK_HP20X_DEV == ret)
    {
      Serial.println("HP20x_dev is available.\n");    
    }
    else
    {
      Serial.println("HP20x_dev isn't available.\n");
    }

    // Grove - Temperature and Humidity Sensor Pro
    dht.begin();

    // Grove - Digital Light Sensor
    TSL2561.init();
    
    // LinkIt One WiFi
    // check for the presence of the shield:
    
    LWiFi.begin();
    Serial.print("Connecting to WiFi AP: ");
    Serial.println(WIFI_AP);
    while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD))){
      Serial.println("Fail and try it again...");
      delay(500);
    }
    Serial.print("My IP is: ");
    Serial.println(LWiFi.localIP());
    
}

void loop()
{
int v_int;
long v_long;
long light;
float ba, bp;
 
    lcd.setRGB(0,0,0);

    // Grove - Digital Light Sensor
    light = TSL2561.readVisibleLux();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("light: ");
    lcd.print(light);

    if(light<750){
      lcd.setRGB(0,0,255);
    } else {
      lcd.setRGB(0,0,0);
    }

    delay(2000);

    // Grove - Temperature and Humidity Sensor Pro
    // http://www.seeedstudio.com/wiki/Grove_-_Temperature_and_Humidity_Sensor_Pro
//    float h = dht.readHumidity();
//    float t = dht.readTemperature();
    // for LinkIt One, use the library: https://github.com/Seeed-Studio/Grove_Starter_Kit_For_LinkIt/tree/master/libraries/Humidity_Temperature_Sensor
    
    float h,t;
    dht.readHT(&t, &h);
    while (isnan(t) || isnan(h)){
      dht.readHT(&t, &h);    
    }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("temp: ");
      lcd.print(t);
    
      lcd.setCursor(0,1);
      lcd.print("humid: ");
      lcd.print(h);
      


    // Wait one second between measurements.
    delay(2000);

    // Grove - Barometer (High-Accuracy)
    if(OK_HP20X_DEV == ret){ 
      //v_long = HP20x.ReadTemperature();
      //float bt = v_long/100.0;
      v_long = HP20x.ReadPressure();
      bp = v_long/100.0;
      v_long = HP20x.ReadAltitude();
      ba = v_long/100.0;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("pressure: ");
      //lcd.print(bt);
      //lcd.print(" ");
      lcd.print(bp);
      
      lcd.setCursor(0, 1);
      lcd.print("altitude: ");
      lcd.print(ba);

      delay(2000);
    }


      String data = "field1="+String(t)+"&field2="+String(h)+"&field3="+String(bp)+"&field4="+String(light);
      String data_len = String(data.length());
      Serial.println("Data: "+data);
      
    Serial.print("Connecting to site...");
    if(!c.connect("api.thingspeak.com", 80)){
      Serial.println("FAIL!");
      Serial.println(LWiFi.localIP());
    } else {
      c.println("POST /update HTTP/1.1");
      c.println("Host: api.thingspeak.com");
      c.println("Connection: close");
      c.println("X-THINGSPEAKAPIKEY: B4MIQ67R4DJWIK7U");
      c.println("Content-Type: application/x-www-form-urlencoded");
      c.println("Content-Length: " + data_len);
      c.println();
      c.println(data);
      

      // read server response
      while (c){
        Serial.print((char)c.read());
      }
      c.stop();
      delay(5000);
    }
    

    
    
    // Grove Kit Sensor: temperature 
    //v_int = analogRead(pinTemp);
    //float resistance = (float)(1023-v_int)*10000/v_int;
    //float temperature = 1/(log(resistance/10000)/B+1/298.15)-273.15;
    // Grove Kit Sensor: light 
    //v_int = analogRead(pinLight);
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("light: ");
    //lcd.print(v_int);
    
}
