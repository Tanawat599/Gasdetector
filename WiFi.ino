void ATC_Setup(){
  WiFi.mode(WIFI_STA);
  wm.addParameter(&custom_mqtt_server);
  wm.setConfigPortalBlocking(false);
  wm.setSaveParamsCallback(saveParamsCallback);
  if(wm.autoConnect("Gas Detector")){
        Serial.println("Connected to WiFi");
        WIFI_Status = 1;
  }
  else {
        Serial.println("Config portal running");
        WIFI_Status = 0;
  }
  Preferences preferences;
  preferences.begin("my-app", false); // Begin preferences with namespace "my-app"
  LINE_TOKEN = preferences.getString("line_token", ""); // Load LINE token from preferences
  preferences.end();
}

void ATC(){
  wm.process();

  if(Token_Status == 1){
      Serial.println("Reset Token");
      LINE.setToken(LINE_TOKEN);
      LINE.notify("Reset Token");
      delay(5000); 
    }
  else {
      //Serial.println("no LINE notification");
      //delay(100); // Delay for 2 seconds
    }
  Token_Status = 0;
}

void saveParamsCallback () {
    LINE_TOKEN = custom_mqtt_server.getValue(); // Get the LINE token from WiFiManager parameter

    if (LINE_TOKEN.length() > 0) {
        Serial.println("LINE token is set");
        Token_Status = 1;
        
        Preferences preferences;
        preferences.begin("my-app", false);
        preferences.putString("line_token", LINE_TOKEN); // Save LINE token to preferences
        preferences.end();
    } else {
        Serial.println("LINE token is not set");
        Token_Status = 0;
    }
}

void WiFi_Connect(){
  String SSID = "jet";
  String PASSWORD = "0959202399";
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  if (WiFi.status() != WL_CONNECTED) {
    WIFI_Status = 0;
    Serial.println("WiFi connection failed.");
  } else {
    WIFI_Status = 1;
    Serial.println("WiFi connected!");

  }
  
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

}


/*
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
void WiFi_Setup(){

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if ( ! deserializeError ) {
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
#endif
          Serial.println("\nparsed json");
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(api_token, json["api_token"]);
        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
  WiFiManagerParameter custom_api_token("apikey", "API token", api_token, 32);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  wifiManager.setSTAStaticIPConfig(IPAddress(10, 0, 1, 99), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));

  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_api_token);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(api_token, custom_api_token.getValue());
  Serial.println("The values in the file are: ");
  Serial.println("\tmqtt_server : " + String(mqtt_server));
  Serial.println("\tmqtt_port : " + String(mqtt_port));
  Serial.println("\tapi_token : " + String(api_token));

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    DynamicJsonDocument json(1024);
#else
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
#endif
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["api_token"] = api_token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
#else
    json.printTo(Serial);
    json.printTo(configFile);
#endif
    configFile.close(); 
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  //LINE_TOKEN = String(mqtt_server) + String(mqtt_port);
  //LINE.setToken(LINE_TOKEN);
  
 
}
*/