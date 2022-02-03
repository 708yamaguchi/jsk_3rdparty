#include <m5stack_ros.h>
#include "PDM_SPM1423.h"
#include <audio_common_msgs/AudioData.h>

audio_common_msgs::AudioData audio_msg;
ros::Publisher audio_pub("audio", &audio_msg);

void setup() {
  setupM5stackROS();
  microPhoneSetup();
  nh.advertise(audio_pub);
}

void loop() {
  audio_msg.data = microRawData;
  audio_msg.data_length = bytesread;
  audio_pub.publish(&audio_msg);
  nh.spinOnce();
  // TODO: set appropriate hz
  delay(30);
}
