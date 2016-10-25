
#include "bmp_manager.h"

BMPManager::BMPManager(int sda, int scl, uint8_t addr) {
  _addr = addr;
  Wire.begin(sda, scl);
  delay(200);
}

bool BMPManager::begin() {
  return _bmp.begin(_addr);
}

void *BMPManager::getData(BMPData &data, float seaLevelhPa) {

  // get data
  float temperature = _bmp.readTemperature();
  float pressurePa = _bmp.readPressure();
  float pressureMmHg = pressurePa / 133.322;
  float altitude = _bmp.readAltitude(seaLevelhPa);

  // save data
  data.temperature = temperature;
  data.pressurePa = pressurePa;
  data.pressureMmHg = pressureMmHg;
  data.altitude = altitude;
}
