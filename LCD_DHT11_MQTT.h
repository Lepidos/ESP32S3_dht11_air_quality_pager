#ifndef LCD_DHT11_MQTT
//  #define DBUG                // Uncomment this for serial print
  #define LCD_DHT11_MQTT
  #define DEF_INTER_TIME  60000000
  #define BUZZ_PIN        17
  #define NET_ACCESS      (String[][2]){{"wifi_ssid1", "wifi_password1"}, {"wifi_ssid2", "wifi_password2"}}

  #define NSENSOR         2                			 // No. Analog sensors attached to ESP
  #define SPIN            (short unsigned int[]){6, 13}           // Analog sensors Pin No.
  #define SNAME           (String[]){"CO", "NH3"}         	// SENSOR NAME/DESCRIPTION

  #define MQTTADDR        (const char *[]){"192.168.1.73"}    // MQTT broker IP address
  #define MQTTUSERNAME    (char*)"mqtt_username"          // MQTT authentification username
  #define MQTTPASSWORD    (char*)"mqtt_password"       // MQTT authentification passord
  #define MQTT_PAYLOAD_S  512
  #define TOPIC_PREFIX    (String) "/"
  #define MQTT_DISCON     (const char *) "/disconnect/"


#endif //LCD_time
