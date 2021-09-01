String status_to_string(int status) {
  String r = "I don't know.";
  if (status == WL_CONNECTED) r = "connected";
  if (status == WL_NO_SHIELD) r = "no shield";
  if (status == WL_IDLE_STATUS) r = "changing"; //"changing between statuses"
  if (status == WL_NO_SSID_AVAIL) r = "no ssid available";
  if (status == WL_SCAN_COMPLETED) r = "scaning completed";
  if (status == WL_CONNECT_FAILED) r = "password incorrect connection failed";
  if (status == WL_CONNECTION_LOST) r = "connection lost";
  if (status == WL_DISCONNECTED) r = "disconnected";
  return r;
}

String mode_to_string(int mode) {
  String r = "i don't know.";
  if (mode == WIFI_AP) r = "ap";
  if (mode == WIFI_STA) r = "station";
  if (mode == WIFI_AP_STA) r = "ap and station";
  if (mode == WIFI_OFF) r = "off";
  return r;
}

String rssi_to_per(int rssi){
  int r;
  r = 130 + rssi;
  return String(r);
}

String bool_to_string(bool b) {
  if (b == true) return "true";
  if (b == false) return "false";
}

String bool_to_checked(bool b) {
  if (b == true) return "checked";
  if (b == false) return "";
}
