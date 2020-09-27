
    #include <Wire.h>
    #include <ADXL345.h>
    #include <ESP8266WiFi.h>
    #include "Wire.h" 
    #include <string.h>

    ADXL345 adxl;
    void ICACHE_RAM_ATTR Interrupt_enc1();
    void ICACHE_RAM_ATTR Interrupt_enc2();
    
    const char* ssid     = "cabinet_router";
    const char* password = "123456789";
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 37842;
     
    const char* host = "192.168.43.1";
    String endConnection = "Connection: close\r\n";
    String contentType = "Content-Type: application/json";
    
    int AcX,AcY,AcZ;
    const int encoderIn1 = 14;
    const int encoderIn2 = 15;
    int countEncoder1=0;
    int countEncoder2=0;
    
    uint32_t timer;//unsigned int
    String response;

    String jsonEncoder(uint32_t arg0, int16_t arg1, int16_t arg2, int16_t arg3, uint32_t arg4, uint32_t arg5) {
      return "{\"time\":\"" + String(arg0) + "\", \"AcX\":\"" + String(arg1) + "\", \"AcY\":\"" + String(arg2) +
              "\", \"AcZ\":\"" + String(arg3) + "\", \"Encode1\":\"" + String(arg4) + "\", \"Encode2\":\"" + String(arg5) + "\"}";
    }

    void wifi_init() {
      Serial.print("Connecting to ");
      Serial.println(ssid);
      WiFi.begin(ssid, password);
      
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
     
      Serial.println("");
      Serial.println("WiFi connected");  
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    void setup_adxl(){
      adxl.powerOn();

      //set activity/ inactivity thresholds (0-255)
      adxl.setActivityThreshold(75); //62.5mg per increment
      adxl.setInactivityThreshold(75); //62.5mg per increment
      adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
      //look of activity movement on this axes - 1 == on; 0 == off 
      adxl.setActivityX(1);
      adxl.setActivityY(1);
      adxl.setActivityZ(1);
 
      //look of inactivity movement on this axes - 1 == on; 0 == off
      adxl.setInactivityX(1);
      adxl.setInactivityY(1);
      adxl.setInactivityZ(1);
 
      //look of tap movement on this axes - 1 == on; 0 == off
      adxl.setTapDetectionOnX(0);
      adxl.setTapDetectionOnY(0);
      adxl.setTapDetectionOnZ(1);
 
      //set values for what is a tap, and what is a double tap (0-255)
      adxl.setTapThreshold(50); //62.5mg per increment
      adxl.setTapDuration(15); //625us per increment
      adxl.setDoubleTapLatency(80); //1.25ms per increment
      adxl.setDoubleTapWindow(200); //1.25ms per increment
 
      //set values for what is considered freefall (0-255)
      adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
      adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
      //setting all interrupts to take place on int pin 1
      //I had issues with int pin 2, was unable to reset it
      adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
      adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
      adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
      adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
      adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
      //register interrupt actions - 1 == on; 0 == off  
      adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
      adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
      adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
      adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
      adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
    }
    
    void setup() {
      setup_adxl();
      pinMode(encoderIn1, INPUT_PULLUP);
      pinMode(encoderIn2, INPUT_PULLUP);

  
      Serial.begin(115200);
      delay(100);
     
      // We start by connecting to a WiFi network
     
      Serial.println();
      Serial.println();
      
      wifi_init();
      
      
    }
     

    
    void loop() {
      delay(50); 

      adxl.readXYZ(&AcX, &AcY, &AcZ);
      
      response = "";
      timer = millis();



      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }

      
      client.print(String("POST ") + "/api/toycar" + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             endConnection +
             contentType + "\r\n\r\n" +
             jsonEncoder(timer, AcX, AcY, AcZ, countEncoder1, countEncoder2)
             + "\r\n");

      String line = client.readStringUntil('\r');

      if(line == "") {
          WiFi.disconnect();
          while (WiFi.status() == WL_CONNECTED) {
          delay(500);
          Serial.print(".");
        }
        wifi_init();
      }
      
    }

    void Interrupt_enc1() { 
       countEncoder1++;
    }
    void Interrupt_enc2() { 
       countEncoder2++;
    }
