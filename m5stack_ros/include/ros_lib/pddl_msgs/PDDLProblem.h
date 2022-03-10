#ifndef _ROS_pddl_msgs_PDDLProblem_h
#define _ROS_pddl_msgs_PDDLProblem_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "pddl_msgs/PDDLObject.h"

namespace pddl_msgs
{

  class PDDLProblem : public ros::Msg
  {
    public:
      typedef const char* _name_type;
      _name_type name;
      typedef const char* _domain_type;
      _domain_type domain;
      uint32_t objects_length;
      typedef pddl_msgs::PDDLObject _objects_type;
      _objects_type st_objects;
      _objects_type * objects;
      uint32_t initial_length;
      typedef char* _initial_type;
      _initial_type st_initial;
      _initial_type * initial;
      typedef const char* _goal_type;
      _goal_type goal;
      typedef const char* _metric_type;
      _metric_type metric;

    PDDLProblem():
      name(""),
      domain(""),
      objects_length(0), objects(NULL),
      initial_length(0), initial(NULL),
      goal(""),
      metric("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_name = strlen(this->name);
      varToArr(outbuffer + offset, length_name);
      offset += 4;
      memcpy(outbuffer + offset, this->name, length_name);
      offset += length_name;
      uint32_t length_domain = strlen(this->domain);
      varToArr(outbuffer + offset, length_domain);
      offset += 4;
      memcpy(outbuffer + offset, this->domain, length_domain);
      offset += length_domain;
      *(outbuffer + offset + 0) = (this->objects_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->objects_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->objects_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->objects_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->objects_length);
      for( uint32_t i = 0; i < objects_length; i++){
      offset += this->objects[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->initial_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->initial_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->initial_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->initial_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->initial_length);
      for( uint32_t i = 0; i < initial_length; i++){
      uint32_t length_initiali = strlen(this->initial[i]);
      varToArr(outbuffer + offset, length_initiali);
      offset += 4;
      memcpy(outbuffer + offset, this->initial[i], length_initiali);
      offset += length_initiali;
      }
      uint32_t length_goal = strlen(this->goal);
      varToArr(outbuffer + offset, length_goal);
      offset += 4;
      memcpy(outbuffer + offset, this->goal, length_goal);
      offset += length_goal;
      uint32_t length_metric = strlen(this->metric);
      varToArr(outbuffer + offset, length_metric);
      offset += 4;
      memcpy(outbuffer + offset, this->metric, length_metric);
      offset += length_metric;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_name;
      arrToVar(length_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_name-1]=0;
      this->name = (char *)(inbuffer + offset-1);
      offset += length_name;
      uint32_t length_domain;
      arrToVar(length_domain, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_domain; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_domain-1]=0;
      this->domain = (char *)(inbuffer + offset-1);
      offset += length_domain;
      uint32_t objects_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      objects_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      objects_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      objects_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->objects_length);
      if(objects_lengthT > objects_length)
        this->objects = (pddl_msgs::PDDLObject*)realloc(this->objects, objects_lengthT * sizeof(pddl_msgs::PDDLObject));
      objects_length = objects_lengthT;
      for( uint32_t i = 0; i < objects_length; i++){
      offset += this->st_objects.deserialize(inbuffer + offset);
        memcpy( &(this->objects[i]), &(this->st_objects), sizeof(pddl_msgs::PDDLObject));
      }
      uint32_t initial_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      initial_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      initial_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      initial_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->initial_length);
      if(initial_lengthT > initial_length)
        this->initial = (char**)realloc(this->initial, initial_lengthT * sizeof(char*));
      initial_length = initial_lengthT;
      for( uint32_t i = 0; i < initial_length; i++){
      uint32_t length_st_initial;
      arrToVar(length_st_initial, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_initial; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_initial-1]=0;
      this->st_initial = (char *)(inbuffer + offset-1);
      offset += length_st_initial;
        memcpy( &(this->initial[i]), &(this->st_initial), sizeof(char*));
      }
      uint32_t length_goal;
      arrToVar(length_goal, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_goal; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_goal-1]=0;
      this->goal = (char *)(inbuffer + offset-1);
      offset += length_goal;
      uint32_t length_metric;
      arrToVar(length_metric, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_metric; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_metric-1]=0;
      this->metric = (char *)(inbuffer + offset-1);
      offset += length_metric;
     return offset;
    }

    const char * getType(){ return "pddl_msgs/PDDLProblem"; };
    const char * getMD5(){ return "dfd9bdd094d91245128f960694763882"; };

  };

}
#endif
