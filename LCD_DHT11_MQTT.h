#ifndef LCD_DHT11_MQTT
  #define DBUG            1    // Uncomment this for serial print  
  #define LCD_DHT11_MQTT
  #define DEF_INTER_TIME  60000000
  #define BUZZ_PIN        17
  #define NET_ACCESS      (String[][2]){{"MEO-6B1EF1", "2E9C44CDF0"}, {"Sala_5G", "carvalho"}}

  #define NSENSOR         2                 // No. sensors attached to ESP
  #define SPIN            (short unsigned int[]){6, 13}           // ESP32 Pin No.
  #define SNAME           (String[]){"CO", "NH3"}         // SENSOR NAME/DESCRIPTION

  #define MQTTADDR        (const char *[]){"192.168.1.73"}    // MQTT broker IP address
  #define MQTTUSERNAME    (char*)"casa"          // MQTT authentification username
  #define MQTTPASSWORD    (char*)"myhome"       // MQTT authentification passord
  #define MQTT_PAYLOAD_S  512
  #define TOPIC_PREFIX    (String) "/"
  #define MQTT_DISCON     (const char *) "/disconnect/"


#endif //LCD_time
