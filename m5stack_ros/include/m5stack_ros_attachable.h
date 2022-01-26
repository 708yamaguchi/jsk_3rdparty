#include "m5stack_ros.h"
#include <IP5306.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt16.h>

std_msgs::UInt16 level_msg;
ros::Publisher level_pub("battery_level", &level_msg);
std_msgs::Bool charging_msg;
ros::Publisher charging_pub("is_charging", &charging_msg);

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

// This function is additional setup process for m5stack_ros.
// this should be called after setup()
void afterSetup() {
  setupBatteryPublisher();
  setupSleepSubscriber();
}

// This function is additional loop process for m5stack_ros.
// This function should be called after every loop()
void beforeLoop() {
  // Do not enter main loop when is_sleeping is true
  while(is_sleeping){
    nh.spinOnce();
    publishBattery();
    delay(1000);
  }

  publishBattery();
}
