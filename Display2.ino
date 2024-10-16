void Display(String Dp){

   if(Dp == "Setup"){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.println(" HELLO ");
    display.display();
    delay(100);
    display.clearDisplay();
   }
   else if(Dp == "Status"){
    //Condition();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println(CO);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println(TotalCO2);
    delay(20);
    
    //display.display();

  /////////////////////////////////////////
    

     int SumCo = 0;
     for (int i = 0; i < 5; i++)
    {
     SumCo += CO;
    //delay(50);
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    
        if (SumCo <= 45)
        {
            LED("Green");
            digitalWrite(buzzer, HIGH);
            digitalWrite(ledPin, LOW);
            display.println(" Excellent ");
        }
        else if (SumCo > 45 && SumCo < 175) 
        {   
            digitalWrite(buzzer, HIGH);
            digitalWrite(ledPin, LOW);
            display.println(" GOOD ");
        }
        else if (SumCo >= 175 && SumCo < 500) 
        {   
            digitalWrite(buzzer, HIGH);
            digitalWrite(ledPin, LOW);
            display.println(" Moderate ");
        }
        else if (SumCo >= 500 && SumCo < 1000)
        {
            Sound("Short Beep");          
            digitalWrite(ledPin, HIGH);
            display.println(" Poor ");
        }
        else if (SumCo >= 1000 )
        {
          
          float Lat = gps.location.lat();
          float Lng = gps.location.lng();

          SMS = SMS + "\n";
          SMS = SMS + "https://www.google.com/maps/search/?api=1&query=";
          SMS = SMS + Lat;
          SMS = SMS + ",";
          SMS = SMS + Lng;
            Sound("Long Beep");
            digitalWrite(ledPin, HIGH);
            LED("Alert");
            display.println(" Hazardous ");
            LINE.notify("Hazardous");
            LINE.notify(SumCo/5);
            //LINE.notify(Lat);
            //LINE.notify(Lng);
            LINE.notify(SMS); 
            //sendDataToSheet();
            delay(1000);
        }
  display.display();
    
   }
   else if(Dp == "Gps Sent"){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.println(" SOS Sent ");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 20);
    display.println(" Push Button ");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 20);
    display.println(" To Stop Senting ");
    display.display();
    delay(1000);
    display.clearDisplay();


   }
   //delay(100);
   //display.clearDisplay();
   //display.display();
}

void displayPage0() {
  StatusTFT = 0;
  
  Status();
  //SENSOR();
  tft.fillRoundRect(8, 62, 145, 29, 5, ST77XX_WHITE); // Draw rounded rectangle for CO value
  tft.setTextSize(2);
  tft.setCursor(50, 70); // Set cursor position for CO value
  tft.println(CO); // Print CO value

  tft.setTextColor(ST77XX_BLACK); 
  tft.setTextSize(2);
  tft.setCursor(15, 70); // Set cursor position for CO text
  tft.println("CO"); // Print CO text


  tft.setTextSize(1); // Set text size to 1
  tft.setCursor(133, 72); // Set cursor position for ppm text
  tft.println("ppm"); // Print ppm text

  // CO2
  tft.fillRoundRect(8, 94, 145, 29, 5, ST77XX_WHITE); // Draw rounded rectangle for CO2 value
  tft.setTextSize(2); 
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(48, 100); // Set cursor position for CO2 value
  tft.println(TotalCO2); // Print CO2 valueTotalCO2

  tft.setTextColor(ST77XX_BLACK); // Set text color to black
  tft.setTextSize(2);
  tft.setCursor(15, 100); // Set cursor position for CO2 text
  tft.println("CO"); // Print CO2 text
  tft.setTextColor(ST77XX_BLACK); // Set text color to black


  tft.setTextSize(1.5);
  tft.setCursor(40, 109); 
  tft.println("2");
  tft.setCursor(133, 104);
  tft.setTextColor(ST77XX_BLACK); 
  tft.setTextSize(1);
  tft.println("ppm"); // Print ppm text
  Condition();


}

