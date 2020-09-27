
     #include <Wire.h>
    #include <ADXL345.h>
    #include "Wire.h" 
    #include <string.h>
    #include <ESP8266WiFi.h>
    #include <ESPAsyncWebServer.h>

    ADXL345 adxl;
    void ICACHE_RAM_ATTR Interrupt_enc1();
    void ICACHE_RAM_ATTR Interrupt_enc2();
    
    const char* ssid     = "CAR_SHB";
    const char* password = "123456789";
    // Use WiFiClient class to create TCP connections

    AsyncWebServer server(80);
    AsyncWebSocket ws("/ws");
     
    
    int AcX,AcY,AcZ;
    const int encoderIn1 = 14;
    const int encoderIn2 = 12;
    int countEncoder1=0;
    int countEncoder2=0;

    String send_data;
    

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_DATA){
//    Serial.println(send_data);
    client->text(send_data);
  }
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
//
      attachInterrupt(digitalPinToInterrupt(encoderIn1), Interrupt_enc1, RISING);
      attachInterrupt(digitalPinToInterrupt(encoderIn2), Interrupt_enc2, RISING);
  
      Serial.begin(115200);
WiFi.softAP(ssid, password);
 
 
  Serial.println(WiFi.softAPIP());
 
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

      server.begin();
      
    }
     

    
    void loop() {
        adxl.readXYZ(&AcX, &AcY, &AcZ);
        send_data = String(AcX) + "," + String(AcY) + "," + String(AcZ) + "," + String(countEncoder1) + "," + String(countEncoder2);
        delay(50);
      }
 

    void Interrupt_enc1() { 
       countEncoder1++;
    }
    void Interrupt_enc2() { 
       countEncoder2++;
    }
