#ifndef _ROS_pddl_msgs_PDDLDomain_h
#define _ROS_pddl_msgs_PDDLDomain_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "pddl_msgs/PDDLAction.h"

namespace pddl_msgs
{

  class PDDLDomain : public ros::Msg
  {
    public:
      typedef const char* _name_type;
      _name_type name;
      typedef const char* _requirements_type;
      _requirements_type requirements;
      uint32_t types_length;
      typedef char* _types_type;
      _types_type st_types;
      _types_type * types;
      uint32_t constants_length;
      typedef char* _constants_type;
      _constants_type st_constants;
      _constants_type * constants;
      uint32_t predicates_length;
      typedef char* _predicates_type;
      _predicates_type st_predicates;
      _predicates_type * predicates;
      uint32_t actions_length;
      typedef pddl_msgs::PDDLAction _actions_type;
      _actions_type st_actions;
      _actions_type * actions;
      uint32_t functions_length;
      typedef char* _functions_type;
      _functions_type st_functions;
      _functions_type * functions;

    PDDLDomain():
      name(""),
      requirements(""),
      types_length(0), types(NULL),
      constants_length(0), constants(NULL),
      predicates_length(0), predicates(NULL),
      actions_length(0), actions(NULL),
      functions_length(0), functions(NULL)
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
      uint32_t length_requirements = strlen(this->requirements);
      varToArr(outbuffer + offset, length_requirements);
      offset += 4;
      memcpy(outbuffer + offset, this->requirements, length_requirements);
      offset += length_requirements;
      *(outbuffer + offset + 0) = (this->types_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->types_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->types_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->types_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->types_length);
      for( uint32_t i = 0; i < types_length; i++){
      uint32_t length_typesi = strlen(this->types[i]);
      varToArr(outbuffer + offset, length_typesi);
      offset += 4;
      memcpy(outbuffer + offset, this->types[i], length_typesi);
      offset += length_typesi;
      }
      *(outbuffer + offset + 0) = (this->constants_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->constants_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->constants_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->constants_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->constants_length);
      for( uint32_t i = 0; i < constants_length; i++){
      uint32_t length_constantsi = strlen(this->constants[i]);
      varToArr(outbuffer + offset, length_constantsi);
      offset += 4;
      memcpy(outbuffer + offset, this->constants[i], length_constantsi);
      offset += length_constantsi;
      }
      *(outbuffer + offset + 0) = (this->predicates_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->predicates_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->predicates_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->predicates_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->predicates_length);
      for( uint32_t i = 0; i < predicates_length; i++){
      uint32_t length_predicatesi = strlen(this->predicates[i]);
      varToArr(outbuffer + offset, length_predicatesi);
      offset += 4;
      memcpy(outbuffer + offset, this->predicates[i], length_predicatesi);
      offset += length_predicatesi;
      }
      *(outbuffer + offset + 0) = (this->actions_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->actions_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->actions_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->actions_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->actions_length);
      for( uint32_t i = 0; i < actions_length; i++){
      offset += this->actions[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->functions_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->functions_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->functions_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->functions_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->functions_length);
      for( uint32_t i = 0; i < functions_length; i++){
      uint32_t length_functionsi = strlen(this->functions[i]);
      varToArr(outbuffer + offset, length_functionsi);
      offset += 4;
      memcpy(outbuffer + offset, this->functions[i], length_functionsi);
      offset += length_functionsi;
      }
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
      uint32_t length_requirements;
      arrToVar(length_requirements, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_requirements; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_requirements-1]=0;
      this->requirements = (char *)(inbuffer + offset-1);
      offset += length_requirements;
      uint32_t types_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      types_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      types_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      types_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->types_length);
      if(types_lengthT > types_length)
        this->types = (char**)realloc(this->types, types_lengthT * sizeof(char*));
      types_length = types_lengthT;
      for( uint32_t i = 0; i < types_length; i++){
      uint32_t length_st_types;
      arrToVar(length_st_types, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_types; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_types-1]=0;
      this->st_types = (char *)(inbuffer + offset-1);
      offset += length_st_types;
        memcpy( &(this->types[i]), &(this->st_types), sizeof(char*));
      }
      uint32_t constants_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      constants_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      constants_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      constants_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->constants_length);
      if(constants_lengthT > constants_length)
        this->constants = (char**)realloc(this->constants, constants_lengthT * sizeof(char*));
      constants_length = constants_lengthT;
      for( uint32_t i = 0; i < constants_length; i++){
      uint32_t length_st_constants;
      arrToVar(length_st_constants, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_constants; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_constants-1]=0;
      this->st_constants = (char *)(inbuffer + offset-1);
      offset += length_st_constants;
        memcpy( &(this->constants[i]), &(this->st_constants), sizeof(char*));
      }
      uint32_t predicates_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      predicates_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      predicates_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      predicates_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->predicates_length);
      if(predicates_lengthT > predicates_length)
        this->predicates = (char**)realloc(this->predicates, predicates_lengthT * sizeof(char*));
      predicates_length = predicates_lengthT;
      for( uint32_t i = 0; i < predicates_length; i++){
      uint32_t length_st_predicates;
      arrToVar(length_st_predicates, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_predicates; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_predicates-1]=0;
      this->st_predicates = (char *)(inbuffer + offset-1);
      offset += length_st_predicates;
        memcpy( &(this->predicates[i]), &(this->st_predicates), sizeof(char*));
      }
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
      uint32_t functions_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      functions_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      functions_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      functions_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->functions_length);
      if(functions_lengthT > functions_length)
        this->functions = (char**)realloc(this->functions, functions_lengthT * sizeof(char*));
      functions_length = functions_lengthT;
      for( uint32_t i = 0; i < functions_length; i++){
      uint32_t length_st_functions;
      arrToVar(length_st_functions, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_functions; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_functions-1]=0;
      this->st_functions = (char *)(inbuffer + offset-1);
      offset += length_st_functions;
        memcpy( &(this->functions[i]), &(this->st_functions), sizeof(char*));
      }
     return offset;
    }

    const char * getType(){ return "pddl_msgs/PDDLDomain"; };
    const char * getMD5(){ return "1db1abf7dbdf3e62fc55c75c4b365253"; };

  };

}
#endif
