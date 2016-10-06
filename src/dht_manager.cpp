#include "dht_manager.h"

DHTManager::DHTManager(int pin, int type) : _dht(pin, type) { _dht.begin(); delay(1000); }

bool DHTManager::getData(DHTData &data, bool isExternalTemperature, float externalTemperature) {

  // get data
  float temperature = _dht.readTemperature(false, false);
  float humidity = _dht.readHumidity(false);

  // check data
  if (isnan(temperature) || isnan(humidity)) {
    return false;
  }

  // compute heat index
  float heatIndex = 0;
  if (isExternalTemperature) {
    heatIndex = _dht.computeHeatIndex(externalTemperature, humidity, false);
  } else {
    heatIndex = _dht.computeHeatIndex(temperature, humidity, false);
  }

  // save last data
  data.temperature = temperature;
  data.humidity = humidity;
  data.heatIndex = heatIndex;

  return true;
}
