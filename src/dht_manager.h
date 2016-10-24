#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <DHT.h>

class DHTManager {
private:
  DHT _dht;
  float dewPointFast(float temperature, float humidity); // temperature in celsius
public:
  struct DHTData {
    float temperature;
    float humidity;
    float heatIndex;
    float dewPoint;
  };
  DHTManager(int pin, int type);
  bool getData(DHTData &data);
};

#endif
