void ntp_to_time() {
  Serial.print("ntp_to_time ... ");
  //DateTime.setServer("asia.pool.ntp.org");
  DateTime.setTimeZone(8);
  //DateTime.begin(15 * 1000);
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("failed!");
  }else{
    Serial.println("ok.");
  }
}
void service_to_controller(){
  Serial.print("service_to_controller ... ");
  HTTPClient http;
  String url = service + id + "/s2d/pump/";
  http.begin(url);
  int httpCode = http.GET();
  String payload;
  if (httpCode == HTTP_CODE_OK) {
    payload = http.getString();
    if(payload == "true"){
      pump.on();
    }
    if(payload == "false"){
      pump.off();
    }
    Serial.println("ok.");
  }else{
    Serial.println(httpCode);
  }
  http.end();
}
