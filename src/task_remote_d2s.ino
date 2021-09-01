void all_to_service(){
  Serial.print("all_to_service ... ");
  doc.clear();
  //time to doc
  doc["time"] = DateTime.toString();
  doc["boot-time"] = DateFormatter::format("%Y/%m/%d %H:%M:%S", DateTime.getBootTime());
  //system to doc
  doc["id"] = id;
  doc["name"] = name;
  doc["service"] = service;
  doc["version"] = version;
  //wifi to doc
  doc["wifi-mode"] = mode_to_string(WiFi.getMode());
  doc["wifi-station"]["status"] = status_to_string(WiFi.status());
  doc["wifi-station"]["ssid"] = WiFi.SSID();
  doc["wifi-station"]["ip"] = WiFi.localIP().toString();
  doc["wifi-station"]["mask"] = WiFi.subnetMask().toString();
  doc["wifi-station"]["gateway"] = WiFi.gatewayIP().toString();
  doc["wifi-station"]["dns1"] = WiFi.dnsIP().toString();
  doc["wifi-station"]["dns2"] = WiFi.dnsIP(1).toString();
  doc["wifi-station"]["rssi"] = rssi_to_per(WiFi.RSSI());
  doc["wifi-station"]["hostname"] = WiFi.hostname();
  doc["wifi-ap"]["ip"] = WiFi.softAPIP().toString();
  doc["wifi-ap"]["connected"] = WiFi.softAPgetStationNum();
  //sensor to doc
  doc["sensor"]["water-leak"] = water_leak;
  doc["sensor"]["low-water-level"] = low_water_level;
  doc["sensor"]["high-water-level"] = high_water_level; 
  //controller to doc
  doc["controller"]["pump"]["pole"] = pump.pole;
  doc["controller"]["pump"]["locked"] = pump.locked;
  //doc to service
  HTTPClient http;
  String url = service + id + "/d2s/";
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  String data;
  //serializeJson(doc, data);
  serializeJsonPretty(doc, data);
  int httpCode = http.POST(data);
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("ok.");
  } else {
    Serial.println(httpCode);
  }
  http.end();
  Serial.println(data);
}

void sensor_value_cache_to_service() {
  Serial.print("sensor_value_cache_to_service ... ");
  HTTPClient http;
  String url = service + id + "/d2s/water_leak/";
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(String(water_leak));
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("ok.");
  } else {
    Serial.println(httpCode);
  }
}
