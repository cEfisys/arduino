/* RestClient simple GET request
 *
 * by Efisis
 */

#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"

#include "DHT.h"
//#include <SPI.h>
//#include <SD.h>
#include <Time.h>
//#include <TimeAlarms.h>
#include <DS3232RTC.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


// Initialize DHT sensor
// Pin and Type
#define DHTPIN 2            
#define DHTTYPE DHT11   
DHT temperatureHumiditySensor(DHTPIN, DHTTYPE);

#define HTTP_DEBUG
RestClient client = RestClient("efisisbd.herokuapp.com");

//Setup
void setup() {
  Serial.begin(9600);
  Serial.println("connect to network");
  
  // Can still fall back to manual config:
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
  
  //This is needed to configured the client
  if (client.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using Manual Configuration");
  }
  Serial.println("Setup!");
  
  
  // Get the time from the RTC
  setSyncProvider(RTC.get);

delay(5000);
  // Report whether the Arduino was able to communicate with the RTC
  if (timeStatus() != timeSet)
  {
    Serial.println("Unable to sync with the RTC");
    lcd.print("Unable to sync with the RTC");
  }else
    {
      Serial.println("RTC has set the system time");
      lcd.print("RTC has set the system time");
    }

  // Init the DHT11 Sensor
  Serial.println("DHT11 test!");
  temperatureHumiditySensor.begin();

  // Read temperature and humidity values from the sensor
  airHumidity_1 = temperatureHumiditySensor.readHumidity();
  temperatureAir_1 = temperatureHumiditySensor.readTemperature();

  Serial.println("Device Startup");
}

String response;
void loop(){
  
  //Run's in each 10 seconds

    // Read temperature and humidity values from the sensor
    airHumidity_1 = temperatureHumiditySensor.readHumidity();
    temperatureAir_1 = temperatureHumiditySensor.readTemperature();
    
    Serial.println("************************");
  
  Serial.println("Humidade:");
Serial.println(airHumidity_1);
  Serial.println("Temperatura:");
    Serial.println(temperatureAir_1);
  response = "";
  
  /*********************************************************************** 
  Parameters supported for the plataform: note that this is a json request
  designation
  name
  value 
  ***********************************************************************/
  String aux = "";
   String stringOne = "/information_gathereds.json","{\"designation\":\"designation Arduino\",\"name\":\"Arduino\",\"value\":\"1.1\"}";
  String stringThree = stringOne + sensorValue;
  
    int statusCode = client.post("/information_gathereds.json","{\"designation\":\"designation Arduino\",\"name\":\"Arduino\",\"value\":\"1.1\"}", &response);
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  //there are no need to return a message, becaus of 200OK is a standarized message meaning message was perfectly sent
 Serial.print("Response body from server: ");
 Serial.println(response);
  delay(1000);

  
  
}

/*********************************
 *      SerialDebug              *
 *********************************/
void debugSerial()
{
  printDigitsDate(year());
    //printDigitsDate(m());
    printDigitsDate(day());
    Serial.println("Data Entry: ");
    
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.println();
    
    // Display message over Serial
    Serial.println("****Taking Sensor Readings****");
    
    // Print the temperature and humidity readings over the serial port
    Serial.print("Temperature: ");
    Serial.print(temperatureAir_1, DEC);
    Serial.print("C");
    Serial.print(" Humidity: ");
    Serial.print(airHumidity_1);
    Serial.println("%");
}



void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void printDigitsDate(int digits)
{
  //lcd.clear();
  // utility function for digital clock display: prints preceding ":" and leading 0
  Serial.print("/");
  //lcd.print("/");
  if (digits < 10)
  {
    Serial.print('0');
    }
  Serial.print(digits);
  //lcd.print(digits);
}




