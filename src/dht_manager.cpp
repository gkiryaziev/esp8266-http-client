#include "dht_manager.h"

DHTManager::DHTManager(int pin, int type) : _dht(pin, type) {
  _dht.begin();
}

// get data and store to struct
bool DHTManager::getData(DHTData &data) {

  // get data
  float temperature = _dht.readTemperature(false, false);
  float humidity = _dht.readHumidity(false);

  // check data
  if (isnan(temperature) || isnan(humidity)) {
    return false;
  }

  // compute heat index
  float heatIndex = _dht.computeHeatIndex(temperature, humidity, false);

  // compute dew point
  float dewPoint = dewPointFast(temperature, humidity);

  // save last data
  data.temperature = temperature;
  data.humidity = humidity;
  data.heatIndex = heatIndex;
  data.dewPoint = dewPoint;

  return true;
}

// compute dew point
float DHTManager::dewPointFast(float temperature, float humidity) {
  // 0C<T<+60C (+/-0,4C)
  float b = 17.27;
  float c = 237.7;
  float y = (b * temperature) / (c + temperature) + log(humidity * 0.01);
  return (c * y) / (b - y);
}
