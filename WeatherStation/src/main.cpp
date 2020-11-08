#include <Arduino.h>

#include <Wire.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#define seaLevelPressure_hPa 1013.25

#include <Adafruit_BMP085.h>

#ifndef LED_BUILTIN_ESP01
#define LED_BUILTIN_ESP01 2
#endif

Adafruit_BMP085 bmp;
bool bmp_status;

ESP8266WebServer server(80);

void updateStatus() {
  
}

void handleRootPath() {            //Handler for the rooth path

    String result = "";

    if (bmp_status) {
  
      result += "Temperature = ";
      result += bmp.readTemperature();
      result += " *C\n";
      
      result += "Pressure = ";
      result += bmp.readPressure();
      result += " Pa\n";
  
      result += "Altitude = ";
      result += bmp.readAltitude();
      result += " meters\n";
  
      result += "Pressure at sealevel (calculated) = ";
      result += bmp.readSealevelPressure();
      result += " Pa\n";
  
      result += "Real altitude = ";
      result += bmp.readAltitude(seaLevelPressure_hPa * 100);
      result += " meters\n";

    } else {
      result += "No sensor detected.";
    }
 
  server.send(200, "text/plain", result);
 
}
 
void setup() {

  Wire.begin(0, 2);

  delay(2000);
 
  Serial.begin(9600);
  WiFi.begin("Javlyn", "jch5602881");  //Connect to the WiFi network
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
 
    delay(500);
    Serial.println("Waiting to connect…");
 
  }

  Serial.print("Connected!");
 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP
 
  server.on("/other", []() {   //Define the handling function for the path
 
    server.send(200, "text / plain", "Other URL");
 
  });
 
  server.on("/", handleRootPath);    //Associate the handler function to the path
  server.begin();                    //Start the server
  Serial.println("Server listening");

  bmp_status = bmp.begin();
 
}
 
void loop() {
 
  server.handleClient();         //Handling of incoming requests
 
}