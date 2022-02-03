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

int loop_count = 0;

void loop() {
  audio_msg.data = microRawData;
  audio_msg.data_length = bytesread;
  audio_pub.publish(&audio_msg);
  nh.spinOnce();
  // TODO: set appropriate hz
  // Check hz of readMic() function.
  delay(30);
  loop_count++;

  if (loop_count == 100) {
    vTaskSuspend(xTaskMic);
    Serial.println("suspend");
    delay(3000);
    vTaskResume(xTaskMic);
    Serial.println("resume");
    loop_count = 0;
  }
}
