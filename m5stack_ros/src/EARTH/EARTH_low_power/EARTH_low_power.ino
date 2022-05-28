#include <EARTH.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>

std_msgs::Bool moist_msg;
ros::Publisher moist_pub("moist", &moist_msg);
std_msgs::Int16 moisture_msg;
ros::Publisher moisture_pub("moisture", &moisture_msg);

void setup()
{
  setupM5stackROS();
  setupEARTH();
  #if defined(M5STACK)
    M5.Lcd.setBrightness(0);
  #elif defined(M5STICK_C) || defined(M5STICK_C_PLUS)
    M5.Axp.SetLDO2(false);
  #endif

  delay(3000); // Wait for rosserial node
  nh.advertise(moist_pub);
  nh.advertise(moisture_pub);
}

void loop()
{
  // Update connection
  nh.spinOnce();
  delay(3000);

  // Publish before rosserial timeout (15 seconds)
  measureEARTH();
  moist_msg.data = moist;
  moisture_msg.data = moisture;
  moist_pub.publish(&moist_msg);
  moisture_pub.publish(&moisture_msg);
  nh.spinOnce();
  delay(3000); // Wait for topics to be published

  // TODO: publish battery level
  // uint8_t _low_bat = M5.Axp.GetWarningLevel();
  // float battery_level = M5.Axp.GetBatVoltage();

  esp_deep_sleep(6 * 60 * 60 * 1000 * 1000); // Retart after 6 hours
}