void displayPage1() {

  //SENSOR();
  StatusTFT = 1;
  float value1 = TotalCO2;
  float value2 = CO;
  float angle1 = map(value1, 400, 1500, 0, 180);
  float angle2 = map(value2, 0, 200, 0, 180);
  Serial.println(value2);
  tft.setRotation(-1);

  Text_String(70, 10, 2, ST77XX_WHITE , "CO");
  Text_String(70, 83, 2, ST77XX_WHITE , "CO");
  Text_String(93, 95, 1, ST77XX_WHITE , "2");
  Text_String(5, 50, 1, ST77XX_WHITE , "0%");
  Text_String(5, 115, 1, ST77XX_WHITE , "0%");
  Text_String(133, 50, 1, ST77XX_WHITE , "100%");
  Text_String(133, 115, 1, ST77XX_WHITE , "100%");

  drawHalfCircleMeter(METER_CENTER_X_1, METER_CENTER_Y_1, angle1, ST7735_RED, prevAngle1);

  drawHalfCircleMeter(METER_CENTER_X_2, METER_CENTER_Y_2, angle2, ST7735_BLUE, prevAngle2);

  // Display everything drawn so far
  prevAngle1 = angle1;
  prevAngle2 = angle2;

  if(SumStatus == 10){
    reading0 = digitalRead(buttonPin0);
    reading1 = digitalRead(buttonPin1);
    float Lat = gps.location.lat();
    float Lng = gps.location.lng();

    SMS = SMS + "\n";
    SMS = SMS + "https://www.google.com/maps/search/?api=1&query=";
    SMS = SMS + Lat;
    SMS = SMS + ",";
    SMS = SMS + Lng;
    Sound("Long Beep");
    digitalWrite(ledPin, HIGH);
    LED("Alert");
    //display.println(" Hazardous ");
    LINE.notify("Hazardous");
    LINE.notify(SumCo/5);
    //LINE.notify(Lat);
    //LINE.notify(Lng);
    LINE.notify(SMS); 
    //sendDataToSheet();
    delay(1000);
    if(reading1 == LOW && reading0 == LOW){
      LINE.notify("The person inside the car is still conscious.");
      delay(1000);
      LINE.notify("The person inside the car is still conscious.");
    }
  }

}


void Status(){
  tft.setRotation(-1); 
  if(WIFI_Status == 1){
    tft.drawBitmap(110,0,wifi_icon, 28, 28, TFT_WHITE);
    
  }
  else{
    tft.drawBitmap(110,0,Wifi_off, 25, 25, TFT_RED);
  }
  if(GPS_Status == 1){
    tft.fillRect(10, 20,10, 10, TFT_WHITE);
    tft.drawBitmap(10,2,GPS, 25, 25, TFT_WHITE);

  }
  else{
    
    tft.drawBitmap(10,2,GPS, 27, 27, TFT_WHITE);
    tft.fillRect(10, 25 , 27, 5, TFT_BLACK);
    //tft.drawBitmap(10,0,NO_GPS,29,29,TFT_RED);//RED
  }
  if(Humen_Status == 1){
    tft.drawBitmap(65,5,user_icon,15,15,TFT_WHITE);

  }
  else{
    tft.drawBitmap(65,5,user_icon,15,15,TFT_WHITE);//RED
  }
  //tft.setRotation(0); 
  
  //tft.drawBitmap(10,0,GPS, 29, 29, TFT_WHITE);
  //tft.drawBitmap(10,0,NO_GPS,29,29,TFT_RED);

  //tft.drawBitmap(48,0,user_noti,40,33,TFT_WHITE);
  //tft.drawBitmap(48,0,notioff, 29, 29, TFT_RED);

  //tft.drawBitmap(90,0,wifi_icon, 28, 28, TFT_WHITE);
}

void MENU(){

  while(reading0 == LOW){ 
  buttonDown = millis(); 
  //Serial.println(reading1);
  //tft.fillRect(0, 0,160, 160, TFT_BLACK);
  reading1 = digitalRead(buttonPin1);
  Serial.println("LOOP Menu");
  SENSOR();
  displayPage1();
  ATC();
  ld2410();
  GPS_Module();
  if(reading1 == LOW){
      tft.fillRect(0, 0,160, 160, TFT_BLACK);
      while(reading0 == LOW){
        reading1 = digitalRead(buttonPin1);
        tft.setRotation(-1);

        tft.drawBitmap(15,5,QR, 120, 119, TFT_WHITE);
        if(reading1 == LOW){
          tft.fillRect(0, 0,160, 160, TFT_BLACK);
          break;

        }
      }
  }

  if(reading1 == LOW){
    tft.fillRect(0, 0,160, 160, TFT_BLACK);
    break;
  }
  /*
  while(!reading0) {
     //wait until the button is released
    if(millis()-buttonDown>100) {
      //button was held for 0.1 seconds
      MENU2();
      break;  //leave the loop
      }
    }
  }*/
  }

}
void MENU2(){

  while(reading1 == LOW){ //wait until the button is pushed (active-low, so TRUE=not pushed)
      tft.fillRect(0, 0,160, 160, TFT_BLACK);
      SENSOR();
      displayPage1();
      buttonDown2 = millis(); //record the time the button went down
  while(!reading1) {
     //wait until the button is released
    if(millis()-buttonDown2>100) {
      //button was held for 0.1 seconds

      MENU();
      break;  //leave the loop
      }
    }
  }
}

