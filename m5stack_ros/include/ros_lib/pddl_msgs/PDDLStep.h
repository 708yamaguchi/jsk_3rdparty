#ifndef _ROS_pddl_msgs_PDDLStep_h
#define _ROS_pddl_msgs_PDDLStep_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace pddl_msgs
{

  class PDDLStep : public ros::Msg
  {
    public:
      typedef const char* _action_type;
      _action_type action;
      uint32_t args_length;
      typedef char* _args_type;
      _args_type st_args;
      _args_type * args;
      typedef const char* _start_time_type;
      _start_time_type start_time;
      typedef const char* _action_duration_type;
      _action_duration_type action_duration;

    PDDLStep():
      action(""),
      args_length(0), args(NULL),
      start_time(""),
      action_duration("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_action = strlen(this->action);
      varToArr(outbuffer + offset, length_action);
      offset += 4;
      memcpy(outbuffer + offset, this->action, length_action);
      offset += length_action;
      *(outbuffer + offset + 0) = (this->args_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->args_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->args_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->args_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->args_length);
      for( uint32_t i = 0; i < args_length; i++){
      uint32_t length_argsi = strlen(this->args[i]);
      varToArr(outbuffer + offset, length_argsi);
      offset += 4;
      memcpy(outbuffer + offset, this->args[i], length_argsi);
      offset += length_argsi;
      }
      uint32_t length_start_time = strlen(this->start_time);
      varToArr(outbuffer + offset, length_start_time);
      offset += 4;
      memcpy(outbuffer + offset, this->start_time, length_start_time);
      offset += length_start_time;
      uint32_t length_action_duration = strlen(this->action_duration);
      varToArr(outbuffer + offset, length_action_duration);
      offset += 4;
      memcpy(outbuffer + offset, this->action_duration, length_action_duration);
      offset += length_action_duration;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_action;
      arrToVar(length_action, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_action; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_action-1]=0;
      this->action = (char *)(inbuffer + offset-1);
      offset += length_action;
      uint32_t args_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      args_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      args_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      args_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->args_length);
      if(args_lengthT > args_length)
        this->args = (char**)realloc(this->args, args_lengthT * sizeof(char*));
      args_length = args_lengthT;
      for( uint32_t i = 0; i < args_length; i++){
      uint32_t length_st_args;
      arrToVar(length_st_args, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_args; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_args-1]=0;
      this->st_args = (char *)(inbuffer + offset-1);
      offset += length_st_args;
        memcpy( &(this->args[i]), &(this->st_args), sizeof(char*));
      }
      uint32_t length_start_time;
      arrToVar(length_start_time, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_start_time; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_start_time-1]=0;
      this->start_time = (char *)(inbuffer + offset-1);
      offset += length_start_time;
      uint32_t length_action_duration;
      arrToVar(length_action_duration, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_action_duration; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_action_duration-1]=0;
      this->action_duration = (char *)(inbuffer + offset-1);
      offset += length_action_duration;
     return offset;
    }

    const char * getType(){ return "pddl_msgs/PDDLStep"; };
    const char * getMD5(){ return "d79a0663dfa206e7ac7a0755c6d1e154"; };

  };

}
#endif
