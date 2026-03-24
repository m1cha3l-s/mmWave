#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_NeoPixel.h>
#include <mmwave_for_xiao.h>
#include <SoftwareSerial.h>

Adafruit_NeoPixel sensor(2, 9, NEO_GRB + NEO_KHZ800);

uint8_t broadcastAddress[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
