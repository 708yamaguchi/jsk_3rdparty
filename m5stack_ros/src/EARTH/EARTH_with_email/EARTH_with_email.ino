#include <EARTH.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>
#include <jsk_robot_startup/Email.h>

std_msgs::Bool moist_msg;
ros::Publisher moist_pub("moist", &moist_msg);
std_msgs::Int16 moisture_msg;
ros::Publisher moisture_pub("moisture", &moisture_msg);
jsk_robot_startup::Email email_msg;
ros::Publisher email_pub("email", &email_msg);

void publishEARTH() {
  measureEARTH();
  moist_msg.data = moist;
  moisture_msg.data = moisture;
  moist_pub.publish(&moist_msg);
  moisture_pub.publish(&moisture_msg);
}

void publishEmail() {
  email_msg.header.stamp = nh.now();
  email_msg.subject = "スポットクーラーのタンクの状態";
  char body[200];
  if(moisture > 4000){
    sprintf(body, "まだ水は溜まっていません。");
  }
  else {
    sprintf(body, "水が溜まっています。交換してください。");
  }
  uint8_t _low_bat = M5.Axp.GetWarningLevel();
  float battery_level = M5.Axp.GetBatVoltage();
  if (_low_bat == 0) {
    sprintf(body, "%s\n\nM5Stickのバッテリ電圧は%f[V]で、十分高いです\n", body, battery_level);
  }
  else {
    sprintf(body, "%s\n\nM5Stickのバッテリ電圧は%f[V]で、低電力状態です。充電してください。\n", battery_level);
  }
  email_msg.body = body;
  email_msg.sender_address = "yamaguchi@jsk.imi.i.u-tokyo.ac.jp";
  email_msg.receiver_address = "yamaguchi@jsk.imi.i.u-tokyo.ac.jp";
  email_pub.publish(&email_msg);
}

void setup()
{
  setupM5stackROS();
  setupEARTH();
  #if defined(M5STACK)
    M5.Lcd.setBrightness(0);
  #elif defined(M5STICK_C) || defined(M5STICK_C_PLUS)
    M5.Axp.SetLDO2(false);
  #endif

  nh.advertise(moist_pub);
  nh.advertise(moisture_pub);
  nh.advertise(email_pub);
}

void loop()
{
  publishEARTH();
  publishEmail();
  nh.spinOnce();
  // esp_deep_sleep(100 * 1000 * 1000); // Restart after 30 seconds
  delay(3000);
}
