#ifndef _ROS_pddl_msgs_PDDLActionArray_h
#define _ROS_pddl_msgs_PDDLActionArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "pddl_msgs/PDDLAction.h"

namespace pddl_msgs
{

  class PDDLActionArray : public ros::Msg
  {
    public:
      uint32_t actions_length;
      typedef pddl_msgs::PDDLAction _actions_type;
      _actions_type st_actions;
      _actions_type * actions;

    PDDLActionArray():
      actions_length(0), actions(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->actions_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->actions_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->actions_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->actions_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->actions_length);
      for( uint32_t i = 0; i < actions_length; i++){
      offset += this->actions[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t actions_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      actions_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      actions_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      actions_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->actions_length);
      if(actions_lengthT > actions_length)
        this->actions = (pddl_msgs::PDDLAction*)realloc(this->actions, actions_lengthT * sizeof(pddl_msgs::PDDLAction));
      actions_length = actions_lengthT;
      for( uint32_t i = 0; i < actions_length; i++){
      offset += this->st_actions.deserialize(inbuffer + offset);
        memcpy( &(this->actions[i]), &(this->st_actions), sizeof(pddl_msgs::PDDLAction));
      }
     return offset;
    }

    const char * getType(){ return "pddl_msgs/PDDLActionArray"; };
    const char * getMD5(){ return "e99b1b4c780bc71e4f0a0e590a6217ed"; };

  };

}
#endif
