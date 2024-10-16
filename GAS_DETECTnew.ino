#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <MQUnifiedsensor.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <SoftwareSerial.h>

#include <ESP32Servo.h>
#include <HTTPClient.h>


//#include <ESP32Servo.h>
Servo myservo;
int pos = 0; 
int Relay = 19;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

////////////////////////////////////////COMMU////////////////////////////////////////
#include <WiFi.h>
#include <Preferences.h>
#include <WiFiManager.h> 
#include <TridentTD_LineNotify.h>
WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("server", "Line Token", "", 45);
String LINE_TOKEN = ""; 
int Token_Status  = 0;
//#define LINE_TOKEN  "OeFLPI3NclpT8Mrs7WDQj3E2L3EY0YuKzSWHA9tIrsS"
int WIFI_Status = 0 ;
////////////////////////////////////////GPS////////////////////////////////////////
#include <TinyGPS++.h>
int GPS_Status = 0;
#define GPS_BAUDRATE 9600 
unsigned long lastDisplayTime = 0;
//unsigned long displayInterval = 1000;
unsigned long lastDisplayTimeGPS = 0;
TinyGPSPlus gps;
int buzzer = 25;
float factorEscala = 0.1875F;

////////////////////////////////////////MQ135////////////////////////////////////////
#define placa "Arduino UNO"
#define type "MQ-135"
#define RatioMQ135CleanAir 3.6 
MQUnifiedsensor MQ135(placa, type);

int Sen = 0;
int SumStatus;
float CO2;
float CO;
float TotalCO2;
float SumCo ;
float SumCo2 ;
int StatusCO;
int StatusCO2;
Adafruit_ADS1115 ads;
#define OLED_RESET 16
Adafruit_SSD1306 display(OLED_RESET);


//unsigned long period = 1000;
//unsigned long last_time = 0;

////////////////////////////////////////SMS////////////////////////////////////////
String SMS;
String server = "http://maker.ifttt.com";
String eventName = "ESP8266Host";
String IFTTT_Key = "lxD-QgcowjeutPlCZk1U42Na9KT_z5-g3sqvfZSTZ9j";
String IFTTTUrl="http://maker.ifttt.com/trigger/temp_data/with/key/lxD-QgcowjeutPlCZk1U42Na9KT_z5-g3sqvfZSTZ9j";

////////////////////////////////////////LED////////////////////////////////////////
#include <FastLED.h>

#define LED_PIN     26
#define NUM_LEDS    20

CRGB leds[NUM_LEDS];
////////////////////////////////////////NAno////////////////////////////////////////

#define RXp2 16
#define TXp2 17 

#include <HardwareSerial.h>
////////////////////////////////////////BUTTON////////////////////////////////////////
int reading0;
int reading1;

const int buttonPin0 = 13;
const int buttonPin1 = 27;
const int ledPin = 2;

unsigned long previousTaskTime = 0;
const unsigned long taskInterval = 5000;

int buttonState;
unsigned long buttonDown;
int Exit = 13;
int ExitState;
unsigned long buttonDown2;
int mod = 0;
////////////////////////////////////////TFT////////////////////////////////////////
int Noti = 0;
unsigned long drawTime = 0;
int StatusTFT;
#include <Adafruit_GFX.h>  
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h> 
#include <SPI.h>
Adafruit_ST7735 tft = Adafruit_ST7735(15, 2, 23, 18, 4);
#include "bitmaps.h"
#include "TFT_eSPI.h"
#define M_SIZE 0.5

#define TFT_GREY 0x5AEB
#define TFT_ORANGE   
float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = M_SIZE*120, osy = M_SIZE*120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update
int old_analog =  -999; // Value last displayed

int value[6] = {0, 0, 0, 0, 0, 0};
int old_value[6] = { -1, -1, -1, -1, -1, -1};
int d = 0;

#define METER_RADIUS 50
#define METER_CENTER_X_1 5
#define METER_CENTER_Y_1 80
#define METER_CENTER_X_2 75
#define METER_CENTER_Y_2 80
#define PI 3.1415926535897932384626433832795
float prevAngle1 = 0;
float prevAngle2 = 0;

#define AA_FONT_SMALL "NotoSansBold15"
#define AA_FONT_LARGE "NotoSansBold36"

#include "dw_font.h"
extern dw_font_info_t font_th_sarabunpsk_regular40;
dw_font_t myfont;

////////////////////////////////////////LD2410////////////////////////////////////////
#define MONITOR_SERIAL Serial
#define RADAR_SERIAL Serial1
#define RADAR_RX_PIN 32
#define RADAR_TX_PIN 33
#include <ld2410.h>

ld2410 radar;

uint32_t lastReading = 0;
bool radarConnected = false;
int Humen_Status = 0;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Serial2.begin(GPS_BAUDRATE);
  Serial.println(F("ESP32 - GPS module"));

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin0, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(Relay, OUTPUT);

  myservo.attach(19);
  //myservo.write(0);
  myservo.writeMicroseconds(1500);

  digitalWrite(Relay , LOW);
  digitalWrite(buzzer, LOW);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  updateTime = millis();
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  WiFiCheck();

  //WiFi_Connect();
  Sound("Setup");
  Setup_Dis();
  //Display("Setup");
  ld2410_SETUP();
  //sendDataToSheet();
  ATC_Setup();
  Sensor_Setup();
  

  //tft.init();
  ads.startComparator_SingleEnded(2, 1000);
 

  LINE.setToken(LINE_TOKEN);
  LINE.notify("ยินดีต้อนรับ");
  Serial.println("Exit Setup");
  delay(200);
  /*tft.begin();
  tft.setRotation(-1);*/

}

