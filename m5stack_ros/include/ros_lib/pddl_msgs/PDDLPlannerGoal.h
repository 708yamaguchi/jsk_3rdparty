#ifndef _ROS_pddl_msgs_PDDLPlannerGoal_h
#define _ROS_pddl_msgs_PDDLPlannerGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "pddl_msgs/PDDLDomain.h"
#include "pddl_msgs/PDDLProblem.h"
#include "ros/duration.h"

namespace pddl_msgs
{

  class PDDLPlannerGoal : public ros::Msg
  {
    public:
      typedef pddl_msgs::PDDLDomain _domain_type;
      _domain_type domain;
      typedef pddl_msgs::PDDLProblem _problem_type;
      _problem_type problem;
      typedef ros::Duration _max_planning_time_type;
      _max_planning_time_type max_planning_time;

    PDDLPlannerGoal():
      domain(),
      problem(),
      max_planning_time()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->domain.serialize(outbuffer + offset);
      offset += this->problem.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->max_planning_time.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->max_planning_time.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->max_planning_time.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->max_planning_time.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->max_planning_time.sec);
      *(outbuffer + offset + 0) = (this->max_planning_time.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->max_planning_time.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->max_planning_time.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->max_planning_time.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->max_planning_time.nsec);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->domain.deserialize(inbuffer + offset);
      offset += this->problem.deserialize(inbuffer + offset);
      this->max_planning_time.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->max_planning_time.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->max_planning_time.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->max_planning_time.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->max_planning_time.sec);
      this->max_planning_time.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->max_planning_time.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->max_planning_time.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->max_planning_time.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->max_planning_time.nsec);
     return offset;
    }

    const char * getType(){ return "pddl_msgs/PDDLPlannerGoal"; };
    const char * getMD5(){ return "edde570a56f98a3f657b9fa6fcd5af2b"; };

  };

}
#endif
