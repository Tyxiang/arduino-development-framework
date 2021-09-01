#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoJson.h>
#include <ESPDateTime.h>
#include <Relay.h>

bool debug = false;
int build_in_led = 2;

//system
String id = "0003";
String name = "equipment-room";
String service = "http://sm.fimik.com/";
String version = "2020-5-2";

String hostname = name + "-" + id;

DynamicJsonDocument doc(1024);
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

unsigned long last_time_1 = 0;
unsigned long last_time_2 = 0;

Relay pump(13);

int water_leak_sensor_pin = A0; 
int low_water_level_sensor_pin = 4; 
int high_water_level_sensor_pin = 0; 

//sensor_value_cache
bool water_leak = false;
bool low_water_level = false;
bool high_water_level = false;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("device start >>> >>>");
  if (!debug) pinMode(build_in_led, OUTPUT);
  WiFi.begin();
  httpUpdater.setup(&server);
  WiFi.hostname(hostname);
  local_management_on();
  server.begin();
  pump.begin();
  pinMode(low_water_level_sensor_pin, INPUT); 
  pinMode(high_water_level_sensor_pin, INPUT); 
  // 1) 每次启动默认开启AP；
  // 2) 同时解决模块第一次启动 hostname 不会被改的问题；
  WiFi.softAP(hostname);
  Serial.print("ap on, ip ");
  Serial.print(WiFi.softAPIP().toString());
  Serial.println(".");
}

void loop() {
  server.handleClient();
  //timer_1
  int interval_1 = millis() - last_time_1;
  if (interval_1 < 0) last_time_1 = 0;
  if (last_time_1 == 0 || interval_1 >= 1000 * 1) {
    last_time_1 = millis();
    //Serial.println("---------- timer 1s start ----------");
    if (WiFi.status()!= WL_CONNECTED) {
      if (WiFi.getMode()!= WIFI_AP_STA) {
        if (!debug) digitalWrite(build_in_led, HIGH); //HIGH是不亮
        WiFi.softAP(hostname);
        Serial.print("ap on, ip ");
        Serial.print(WiFi.softAPIP().toString());
        Serial.println(".");
      }
    }
    if (WiFi.status()== WL_CONNECTED) {
      if (WiFi.getMode()== WIFI_AP_STA) {
        if (!debug) digitalWrite(build_in_led, LOW); //LOW是亮
        WiFi.softAPdisconnect(true);
        Serial.print("ap off, ip ");
        Serial.print(WiFi.localIP().toString());
        Serial.println(".");
      }
      if (!DateTime.isTimeValid()) ntp_to_time();
    }
    //Serial.println("--------------- end ----------------");
  }
  //timer_2
  int interval_2 = millis() - last_time_2;
  if (interval_2 < 0) last_time_2 = 0;
  if (last_time_2 == 0 || interval_2 >= 1000 * 60) {
    last_time_2 = millis();    
    Serial.println("---------- timer 60s start ---------");
    sensor_to_sensor_value_cache();
    sensor_value_cache_to_controller();
    if (WiFi.status() == WL_CONNECTED) {
      ntp_to_time();
      all_to_service();
      sensor_value_cache_to_service();
      service_to_controller();
    }
    Serial.println("--------------- end ----------------");
  }
}
