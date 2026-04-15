

#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_NeoPixel.h>
#include <mmwave_for_xiao.h>
#include <SoftwareSerial.h>

Adafruit_NeoPixel leds(2, 9, NEO_GRB + NEO_KHZ800);

//Default variables for the sensor from mmwave_for_xiao
#define BUFFER_SIZE 256     // Serial Buffer Size

enum class TargetStatus : byte
        {
            NoTarget = 0x00,     // No target
            MovingTarget = 0x01, // Moving target
            StaticTarget = 0x02, // Static target
            BothTargets = 0x03,  // It can be interpreted as motion, meaning that both the set motion and stationary thresholds are above the set value
            ErrorFrame = 0x04    // Failed to get status
        };


// Setup of sensor and COMs with it
SoftwareSerial COMSerial(2, 3);

#define ShowSerial Serial

Seeed_HSP24 xiao_config(COMSerial, ShowSerial);

Seeed_HSP24::RadarStatus radarStatus;

// Address of the indicator in format 11:22:33:44:55:66
uint8_t broadcastAddress[] = {0xA4, 0xCB, 0x8F, 0x1D, 0x52, 0xB8};
#define CHANNEL 0

float Indicator_current;

typedef struct struck_message {
  float set;
  float get;
} struck_message;

struck_message Indicator_data;
struck_message Sensor_data;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println("Delivering");
}

// What happends when data is recieved
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&Indicator_data, incomingData, sizeof(Indicator_data));
  Indicator_current = Indicator_data.get;
  Serial.println("Data recv: ");
  Serial.println(Indicator_data.get);
  leds.fill(Indicator_data.get);
  leds.show();
}

void setup() {
  //Initialize serial communication
  Serial.begin(115200);
  COMSerial.begin(9600);
  //Initialize esp now
  WiFi.mode(WIFI_STA);
  esp_now_init();
  peerInfo.channel = CHANNEL;
  peerInfo.encrypt = false;
  // add Indicator to esp now
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  // add peer to esp now
  esp_now_add_peer(&peerInfo);
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  //Initialize leds
  leds.begin();
  leds.fill(leds.Color(255, 255, 0), 0, 2);
  leds.show();
  //sensor stuff
  xiao_config.disableEngineeringModel();
}

void loop() {
  //Sensing stuff
  int retryCount = 0;
  const int MAX_RETRIES = 10;  // Maximum number of retries to prevent infinite loops

  //Get radar status
  do {
    radarStatus = xiao_config.getStatus();
    retryCount++;
  } while (radarStatus.targetStatus == Seeed_HSP24::TargetStatus::ErrorFrame && retryCount < MAX_RETRIES);

  // Set color red if sensor is sensing anyone present
  if (radarStatus.targetStatus == Seeed_HSP24::TargetStatus::BothTargets) {
    Sensor_data.set = leds.Color(255, 0, 0);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
    delay(1200);
  }
  else if (radarStatus.targetStatus == Seeed_HSP24::TargetStatus::StaticTarget) {
    Sensor_data.set = leds.Color(255, 0, 0);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
    delay(1200);
  }
  else if (radarStatus.targetStatus == Seeed_HSP24::TargetStatus::BothTargets) {
    Sensor_data.set = leds.Color(255, 0, 0);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
    delay(1200);
  }
  // Set color green if sensor is not sensing anyone present
  else if (radarStatus.targetStatus == Seeed_HSP24::TargetStatus::NoTarget) {
    Sensor_data.set = leds.Color(0, 255, 0);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
    delay(1200);
  }

}