void loop()
{

  //ld2410();
  //Nano();
  //Encoder();
  //SENSOR();
  //displayPage0();
  //displayPage1();
  //GPS_Module();
  //Display("Status");
  //display.display();
  //Condition();
  //ATC();
  //SerialESP.println("Hello from ESP32");
  //myservo.write(0);


  //******Develop by KIDINVETOR******
  myservo.writeMicroseconds(1500); 
  reading0 = digitalRead(buttonPin0);
  reading1 = digitalRead(buttonPin1);
  Serial.print(reading0);Serial.println(reading1);
  
  if(reading0 == LOW){
    tft.fillRect(0, 0,160, 160, TFT_BLACK);

  }
  if(Sen == 0){

    digitalWrite(buzzer, HIGH);
    dw_font_goto(&myfont, 10, 43);
    dw_font_print(&myfont, "Calibrating...");
    SENSOR();
    delay(2000);
    
    tft.fillRect(0, 0,160, 160, TFT_BLACK);
    Sen = 1;
  }

  MENU();
  SENSOR();
  displayPage0();
  ATC();
  ld2410();
  GPS_Module();
  Serial.println("LOOP");
  //WiFiCheck();
  
  delay(50);
}



void Condition(){
  //SENSOR();
  if (SumCo <= 50) StatusCO = 1;//45
  else if (SumCo < 100) StatusCO = 2;//175
  else if (SumCo < 150) StatusCO = 4;//500
  else if (SumCo < 200) StatusCO = 6;//1000
  else StatusCO = 10;

  if (SumCo2 <= 3000) StatusCO2 = 1;
  else if (SumCo2 < 4000) StatusCO2 = 2;
  else if (SumCo2 < 5000) StatusCO2 = 3;
  else if (SumCo2 < 7500) StatusCO2 = 4;
  else StatusCO2 = 10;

  SumStatus = StatusCO2 + StatusCO ;
  //SumStatus = 10 ; 
  if(SumStatus == 2){

    tft.fillRoundRect(5, 30, 150, 25, 10, TFT_GREEN);
    tft.setCursor(28, 40);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(1.5);
    tft.println("Excellent Quality");

    LED("Green");
    digitalWrite(buzzer, HIGH); 

  }
  else if(SumStatus == 3 || SumStatus == 4){
    tft.fillRoundRect(5, 30, 150, 25, 10, TFT_GREEN);
    tft.setCursor(11, 35);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Good Quality");
    LED("Green");
    digitalWrite(buzzer, HIGH);

  }
  else if(SumStatus == 5 || SumStatus == 6){
    tft.fillRoundRect(5, 30, 150, 25, 10, TFT_YELLOW);
    tft.setCursor(35, 40);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(1);
    tft.println("Moderate Quality");
    LED("Yellow");
    digitalWrite(buzzer, HIGH);
  }
  else if(SumStatus == 7 || SumStatus == 8){
    tft.fillRoundRect(5, 30, 150, 25, 10, TFT_RED);
    tft.setCursor(10, 35);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Poor Quality");
    LED("Yellow");
    Sound("Short Beep");
  }
  else if(SumStatus >= 9 ){
    reading0 = digitalRead(buttonPin0);
    reading1 = digitalRead(buttonPin1);
    tft.fillRoundRect(5, 30, 150, 25, 10, TFT_RED); 
    tft.setTextColor(TFT_BLACK); 
    tft.setTextSize(1); 
    tft.setCursor(30, 38); 
    tft.println("Hazardous Quality");
    Sound("Long Beep");
    LED("Alert");
    //myservo.write(360);
    myservo.writeMicroseconds(1400);
    delay(200);
    if(Humen_Status == 1){

    //if(Noti == 0){
        digitalWrite(Relay , HIGH);
        LINE.notify("Hazardous Quality");
        LINE.notify("Humans is inside a car");
        if(reading1 == LOW && reading0 == LOW){
          LINE.notify("The person inside the car is still conscious.");
          delay(1000);
          LINE.notify("The person inside the car is still conscious.");
          
        }
   // }
    //else if(Noti >= 20 && Noti < 40){
        //LINE.notify("Hazardous Quality");
        //LINE.notify("Humans is moving");
        if(reading1 == LOW && reading0 == LOW){
          LINE.notify("The person inside the car is still conscious.");
          delay(1000);
          LINE.notify("The person inside the car is still conscious.");
          
        }
        SMS = SMS + "\n";
        SMS = SMS + "https://www.google.com/maps/search/?api=1&query=";
        SMS = SMS + "13.7563";//Lat;
        SMS = SMS + ",";
        SMS = SMS + "100.5018";
        LINE.notify(SMS);
    //}
        Noti = Noti + 1;
        delay(300);
        digitalWrite(Relay , LOW);
        
    }
    if(CO > 500 && CO2 > 500){
        CO = 5000;
        TotalCO2 = 10000;
        
        tft.fillScreen(TFT_BLACK);
        tft.fillCircle(120, 120 , 100 , TFT_RED);
        tft.setCursor(90, 90);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(10);
        tft.println("!");
        tft.fillScreen(TFT_BLACK);
    }

  }
}