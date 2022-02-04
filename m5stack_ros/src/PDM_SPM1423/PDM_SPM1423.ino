#include <m5stack_ros.h>
#include <IP5306.h>
#include "PDM_SPM1423.h"
#include <audio_common_msgs/AudioData.h>
#include <std_msgs/Bool.h>
#include <std_msgs/UInt16.h>

audio_common_msgs::AudioData audio_msg;
ros::Publisher audio_pub("audio", &audio_msg);


std_msgs::UInt16 level_msg;
ros::Publisher level_pub("battery_level", &level_msg);
std_msgs::Bool charging_msg;
ros::Publisher charging_pub("is_charging", &charging_msg);

// xTaskHandle xTaskBattery;

void setup() {
  setupM5stackROS();

  setupIP5306();

  // xTaskCreatePinnedToCore(pubBattery, "batteryTask", 4096, NULL, 3, &xTaskBattery, 1);
  // vTaskSuspend(xTaskBattery);
  
  microPhoneSetup();
  nh.advertise(audio_pub);
  nh.advertise(level_pub);
  nh.advertise(charging_pub);


}


void pubBattery() {
  // Enable I2C
  measureIP5306();
  level_msg.data = battery_level;
  charging_msg.data = isCharging;
  level_pub.publish(&level_msg);
  charging_pub.publish(&charging_msg);
}

int loop_count = 0;

void loop() {

  // TODO: publish battery_state and publish audio using vTaskSuspend and vTaskResume
  // TODO: publish /volume topic

  if (loop_count == 100) {
    // Publish battery info while stopping publishing audio info
    // This is because I2C and I2S share the pin and they can't be measured simultaneously 
    vTaskSuspend(xTaskMic);
    Serial.println("suspend");
    // Temporary  change pin assign for I2C
    // i2s_stop(I2S_NUM_0);

    i2s_driver_uninstall(I2S_NUM_0);
    InitI2SSpakerOrMicDummy(MODE_MIC);
    
    // Wire.begin(22, 21);
    // Wire.begin(21, 22);
    Serial.println("before measurement");
    // pinMode(21, INPUT_PULLUP);
    // pinMode(22, INPUT_PULLUP);
    // Wire.begin(22, 21);
    Wire.begin();
    pubBattery();
    Serial.print("battery_level: ");
    Serial.println(battery_level);
    Serial.print("isCharging: ");
    Serial.println(isCharging);
    Serial.println("after measurement");
    Wire.setPins(32, 33);
    Wire.begin(32, 33); // Set dummy I2C for I2S
    Wire.endTransmission(true);

    // i2s_start(I2S_NUM_0);
    InitI2SSpakerOrMic(MODE_MIC);

    /*
    // TODO: create pin_config function
    // Reset I2S pin config correctly
    i2s_pin_config_t pin_config;
    pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num    = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num  = PIN_DATA;
    i2s_set_pin(I2S_NUM_0, &pin_config);
    */

    vTaskResume(xTaskMic);
    delay(30); // wait for new audio data
    Serial.println("resume");
    loop_count = 0;
  }
  else {
    audio_msg.data = microRawData;
    audio_msg.data_length = bytesread;
    audio_pub.publish(&audio_msg);
  }

  // TODO: set appropriate hz
  // Check hz of readMic() function.
  delay(30);
  loop_count++;
  nh.spinOnce();
}
