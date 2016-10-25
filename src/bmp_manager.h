
#ifndef BMP_MANAGER_H
#define BMP_MANAGER_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

class BMPManager {
private:
  Adafruit_BMP280 _bmp;
  uint8_t _addr;
public:
  struct BMPData {
    float temperature;
    float pressurePa;
    float pressureMmHg;
    float altitude;
  };
  BMPManager(int sda, int scl, uint8_t addr);
  bool begin();
  void *getData(BMPData &data, float seaLevelhPa = 1013.25);
};

#endif
