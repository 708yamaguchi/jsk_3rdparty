#ifndef _ROS_pddl_msgs_PDDLPlannerResult_h
#define _ROS_pddl_msgs_PDDLPlannerResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "pddl_msgs/PDDLStep.h"

namespace pddl_msgs
{

  class PDDLPlannerResult : public ros::Msg
  {
    public:
      uint32_t data_length;
      typedef char* _data_type;
      _data_type st_data;
      _data_type * data;
      typedef bool _use_durative_action_type;
      _use_durative_action_type use_durative_action;
      uint32_t sequence_length;
      typedef pddl_msgs::PDDLStep _sequence_type;
      _sequence_type st_sequence;
      _sequence_type * sequence;

    PDDLPlannerResult():
      data_length(0), data(NULL),
      use_durative_action(0),
      sequence_length(0), sequence(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->data_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->data_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->data_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->data_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data_length);
      for( uint32_t i = 0; i < data_length; i++){
      uint32_t length_datai = strlen(this->data[i]);
      varToArr(outbuffer + offset, length_datai);
      offset += 4;
      memcpy(outbuffer + offset, this->data[i], length_datai);
      offset += length_datai;
      }
      union {
        bool real;
        uint8_t base;
      } u_use_durative_action;
      u_use_durative_action.real = this->use_durative_action;
      *(outbuffer + offset + 0) = (u_use_durative_action.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->use_durative_action);
      *(outbuffer + offset + 0) = (this->sequence_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->sequence_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->sequence_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->sequence_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->sequence_length);
      for( uint32_t i = 0; i < sequence_length; i++){
      offset += this->sequence[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t data_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->data_length);
      if(data_lengthT > data_length)
        this->data = (char**)realloc(this->data, data_lengthT * sizeof(char*));
      data_length = data_lengthT;
      for( uint32_t i = 0; i < data_length; i++){
      uint32_t length_st_data;
      arrToVar(length_st_data, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_data; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_data-1]=0;
      this->st_data = (char *)(inbuffer + offset-1);
      offset += length_st_data;
        memcpy( &(this->data[i]), &(this->st_data), sizeof(char*));
      }
      union {
        bool real;
        uint8_t base;
      } u_use_durative_action;
      u_use_durative_action.base = 0;
      u_use_durative_action.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->use_durative_action = u_use_durative_action.real;
      offset += sizeof(this->use_durative_action);
      uint32_t sequence_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      sequence_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      sequence_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      sequence_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->sequence_length);
      if(sequence_lengthT > sequence_length)
        this->sequence = (pddl_msgs::PDDLStep*)realloc(this->sequence, sequence_lengthT * sizeof(pddl_msgs::PDDLStep));
      sequence_length = sequence_lengthT;
      for( uint32_t i = 0; i < sequence_length; i++){
      offset += this->st_sequence.deserialize(inbuffer + offset);
        memcpy( &(this->sequence[i]), &(this->st_sequence), sizeof(pddl_msgs::PDDLStep));
      }
     return offset;
    }

    const char * getType(){ return "pddl_msgs/PDDLPlannerResult"; };
    const char * getMD5(){ return "9220276db431d308099ea5d53fef9a9b"; };

  };

}
#endif
