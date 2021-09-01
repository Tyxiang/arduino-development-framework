void local_management_on(){
  Serial.println("local_management_on.");
  server.on("/",                  all_to_web);
  server.on("/web/",              all_to_web);
  server.on("/web/wifi/edit/",    wifi_to_web);
  server.on("/web/wifi/save/",    web_to_wifi);
  server.on("/web/controller/edit/",  controller_to_web);
  server.on("/web/controller/save/",  web_to_controller);
  server.onNotFound(handle_not_found);
}

void all_to_web() {
  Serial.print("all_to_web ... ");
  const String title = hostname;
  const String h1 = hostname;
  String main = "";
    main += "<h2>system</h2>";
    main += "id: " + id + "</br>";
    main += "name: " + name + "</br>";
    main += "service: " + service + "</br>";
    main += "wifi-mode: " + mode_to_string(WiFi.getMode()) + "</br>";
    main += "version: " + version + "</br>";
    main += "time: " + DateTime.toString() + "</br>";
    main += "boot-time: " + DateFormatter::format("%Y/%m/%d %H:%M:%S", DateTime.getBootTime()) + "</br>";
    main += "</br>";
    main += "<a href='/update'>Update</a>";
    main += "<h2>wifi-station</h2>";
    main += "status: " + status_to_string(WiFi.status()) + "</br>";
    main += "ssid: " + WiFi.SSID() + "</br>";
    main += "ip: " + WiFi.localIP().toString() + "</br>";
    main += "mask: " + WiFi.subnetMask().toString() + "</br>";
    main += "gateway: " + WiFi.gatewayIP().toString() + "</br>";
    main += "dns1: " + WiFi.dnsIP().toString() + "</br>";
    main += "dns2: " + WiFi.dnsIP(1).toString() + "</br>";
    main += "rssi: " + rssi_to_per(WiFi.RSSI()) + "</br>";
    main += "hostname: " + WiFi.hostname() + "</br>";
    main += "</br>";
    main += "<a href='/web/wifi/edit/'>Edit</a>";
    main += "<h2>wifi-ap</h2>";
    main += "ip: " + WiFi.softAPIP().toString() + "</br>";
    main += "connected: " + String(WiFi.softAPgetStationNum()) + "</br>";
    main += "<h2>sensor</h2>";
    main += "water leak: " + String(water_leak) + "</br>";
    main += "low water level: " + String(low_water_level) + "</br>";
    main += "high water level: " + String(high_water_level) + "</br>";
    main += "<h2>controller</h2>";
    main += "pump: " + String(pump.pole) + "</br>";
    main += "pump luck: " + String(pump.locked) + "</br>";
    main += "</br>";
    main += "<a href='/web/controller/edit/'>Edit</a>";
  String html = make_html(title, h1, main, "", "", "");
  server.send(200, "text/html", html);
  Serial.println("ok.");
}

void wifi_to_web() {
  Serial.print("wifi_to_web ... ");
  const String title = hostname;
  const String h1 = "Wifi Edit";
  String main = "";
    main +=   "<form method='post' action='/web/wifi/save/'>";
    main +=     "<p><b>ssid:</b></br>";
    main +=     "<input type='text' name='ssid'></p>";
    main +=     "<p><b>password:</b></br>";
    main +=     "<input type='password' name='password'></p>";
    main +=     "<input type='submit' value='Submit'>";
    main +=   "</form>";
  String html = make_html(title, h1, main, "", "", "");
  server.send(200, "text/html", html);
  Serial.println("ok.");
}
void web_to_wifi() {
  Serial.print("web_to_wifi ... ");
  if (server.method() == HTTP_POST) {
    if(server.argName(0)== "ssid" && server.argName(1)== "password" ){
      WiFi.begin(server.arg(0), server.arg(1));
    }
    //server.send(200, "text/plain", "Change wifi setting ok.");
    server.sendHeader("Location", "/");
    server.send(301);
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
  Serial.println("ok.");
}

void controller_to_web() {
  Serial.print("controller_to_web ... ");
  const String title = hostname;
  const String h1 = "Controller Edit";
  String main = "";
    main +=   "<form method='post' action='/web/controller/save/'>";
    main +=     "<h2>pump</h2>";
    main +=     "<input type='checkbox' name='pump' " + bool_to_checked(pump.pole) + "> on</br>";
    main +=     "<input type='checkbox' name='pump_lock' " + bool_to_checked(pump.locked) + "> lock</br>";
    main +=     "<p><input type='submit' value='Submit'></p>";
    main +=   "</form>";
  String html = make_html(title, h1, main, "", "", "");
  server.send(200, "text/html", html);
  Serial.println("ok.");
}
void web_to_controller() {
  Serial.print("web_to_controller ... ");
  if (server.method() == HTTP_POST) {
    pump.off();
    pump.locked = false;
    for (int i = 0; i < server.args(); i++) {
      if(server.argName(i)== "pump"){
        pump.on();
      }
      if(server.argName(i)== "pump_lock" ){
        pump.locked = true;
      }
    }
    //server.send(200, "text/plain", "Change controller setting ok.");
    server.sendHeader("Location", "/");
    server.send(301);
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
  Serial.println("ok.");
}

void handle_not_found() {
  Serial.print("handle_not_found ... ");
  server.send(404, "text/plain", "File Not Found");
  Serial.println("ok.");
}

String make_html(String title, String h1, String main, String css, String js, String meta){
  String html = "";
    html += "<html>";
    html += "<head>";
    html +=   "<meta charset='utf-8' />";
    html +=   "<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no' />";
    html +=   meta;
    html +=   "<style type='text/css'>h3{color:green;}</style>"; 
    html +=   css;
    html +=   js; 
    html +=   "<title>" + title + "</title>";
    html += "</head>";
    html += "<body>";
    html +=   "<h1>" + h1 + "</h1>"; 
    html +=   main;
    html += "</body>";
    html += "</html>";
  return html;
}
