#include <m5stack_ros.h>
#include <IP5306.h>
#include "PDM_SPM1423.h"
#include <audio_common_msgs/AudioData.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/UInt16.h>

int loop_count = 0;

audio_common_msgs::AudioData audio_msg;
ros::Publisher audio_pub("audio", &audio_msg);
std_msgs::Float32 volume_msg;
ros::Publisher volume_pub("volume", &volume_msg);
std_msgs::UInt16 level_msg;
ros::Publisher level_pub("battery_level", &level_msg);
std_msgs::Bool charging_msg;
ros::Publisher charging_pub("is_charging", &charging_msg);

void pubBattery() {
  // Enable I2C
  measureIP5306();
  level_msg.data = battery_level;
  charging_msg.data = isCharging;
  level_pub.publish(&level_msg);
  charging_pub.publish(&charging_msg);
}

void pubAudio() {
  readMic();
  calcVolume();
  audio_msg.data = microRawData;
  audio_msg.data_length = bytesread;
  audio_pub.publish(&audio_msg);
  volume_msg.data = volume;
  volume_pub.publish(&volume_msg);
  // Draw volume on Lcd
  drawVolume(volume);
}

void setup() {
  setupM5stackROS();
  setupIP5306();
  microPhoneSetup();
  nh.advertise(audio_pub);
  nh.advertise(volume_pub);
  nh.advertise(level_pub);
  nh.advertise(charging_pub);
  header("PDM Unit", BLACK);
}

void loop() {
  if (loop_count == 1000) {
    // Publish battery info while stopping publishing audio info
    // This is because I2C and I2S share the pin and they can't be measured simultaneously
    i2s_stop(I2S_NUM_0);    
    Wire.begin();
    pubBattery();
    Wire.endTransmission(true);
    i2s_driver_uninstall(I2S_NUM_0);
    InitI2SSpakerOrMic(MODE_MIC);
    loop_count = 0;
  }
  else {
    calcVolume();
    pubAudio();
  }
  nh.spinOnce();
  loop_count++;
  delay(30);
}
