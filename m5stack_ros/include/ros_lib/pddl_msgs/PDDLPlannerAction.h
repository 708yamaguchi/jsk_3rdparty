#ifndef _ROS_pddl_msgs_PDDLPlannerAction_h
#define _ROS_pddl_msgs_PDDLPlannerAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "pddl_msgs/PDDLPlannerActionGoal.h"
#include "pddl_msgs/PDDLPlannerActionResult.h"
#include "pddl_msgs/PDDLPlannerActionFeedback.h"

namespace pddl_msgs
{

  class PDDLPlannerAction : public ros::Msg
  {
    public:
      typedef pddl_msgs::PDDLPlannerActionGoal _action_goal_type;
      _action_goal_type action_goal;
      typedef pddl_msgs::PDDLPlannerActionResult _action_result_type;
      _action_result_type action_result;
      typedef pddl_msgs::PDDLPlannerActionFeedback _action_feedback_type;
      _action_feedback_type action_feedback;

    PDDLPlannerAction():
      action_goal(),
      action_result(),
      action_feedback()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->action_goal.serialize(outbuffer + offset);
      offset += this->action_result.serialize(outbuffer + offset);
      offset += this->action_feedback.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->action_goal.deserialize(inbuffer + offset);
      offset += this->action_result.deserialize(inbuffer + offset);
      offset += this->action_feedback.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "pddl_msgs/PDDLPlannerAction"; };
    const char * getMD5(){ return "c37a38bd984d0191f0e685d570293874"; };

  };

}
#endif
