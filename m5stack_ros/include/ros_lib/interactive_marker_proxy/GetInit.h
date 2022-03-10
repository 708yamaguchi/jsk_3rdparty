#ifndef _ROS_SERVICE_GetInit_h
#define _ROS_SERVICE_GetInit_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "visualization_msgs/InteractiveMarkerInit.h"

namespace interactive_marker_proxy
{

static const char GETINIT[] = "interactive_marker_proxy/GetInit";

  class GetInitRequest : public ros::Msg
  {
    public:

    GetInitRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return GETINIT; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetInitResponse : public ros::Msg
  {
    public:
      typedef visualization_msgs::InteractiveMarkerInit _msg_type;
      _msg_type msg;

    GetInitResponse():
      msg()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->msg.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->msg.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return GETINIT; };
    const char * getMD5(){ return "4880f9fbacc796ee9d6c08994daf3e3c"; };

  };

  class GetInit {
    public:
    typedef GetInitRequest Request;
    typedef GetInitResponse Response;
  };

}
#endif