void Setup_Dis(){
  tft.initR(INITR_BLACKTAB);
  tft.fillRect(0, 0,160, 160, TFT_BLACK);
  tft.setRotation(-1);
  dw_font_init(&myfont,128, 160, draw_WHITE, clear_BLACK);

  dw_font_setfont(&myfont, &font_th_sarabunpsk_regular40);
  dw_font_goto(&myfont, 10, 43);
  dw_font_print(&myfont, "POWER ON");


}





// #########################################################################

// #########################################################################
void drawHalfCircleMeter(int centerX, int centerY, float angle, uint16_t color, float prevAngle) {
  tft.setRotation(-2);
  float prevX = centerX + (METER_RADIUS - 5) * cos((prevAngle - 90) * PI / 180);

    float prevY = centerY + (METER_RADIUS - 5) * sin((prevAngle - 90) * PI / 180);
    tft.drawLine(centerX, centerY, prevX, prevY, ST7735_BLACK);

    // Redraw markings
    for (int i = -90; i <= 90; i += 10) {
        float x1 = centerX + (METER_RADIUS - 2) * cos(i * PI / 180);
        float y1 = centerY + (METER_RADIUS - 2) * sin(i * PI / 180);
        float x2 = centerX + (METER_RADIUS + 2) * cos(i * PI / 180);
        float y2 = centerY + (METER_RADIUS + 2) * sin(i * PI / 180);
        tft.drawLine(x1, y1, x2, y2, ST7735_WHITE);
    }

    // Draw half circle
    for (int i = -90; i <= 90; i++) {
        float x1 = centerX + METER_RADIUS * cos(i * PI / 180);
        float y1 = centerY + METER_RADIUS * sin(i * PI / 180);
        float x2 = centerX + METER_RADIUS * cos((i + 1) * PI / 180);
        float y2 = centerY + METER_RADIUS * sin((i + 1) * PI / 180);
        tft.drawLine(x1, y1, x2, y2, ST7735_WHITE);
    }

    // Draw new needle line
    float x = centerX + (METER_RADIUS - 5) * cos((angle - 90) * PI / 180);
    float y = centerY + (METER_RADIUS - 5) * sin((angle - 90) * PI / 180);
    tft.drawLine(centerX, centerY, x, y, color);
  
}


void Text_String(int x, int y,int size, uint16_t color , String Text){

  tft.setTextColor(color); // Set text color
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.println(Text);
}
void Text_int(int x, int y,int size, uint16_t color , int Value){

  tft.setTextColor(color); // Set text color
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.println(Value);
}
void drawHalfCircle(int x, int y, int radius, uint32_t color) {
  for (int angle = 0; angle <= 180; angle++) { // Iterate from 0 to 180 degrees
    int xPos = x + cos((angle - 90) * PI / 180) * radius; // Calculate x position
    int yPos = y + sin((angle - 90) * PI / 180) * radius; // Calculate y position
    tft.drawPixel(xPos, yPos, color); // Draw pixel at calculated position
  }
}
 
void draw_WHITE(int16_t x, int16_t y)
{
  tft.drawPixel(x, y, ST77XX_WHITE);
}

void clear_BLACK(int16_t x, int16_t y)
{
  tft.drawPixel(x, y, ST77XX_BLACK);
}

void draw_BLACK(int16_t x, int16_t y)
{
  tft.drawPixel(x, y, ST77XX_BLACK);
}

void clear_WHITE(int16_t x, int16_t y)
{
  tft.drawPixel(x, y, ST77XX_WHITE);
}




 
/*
void header(const char *string, uint16_t color)
{
  tft.fillScreen(color);
  tft.setTextSize(1);
  tft.setTextColor(TFT_MAGENTA, TFT_BLUE);
  tft.fillRect(0, 0, 320, 30, TFT_BLUE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString(string, 160, 2, 4); // Font 4 for fast drawing with background
}*/