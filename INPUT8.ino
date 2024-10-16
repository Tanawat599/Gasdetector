void SENSOR(){
  /*
  Serial.println(Serial2.readString());
  String data = Serial2.readString();

// Find the position of the first and last spaces
  int firstSpace = data.indexOf(' ');
  int lastSpace = data.lastIndexOf(' ');

// Extract the substrings containing the numbers
  String co2_str = data.substring(0, firstSpace);
  String co_str = data.substring(lastSpace + 1);

// Convert the substrings to floats
  float TotalCO2 = co2_str.toFloat();
  float CO = co_str.toFloat();

// Print the floats
*/
/*
  Serial.print("CO2: ");
  Serial.println(TotalCO2);
  Serial.print("CO: ");
  Serial.println(CO);
  */


  short adc0 = ads.readADC_SingleEnded(0);
  float voltios = (adc0 * factorEscala) / 1000.0;
  MQ135.externalADCUpdate(voltios);

  MQ135.setA(605.18);
  MQ135.setB(-3.937);
  CO = MQ135.readSensor();

  MQ135.setA(110.47);
  MQ135.setB(-2.862);
  CO2 = MQ135.readSensor();

  TotalCO2 = CO2 + 400;
  SumCo = 0;
  SumCo2 = 0;
  
    for (int i = 0; i < 5; i++)
  {
    SumCo += CO;
    SumCo2 += TotalCO2;

    delay(50);
  }


  Serial.print("CO2: ");
  Serial.println(TotalCO2);
  Serial.print("CO: ");
  Serial.println(CO);

  //SerialESP.println(CO + " " + "CO2" + " " + TotalCO2);
  //SerialESP.println(CO);



}
void Sensor_Setup(){
  MQ135.setRegressionMethod(1);  
  MQ135.init();
  ads.begin();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++)
  {
    short adc0 = ads.readADC_SingleEnded(0);
    float voltios = (adc0 * factorEscala) / 1000.0;
    MQ135.externalADCUpdate(voltios);
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println("  done!.");
  
  if (isinf(calcR0))
  {
    Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
    while (1);
  }
  if (calcR0 == 0)
  {
    Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
    while (1);
  }
  


}

void GPS_Module(){

  if (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      if (gps.location.isValid()&&(millis() - lastDisplayTimeGPS >= 2000)) {
        String strLat = String(gps.location.lat(), 6); // Assuming 6 decimal places
        String strLng = String(gps.location.lng(), 6);
        Serial.print(F("- latitude: "));
        Serial.println(strLat);

        Serial.print(F("- longitude: "));
        Serial.println(strLng);
        GPS_Status = 1;
        Serial.print(F("- altitude: "));
        lastDisplayTime = millis();
        if (gps.altitude.isValid()){
          Serial.println(gps.altitude.meters());
          
        }
         else{
          Serial.println(F("INVALID"));
          GPS_Status = 0;
        }
      } else {
        Serial.println(F("- location: INVALID"));
      }

      Serial.print(F("- GPS date&time: "));
      if (gps.date.isValid() && gps.time.isValid()) {
        Serial.print(gps.date.year());
        Serial.print(F("-"));
        Serial.print(gps.date.month());
        Serial.print(F("-"));
        Serial.print(gps.date.day());
        Serial.print(F(" "));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        Serial.println(gps.time.second());
      } else {
        Serial.println(F("INVALID"));
      }

      Serial.println();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
  float Lat = gps.location.lat();
  float Lng = gps.location.lng();
/*
  SMS = SMS + "\n";
  SMS = SMS + "https://www.google.com/maps/search/?api=1&query=";
  SMS = SMS + 13.9620072;//Lat;
  SMS = SMS + ",";
  SMS = SMS + 100.6154272;//Lng;*/


}
void ld2410_SETUP(){
  
  MONITOR_SERIAL.begin(115200); //Feedback over Serial Monitor
  //radar.debug(MONITOR_SERIAL); //Uncomment to show debug information from the library on the Serial Monitor. By default this does not show sensor reads as they are very frequent.
  #if defined(ESP32)
    RADAR_SERIAL.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN); //UART for monitoring the radar
  #elif defined(__AVR_ATmega32U4__)
    RADAR_SERIAL.begin(256000); //UART for monitoring the radar
  #endif
  delay(500);
  MONITOR_SERIAL.print(F("\nConnect LD2410 radar TX to GPIO:"));
  MONITOR_SERIAL.println(RADAR_RX_PIN);
  MONITOR_SERIAL.print(F("Connect LD2410 radar RX to GPIO:"));
  MONITOR_SERIAL.println(RADAR_TX_PIN);
  MONITOR_SERIAL.print(F("LD2410 radar sensor initialising: "));
  if(radar.begin(RADAR_SERIAL))
  {
    MONITOR_SERIAL.println(F("OK"));
    MONITOR_SERIAL.print(F("LD2410 firmware version: "));
    MONITOR_SERIAL.print(radar.firmware_major_version);
    MONITOR_SERIAL.print('.');
    MONITOR_SERIAL.print(radar.firmware_minor_version);
    MONITOR_SERIAL.print('.');
    MONITOR_SERIAL.println(radar.firmware_bugfix_version, HEX);
  }
  else
  {
    MONITOR_SERIAL.println(F("not connected"));
  }
}

void ld2410(){
  radar.read();
  if(radar.isConnected() && millis() - lastReading > 1000)  //Report every 1000ms
  {
    lastReading = millis();
    if(radar.presenceDetected())
    {
      if(radar.stationaryTargetDetected())
      {
        
        Serial.print(F("Stationary target: "));
        Serial.print(radar.stationaryTargetDistance());
        Serial.print(F("cm energy:"));
        Serial.print(radar.stationaryTargetEnergy());
        Serial.print(' ');
      }
      if(radar.movingTargetDetected())
      {
        /*
        Serial.print(F("Moving target: "));
        Serial.print(radar.movingTargetDistance());
        Serial.print(F("cm energy:"));
        Serial.print(radar.movingTargetEnergy());*/
        
      }
      //Serial.println();
      Humen_Status = 1;
    }
    else
    {
      Serial.println(F("No target"));
      Humen_Status = 0;
    }
  }

}
void WiFiCheck(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  /*
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");
*/
  //Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  //Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  //Serial.println(timeWeekDay);
  Serial.println();

  char dayOfMonth[3];
  strftime(dayOfMonth, 3, "%d", &timeinfo);
  if (strcmp(dayOfMonth, "28") == 0) {
    Serial.println("WiFi Reset...");
    ESP.restart();
  }
  if (strcmp(dayOfMonth, "29") == 0) {
    Serial.println("WiFi Reset...");
    ESP.restart();
  }


}