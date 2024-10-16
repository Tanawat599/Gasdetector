void LED(String led){
  
  if(led == "Alert"){
    for(int i = 0; i < 10; i++){
      for (int i = 0; i < 16; i++) {
        leds[i] = CRGB(255, 0, 0);
      }
      FastLED.show();
      delay(300);
      for (int i = 0; i < 16; i++) {
        leds[i] = CRGB(0, 0, 255);
      }
      FastLED.show();
      delay(200);      
    }
  }
  else if(led == "Green"){
  for (int i = 0; i < 16; i++) {
    leds[i] = CRGB(0, 10, 0);
  }
  FastLED.show();
  delay(100);
  }
  else if(led == "Yellow"){
    for (int i = 0; i < 16; i++) {
    leds[i] = CRGB(10, 10, 0);
  }
  FastLED.show();
  delay(100);
  }
  else if(led == "Red"){
  for (int i = 0; i < 16; i++) {
    leds[i] = CRGB(10, 0, 0);
  }
  FastLED.show();
  delay(100);


  }


}
void Sound(String Bz){
  if(Bz == "Setup"){
    digitalWrite(buzzer, LOW);
    delay(50);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
    digitalWrite(buzzer, HIGH);
  }
  else if(Bz == "Long Beep"){
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);

  }
  else if(Bz == "Short Beep"){
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
  }
}
void sendDataToSheet(void)
{
  String url = server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + String(SMS) + "&value2="+String(" Hazardous ") +"&value3=" + String(" ");  
  Serial.println(url);
  //Start to send data to IFTTT
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin(url); //HTTP

  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  // httpCode will be negative on error
  if(httpCode > 0) {

    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

}
void Nano() {
  
  String str = Serial2.readString();

  Serial.print("Received data: ");
  Serial.println(str);
  int receivedData = str.toInt(); 
  int X = 0;
  if (!str.isEmpty()) {
    if (receivedData == 1) {
      
      Serial.println("Received CO Excellent");
      LED("Green");
      digitalWrite(buzzer, LOW);
      //digitalWrite(ledPin, LOW);      
    } else if (receivedData == 2) {
      Serial.println("Received CO Good");
      LED("Green");
      digitalWrite(buzzer, X);
      digitalWrite(ledPin, LOW);      
    } else if (receivedData == 3) {
      Serial.println("Received CO Moderate");
      LED("Yellow");
      digitalWrite(buzzer, X);
      digitalWrite(ledPin, LOW);      
    } else if (receivedData == 4) {
      Serial.println("Received CO Poor");
      LED("Yellow");
      Sound("Short Beep");
      digitalWrite(ledPin, LOW);      
    } else if (receivedData == 5) {
    unsigned long currentTime = millis();
    if (currentTime - previousTaskTime >= taskInterval) {

        previousTaskTime = currentTime;
        digitalWrite(buzzer, HIGH);
        Serial.println("Received CO Hazardous");
        Sound("Long Beep");
        LED("Alert");
     // digitalWrite(ledPin, HIGH);
        LINE.notify("Hazardous");
        LINE.notify(SMS);
      //sendDataToSheet();
    }

    } else {
      Serial.println("Unknown data received");
    }
  } else {
    Serial.println("No data received");
  }


}

void Encoder(){
  const int bufferSize = 64;
char Str[bufferSize];

// Read from Serial2 and store it in Str
String inputString = Serial2.readStringUntil('\n');
inputString.toCharArray(Str, bufferSize);

Serial.println(inputString);

char *token = strtok(Str, "/");
while (token != NULL) {
  // Convert token to float
  float floatValue = atof(token);
  
  Serial.print("String converted to float: ");
  Serial.println(floatValue, 5); 
  
  token = strtok(NULL, "/");
}

}


