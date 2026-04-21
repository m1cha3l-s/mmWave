#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_NeoPixel.h>
#include <mmwave_for_xiao.h>
#include <SoftwareSerial.h>

Adafruit_NeoPixel leds(2, 9, NEO_GRB + NEO_KHZ800);

//Default variables for the sensor from mmwave_for_xiao
#define BUFFER_SIZE 256     // Serial Buffer Size

// Setup of sensor and COMs with it
SoftwareSerial COMSerial(D2, D3);

#define ShowSerial Serial

Seeed_HSP24 xiao_config(COMSerial, ShowSerial);

Seeed_HSP24::RadarStatus radarStatus;

// Address of the indicator in format 11:22:33:44:55:66
uint8_t broadcastAddress[] = {0xA4, 0xCB, 0x8F, 0x1D, 0x52, 0xB8};
#define CHANNEL 0

float Indicator_current;
float Sensor_set;

typedef  struct struck_message {
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
  Serial.println("Data recv:");
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
  Serial.println("Starting!");
}

void loop() {
  //Sensing stuff
  int retryCount = 0;
  const int MAX_RETRIES = 10;  // Maximum number of retries to prevent infinite loops
  Sensor_set = Sensor_data.set;
  //Get radar status
  do {
    radarStatus = xiao_config.getStatus();
    retryCount++;
  } while (radarStatus.distance == -1 && retryCount < MAX_RETRIES);
  // Set color red if sensor is sensing anyone present
  if (String(targetStatusToString(radarStatus.targetStatus)) == "BothTargets") {
    Serial.println("Detected: " + String(targetStatusToString(radarStatus.targetStatus)));
    Sensor_data.set = leds.Color(255, 0, 0);
    Serial.println("Sent:");
    Serial.println(Sensor_data.set);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
  }
  else if (String(targetStatusToString(radarStatus.targetStatus)) == "StaticTarget") {
    Serial.println("Detected: " + String(targetStatusToString(radarStatus.targetStatus)));
    Sensor_data.set = leds.Color(255, 0, 0);
    Serial.println("Sent:");
    Serial.println(Sensor_data.set);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
  }
  else if (String(targetStatusToString(radarStatus.targetStatus)) == "BothTargets") {
    Serial.println("Detected: " + String(targetStatusToString(radarStatus.targetStatus)));
    Sensor_data.set = leds.Color(255, 0, 0);
    Serial.println("Sent:");
    Serial.println(Sensor_data.set);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
  }
  // Set color green if sensor is not sensing anyone present
  else if (String(targetStatusToString(radarStatus.targetStatus)) == "NoTarget") {
    Serial.println("Detected: " + String(targetStatusToString(radarStatus.targetStatus)));
    Sensor_data.set = leds.Color(0, 255, 0);
    Serial.println("Sent:");
    Serial.println(Sensor_data.set);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
  }
  else if (String(targetStatusToString(radarStatus.targetStatus)) == "Unknown") {
    Serial.println("Detected: " + String(targetStatusToString(radarStatus.targetStatus)));
    Sensor_data.set = leds.Color(255, 255, 0);
    Serial.println("Sent:");
    Serial.println(Sensor_data.set);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Sensor_data, sizeof(Sensor_data));
  }
delay(250);
Serial.println("Last recv:");
Serial.println(Indicator_data.get);
delay(1200);

}

const char* targetStatusToString(Seeed_HSP24::TargetStatus status) {
  switch (status) {
    case Seeed_HSP24::TargetStatus::NoTarget:
      return "NoTarget";
    case Seeed_HSP24::TargetStatus::MovingTarget:
      return "MovingTarget";
    case Seeed_HSP24::TargetStatus::StaticTarget:
      return "StaticTarget";
    case Seeed_HSP24::TargetStatus::BothTargets:
      return "BothTargets";
    default:
      return "Unknown";
  }
}
