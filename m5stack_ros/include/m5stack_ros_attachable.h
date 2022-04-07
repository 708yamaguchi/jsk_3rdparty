#include "m5stack_ros.h"
#include <IP5306.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>

std_msgs::UInt16 level_msg;
ros::Publisher level_pub("battery_level", &level_msg);
std_msgs::Bool charging_msg;
ros::Publisher charging_pub("is_charging", &charging_msg);

char sensor_type[30] = "null";
std_msgs::String sensor_type_msg;
ros::Publisher sensor_type_pub("sensor_type", &sensor_type_msg);
char attach_type[30] = "null";
std_msgs::String attach_type_msg;
ros::Publisher attach_type_pub("attach_type", &attach_type_msg);

bool is_sleeping = false;
void stopCb( const std_msgs::Empty& stop_msg ){ is_sleeping = true; };
void startCb( const std_msgs::Empty& start_msg ){ is_sleeping = false; };
ros::Subscriber<std_msgs::Empty> stop_sub("stop", &stopCb);
ros::Subscriber<std_msgs::Empty> start_sub("start", &startCb);

void setupBatteryPublisher() {
  setupIP5306();
  nh.advertise(level_pub);
  nh.advertise(charging_pub);
}

void setupModuleInfo() {
  nh.advertise(sensor_type_pub);
  nh.advertise(attach_type_pub);
}

void setupSleepSubscriber() {
  nh.subscribe(stop_sub);
  nh.subscribe(start_sub);
}

void publishBattery() {
  measureIP5306();
  level_msg.data = battery_level;
  charging_msg.data = isCharging;
  level_pub.publish(&level_msg);
  charging_pub.publish(&charging_msg);
  nh.spinOnce();
}

void publishModuleInfo() {
  sensor_type_msg.data = sensor_type;
  sensor_type_pub.publish(&sensor_type_msg);
  attach_type_msg.data = attach_type;
  attach_type_pub.publish(&attach_type_msg);
  nh.spinOnce();
}

void blinkCharging() {
  // 背景を真っ黒に塗りつぶす
  M5.Lcd.fillScreen(BLACK);
  // 電池へそ
  M5.Lcd.fillRoundRect(280,45,25,110,5,WHITE);
  // 大外枠
  M5.Lcd.fillRoundRect(40,20,240,160,5,WHITE);
  // 中枠
  int battery_bar = (int) (220 * battery_level / 100.0);
  M5.Lcd.fillRoundRect(50,30,battery_bar,140,5,GREEN);
  // 充電マーク
  M5.Lcd.fillTriangle(160, 65, 140, 110, 160, 110, BLACK);
  M5.Lcd.fillTriangle(160, 135, 180, 90, 160, 90, BLACK);
  // 点滅
  delay(3000);
  M5.Lcd.sleep();
  M5.Lcd.setBrightness(0);
  delay(3000);
  M5.Lcd.wakeup();
  M5.Lcd.setBrightness(255);
}

// This function is additional setup process for m5stack_ros.
// this should be called after setup()
void afterSetup() {
  setupBatteryPublisher();
  setupModuleInfo();
  setupSleepSubscriber();
}

// This function is additional loop process for m5stack_ros.
// This function should be called after every loop()
void beforeLoop() {
  // Do not enter main loop when is_sleeping is true
  while (is_sleeping || isCharging) {
    if (is_sleeping) {
      delay(1000);
    }
    if (isCharging) {
      blinkCharging();
    }
    nh.spinOnce();
    publishBattery();
    publishModuleInfo();
  }
  publishBattery();
  publishModuleInfo();
}
