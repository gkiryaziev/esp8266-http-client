/*
	26.09.2016 by Admin :-)
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "http_manager.h"
#include "dht_manager.h"
#include "bmp_manager.h"

//ADC_MODE(ADC_VCC);    // internal ADC

// DHT
#define DHT_PIN       13   // pin D7
#define DHT_TYPE      11   // DHT11
// BMP
#define BMP_SDA       12   // pin D6
#define BMP_SCL       14   // pin D5
#define BMP_ADDR      0x76 // I2C address
// WIFI
#define SSID          "ThomsonAP"
#define PASSWORD      "zxcasdqwe"
// time
#define ONE_SECOND 1000
#define ONE_MINUTE 60000

// intervals
const int amountOfIntervals                      = 1;
unsigned long previousMillis[amountOfIntervals]  = {0};
unsigned long currentMillis                      = 0;
int intervals[amountOfIntervals]                 = {10 * ONE_MINUTE};

// functions
void httpPost();

// network
HTTPManager httpm;

// sensors
DHTManager dhtm(DHT_PIN, DHT_TYPE);
BMPManager bmpm(BMP_SDA, BMP_SCL, BMP_ADDR);

// structures
DHTManager::DHTData dhtData;
BMPManager::BMPData bmpData;

// first setup
void setup() {
	// serial
	Serial.begin(115200);
	delay(50);

	//BMP
	if (!bmpm.begin()) {
		Serial.println("Could not find a valid BMP280 sensor, check wiring!");
		while (1);
	}

	// wifi
	Serial.println("\n--- WiFi ---");
	Serial.print("Connecting to ");
	Serial.print(SSID);

	WiFi.begin(SSID, PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("\nWiFi connected.");
	Serial.print("IP address: ");
	Serial.print(WiFi.localIP());
	Serial.println();

	Serial.println("--- System ---");
	Serial.printf("getChipId: %d\n", ESP.getChipId());
	Serial.printf("getCpuFreqMHz: %d\n", ESP.getCpuFreqMHz());
	Serial.printf("getSdkVersion: %s\n", ESP.getSdkVersion());
	Serial.printf("getBootVersion: %d\n", ESP.getBootVersion());
	Serial.printf("getBootMode: %d\n", ESP.getBootMode());
	Serial.printf("getCycleCount: %u\n", ESP.getCycleCount());
	//Serial.printf("getVcc: %s\n", String(ESP.getVcc() / 1024.0, 2).c_str());
	Serial.println("--- Memory ---");
	Serial.printf("getFreeHeap: %u\n", ESP.getFreeHeap());
	Serial.printf("getSketchSize: %u\n", ESP.getSketchSize());
	Serial.printf("getFreeSketchSpace: %u\n", ESP.getFreeSketchSpace());

	Serial.print("\nFirst POST request ...");
	httpPost();
	Serial.println("done.");
}

// main loop
void loop() {
	// get time and run intervals
	currentMillis = millis();
	for (int i = 0; i < amountOfIntervals; i++) {
		if ((unsigned long)(currentMillis - previousMillis[i]) > intervals[i]) {
			if (i == 0) { httpPost(); }
			// if (i == 1) { other timer }
			previousMillis[i] = currentMillis;
		}
	}
}

// POST request
void httpPost() {

	// dht11
	if (dhtm.getData(dhtData)) {
		String dht11 = "temperature=" + String(dhtData.temperature, 2) +
									 "&humidity=" + String(dhtData.humidity, 2) +
									 "&heat_index=" + String(dhtData.heatIndex, 2) +
									 "&dew_point=" + String(dhtData.dewPoint, 2);
		httpm.POST("http://iot-gkdevmaster.rhcloud.com/api/v1/dht11", dht11);
	} else {
		Serial.println("Failed to read from DHT sensor!");
	}

	// bmp280
	bmpm.getData(bmpData, 1016.69);
	String bmp280 = "temperature=" + String(bmpData.temperature, 2) +
									"&pressure_pa=" + String(bmpData.pressurePa, 2) +
									"&pressure_mmhg=" + String(bmpData.pressureMmHg, 2) +
									"&altitude=" + String(bmpData.altitude, 2);
	httpm.POST("http://iot-gkdevmaster.rhcloud.com/api/v1/bmp280", bmp280);
}
