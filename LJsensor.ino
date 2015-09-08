#include <HP20x_dev.h>
#include <KalmanFilter.h>

#include <DHT.h>

#include <rgb_lcd.h>
#include <Wire.h>
#include <Arduino.h>


// Grove Kit Sensor: temperature 
//const int pinTemp = A1;.
//const int B = 3975;

// Grove Kit Sensor: light 
const int pinLight = A0;

// Grove - Temperature and Humidity Sensor Pro
// http://www.seeedstudio.com/wiki/Grove_-_Temperature_and_Humidity_Sensor_Pro
#define DHTPIN A1     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);


// Grove - Barometer (High-Accuracy)
unsigned char ret = 0;
/* Instance */
KalmanFilter t_filter;    //temperature filter
KalmanFilter p_filter;    //pressure filter
KalmanFilter a_filter;    //altitude filter

//====================
rgb_lcd lcd;

void setup()
{
    lcd.begin(16, 2);
    // Configure the serial communication line at 9600 baud (bits per second.)
    Serial.begin(9600);

    dht.begin();


    // Grove - Barometer (High-Accuracy)
    /* Power up,delay 150ms,until voltage is stable */
    delay(150);
    /* Reset HP20x_dev */
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
}

void loop()
{
int v_int;
long v_long;

    lcd.setRGB(0,0,0);
    
    // Grove Kit Sensor: temperature 
    //v_int = analogRead(pinTemp);
    //float resistance = (float)(1023-v_int)*10000/v_int;
    //float temperature = 1/(log(resistance/10000)/B+1/298.15)-273.15;
    
    // Grove Kit Sensor: light 
    v_int = analogRead(pinLight);


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("light: ");
    lcd.print(v_int);

    delay(2000);

    // Grove - Temperature and Humidity Sensor Pro
    // http://www.seeedstudio.com/wiki/Grove_-_Temperature_and_Humidity_Sensor_Pro
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(t) || isnan(h)){
    } else {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("temp: ");
      lcd.print(t);
    
    
      lcd.setCursor(0,1);
      lcd.print("humid: ");
      lcd.print(h);
      
    }    



    // Wait one second between measurements.
    delay(2000);

    // Grove - Barometer (High-Accuracy)
    if(OK_HP20X_DEV == ret)
    { 
      //v_long = HP20x.ReadTemperature();
      //float bt = v_long/100.0;
      v_long = HP20x.ReadPressure();
      float bp = v_long/100.0;
      v_long = HP20x.ReadAltitude();
      float ba = v_long/100.0;

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

    
}
