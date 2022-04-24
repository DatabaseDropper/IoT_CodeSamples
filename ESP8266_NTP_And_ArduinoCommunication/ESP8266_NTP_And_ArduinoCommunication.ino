#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>

const char *ssid     = "MyWiFi";
const char *password = "admin123";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void setup() 
{
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(500);
  
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  
  // Time Zone in Warsaw, Poland (Warszawa)
  // Current Offset: 	UTC/GMT +2 hours
  timeClient.setTimeOffset(7200);
}

void loop() 
{
  timeClient.update();
  
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  
  String formattedTime = timeClient.getFormattedTime();
  String weekDay = weekDays[timeClient.getDay()];
  
  int currentYear = ptm->tm_year+1900;
  int currentMonth = ptm->tm_mon+1;
  int monthDay = ptm->tm_mday;
  
  // Sends date & time in a hacky format on both Serials - "it's own" and the other where Arduino is listening
  // Sunday-2022-4-24|08:23:45
  
  String currentDate = weekDay + "-" + String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "|" + formattedTime;  
  Serial.println(currentDate);
  Serial1.println(currentDate);
  delay(30000);
}