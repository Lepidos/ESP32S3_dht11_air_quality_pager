#include <WiFi.h>
#include <MQTT.h>
#include <time.h>
#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "LCD_DHT11_MQTT.h"
#include <ESP32Time.h>
#include "DHT11.h"

// MQTT
String mqttclientid;                                                 
WiFiClient net_client;                                                 
MQTTClient mqtt_client(MQTT_PAYLOAD_S);
char * mqtt_client_id;

// DHT11
DHT11 dht11(44);
int last_temp = 0;
int last_humd = 0;


// INTERRUPTS ESPRESSIF 2.x.x API
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer();
volatile unsigned int sensorval[NSENSOR] = {0};
volatile unsigned int interruptCounter = 0;
volatile bool iflag = false; 

void setup() {
  #ifdef DBUG
    while(!Serial)
    Serial.begin(9600);
    Serial.println("Start");
  #endif
  pinMode(BUZZ_PIN, OUTPUT);
  Config_Init();
  LCD_Init();
  LCD_SetBacklight(40);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_SetRotate(90);
  LCD_Clear(BLACK);
  delay(1000);
  connect();

  timer = timerBegin(0, 40, true);   
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, DEF_INTER_TIME, true);
  timerAlarmEnable(timer);                     
}
void connect() {                               
  while (WiFi.status() != WL_CONNECTED) {     
    for (int i=0; i<sizeof(NET_ACCESS); i++) {
      if (WiFi.status() == WL_CONNECTED) break;
      Paint_DrawString_EN(20, 50, NET_ACCESS[i][0].c_str(), &Font20, BLACK, RED);
      WiFi.begin(NET_ACCESS[i][0], NET_ACCESS[i][1]);
      delay(2000);                             
    }
  }
  delay(1000);
  mqtt_client_id = new char[18];                 
  mqttclientid = WiFi.macAddress();              
  strcpy(mqtt_client_id, WiFi.macAddress().c_str());

  while (!mqtt_client.connected()) {
    for (int i=0; i<sizeof(MQTTADDR); i++) {
      mqtt_client.begin(MQTTADDR[i], 1883, net_client); 
      if (mqtt_client.connect(mqtt_client_id, MQTTUSERNAME, MQTTPASSWORD))    
        break;
      delay(1200);                                  
    }
  }
  mqtt_client.setOptions(60 , true , 99999);
  mqtt_client.onMessage(messageReceived);
  mqtt_client.subscribe(TOPIC_PREFIX + mqtt_client_id);
  mqtt_client.subscribe("/:::::/#");  
  mqtt_client.subscribe("+/notification/#");  
 }

void messageReceived(String &topic, String &payload) {
  digitalWrite(BUZZ_PIN, HIGH);
  delay(10);
  digitalWrite(BUZZ_PIN, LOW);
  if (strstr(topic.c_str(), MQTT_DISCON) != NULL) {
    delay(payload.toInt());
    connect();
    mqtt_client.setOptions(60 , true , 99999);
    mqtt_client.onMessage(messageReceived);
    mqtt_client.subscribe(TOPIC_PREFIX + mqtt_client_id);
    mqtt_client.subscribe("/:::::/#"); 
    mqtt_client.subscribe("+/notification/#");
    return;
  }
  LCD_Clear(BLACK);
  Paint_DrawString_EN(20, 5, payload.c_str(), &Font20, BLACK, WHITE);
}

void loop() {
  mqtt_client.loop();
  if (WiFi.status() != WL_CONNECTED) {
    connect();               // this activates our event
    mqtt_client.onMessage(messageReceived);
    mqtt_client.subscribe(TOPIC_PREFIX + mqtt_client_id);
    mqtt_client.subscribe("/:::::/#"); 
    mqtt_client.subscribe("+/notification/#");
  }
  if (iflag) {
    #ifdef DBUG
      Serial.println((String) "New interrupt");
    #endif
    iflag=false;
    int temperature = 0;
    int humidity = 0;
    if (! dht11.readTemperatureHumidity(temperature, humidity)) {
      String temp = "Temperature ";
      String humi = " Humidity ";
      String msg = temp + String(temperature) + humi + String(humidity);
      Paint_DrawString_EN(20, 80, msg.c_str(), &Font20, BLACK, RED);
      mqtt_client.publish("/"+mqttclientid+"/Temperature/", String(temperature));
      mqtt_client.publish("/"+mqttclientid+"/Humidity/", String(humidity));
    } else { 
      #ifdef DBUG
        Serial.println((String) "DHT11 Error");
      #endif
    }
    for ( int i=0; i < NSENSOR; i++) {                      
       sensorval[i] = analogRead(SPIN[i]);                

       int nlength = SNAME[i].length();                   
       mqtt_client.publish("/"+mqttclientid+"/"+ SNAME[i] + "/",(String) sensorval[i]);

    }
  }
} 


void IRAM_ATTR onTimer() {   
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;               
  portEXIT_CRITICAL_ISR(&timerMux); 
  iflag = true;                     
}

