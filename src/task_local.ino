void sensor_to_sensor_value_cache(){
  Serial.print("sensor_to_sensor_value_cache ... ");
  int water_leak_value = analogRead(water_leak_sensor_pin);
  if (water_leak_value < 1000) {
    water_leak = true;
  }else{
    water_leak = false;
  }
  low_water_level = !digitalRead(low_water_level_sensor_pin);
  high_water_level = !digitalRead(high_water_level_sensor_pin);
  Serial.println("ok.");
}
void sensor_value_cache_to_controller(){
  Serial.print("sensor_value_cache_to_controller ... ");
  //如果比低水位还低就关闭
  if(!low_water_level) pump.off();
  //如果比高水位还高就打开
  if(high_water_level) pump.on();
  Serial.println("ok.");
}
