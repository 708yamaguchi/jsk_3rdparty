// Copied from
// https://github.com/m5stack/M5Unit-ENV/blob/eaa1983195c034b98b03918e4dd3521a894f9daa/examples/Unit_ENVIII_M5Core/Unit_ENVIII_M5Core.ino
//
// Required libraries are
// - M5Unit-ENV
//   https://github.com/m5stack/M5Unit-ENV
// - Adafruit_BMP280 version 2.6.3
//   https://github.com/adafruit/Adafruit_BMP280_Library

#define ROSSERIAL_ARDUINO_BLUETOOTH

#include <m5stack_ros.h>
#include <std_msgs/Float32.h>
#include <M5_ENV_III.h>

std_msgs::Float32 tmp_msg;
ros::Publisher tmp_pub("temperature", &tmp_msg);
std_msgs::Float32 hum_msg;
ros::Publisher hum_pub("humidity", &hum_msg);
std_msgs::Float32 pressure_msg;
ros::Publisher pressure_pub("pressure", &pressure_msg);

uint8_t bt_mac[6];
char device_name[128];

void publishENV() {
  tmp_msg.data = tmp;
  hum_msg.data = hum;
  pressure_msg.data = pressure;
  tmp_pub.publish(&tmp_msg);
  hum_pub.publish(&hum_msg);
  pressure_pub.publish(&pressure_msg);
}

void setup() {
  esp_read_mac(bt_mac, ESP_MAC_BT);
  snprintf(device_name,
           127,
           "ENVIII %02X %02X %02X %02X %02X %02X",
           bt_mac[0],
           bt_mac[1],
           bt_mac[2],
           bt_mac[3],
           bt_mac[4],
           bt_mac[5]);
  setupM5stackROS(device_name);
  setupENV(device_name);

  nh.advertise(tmp_pub);
  nh.advertise(hum_pub);
  nh.advertise(pressure_pub);
}

void loop() {
  measureENV();
  publishENV();
  nh.spinOnce();
  delay(10);
}
