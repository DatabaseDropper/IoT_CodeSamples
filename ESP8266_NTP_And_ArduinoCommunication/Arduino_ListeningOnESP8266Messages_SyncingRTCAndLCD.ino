#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

DS3231 rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 16, 2);   
int counter = 0;

void handleDate(String date)
{
	// Parsers "Sunday-2022-4-24" in a poor way
    String delimiter = "-";
    size_t pos = 0;
    String token;
    int state_machine = 0;
    int year = 0;
    int month = 0;
    int day = 0;
    byte dayId = 1;
    while ((pos = date.indexOf(delimiter)) != -1) 
    {
        token = date.substring(0, pos);
        
        switch (state_machine)
        {
             case 0:
                  if (token == "Monday")
                  {
                    dayId = 1;
                  }
                  else if (token == "Tuesday")
                  {
                    dayId = 2;
                  }
                  else if (token == "Wednesday")
                  {
                    dayId = 3;
                  }
                  else if (token == "Thursday")
                  {
                    dayId = 4;
                  }
                  else if (token == "Friday")
                  {
                    dayId = 5;
                  }
                  else if (token == "Saturday")
                  {
                    dayId = 6;
                  }
                  else
                  {
                    dayId = 7;
                  }
                break;
             case 1:
                year = token.toInt();
                break;  
            case 2:
                month = token.toInt();
                break;
        }

        date.remove(0, pos + delimiter.length());
        state_machine++;
    }

    day = date.toInt();
    //Serial.print(day);
    //Serial.print("-");
    //Serial.print(month);
    //Serial.print("-");
    //Serial.println(year);
    rtc.setDOW(dayId);
    rtc.setDate(day, month, year);
}

void handleHour(String hour)
{
	// Parsers "08:23:45" in a poor way
    String delimiter = ":";
    size_t pos = 0;
    String token;
    int state_machine = 0;
    int hours = 0;
    int min = 0;
    int sec = 0;
    while ((pos = hour.indexOf(delimiter)) != -1) 
    {
        token = hour.substring(0, pos);
        
        switch (state_machine)
        {
             case 0:
                hours = token.toInt();
                break;
             case 1:
                min = token.toInt();
                break;  
        }

        hour.remove(0, pos + delimiter.length());
        state_machine++;
    }
    
    sec = hour.toInt();
    
    //Serial.print(hours);
    //Serial.print(":");
    //Serial.print(min);
    //Serial.print(":");
    //Serial.println(sec);
    
    rtc.setTime(hours, min, sec); 
}

void setup() 
{ 
  Serial.begin(115200);
  rtc.begin();
  lcd.init();
  lcd.backlight();
}

void loop() 
{
  counter++;
  if (counter >= 10 && Serial.available() > 0) 
  {
    //Serial.println("Syncing");
    String str = Serial.readString();
    String delimiter = "|";
    int splitIndex = str.indexOf(delimiter);
    String date = str.substring(0, splitIndex);
    String hour = str.substring(splitIndex + delimiter.length(), str.length());

    handleDate(date);
    handleHour(hour);
    counter = 0;
  }
  
  //Serial.print("Updating LCD");
  lcd.home();
  lcd.print(rtc.getDateStr());
  
  lcd.setCursor(0, 1);
  lcd.print(rtc.getTimeStr());
  
  delay(1000);
  lcd.clear();
}
