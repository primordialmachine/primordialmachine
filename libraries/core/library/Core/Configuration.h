#if !defined(CORE_CONFIGURATION_H_INCLUDED)
#define CORE_CONFIGURATION_H_INCLUDED

#include "Core/String.h"
#include "dx/core/inline_pointer_hashmap.h"

Core_declareObjectType("Core.Configuration",
                       Core_Configuration,
                       Core_Object);

static inline Core_Configuration* CORE_CONFIGURATION(void* p) {
  return (Core_Configuration*)p;
}                       

struct Core_Configuration {
  Core_Object _parent;
  Core_InlineHashMapPP backend;
};

static inline Core_Configuration_Dispatch* CORE_CONFIGURATION_DISPATCH(void* p) {
  return (Core_Configuration_Dispatch*)p;
}

struct Core_Configuration_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_Configuration_construct(Core_Configuration* SELF);

Core_Result Core_Configuration_create(Core_Configuration** RETURN);

Core_Result Core_Configuration_setN8(Core_Configuration *SELF, Core_String* key, Core_Natural8 value);

Core_Result Core_Configuration_setS(Core_Configuration* SELF, Core_String* key, Core_String* value);

Core_Result Core_Configuration_getN8(Core_Natural8* RETURN, Core_Configuration* SELF, Core_String* key);

Core_Result Core_Configuration_getS(Core_String** RETURN, Core_Configuration* SELF, Core_String* key);

Core_declareObjectType("Core.ConfigurationIterator",
                       Core_ConfigurationIterator,
                       Core_Object);

static inline Core_ConfigurationIterator* CORE_CONFIGURATIONITERATOR(void* p) {
  return (Core_ConfigurationIterator*)p;
}

struct Core_ConfigurationIterator {
  Core_Object _parent;
  Core_Configuration *target;
  Core_InlineHashMapPPIterator backend;
};

static inline Core_ConfigurationIterator_Dispatch* CORE_CONFIGURATIONITERATOR_DISPATCH(void* p) {
  return (Core_ConfigurationIterator_Dispatch*)p;
}

struct Core_ConfigurationIterator_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_ConfigurationIterator_construct(Core_ConfigurationIterator* SELF, Core_Configuration* target);

Core_Result Core_ConfigurationIterator_create(Core_ConfigurationIterator** RETURN, Core_Configuration* target);

/*iterator api*/
Core_Result Core_ConfigurationIterator_hasEntry(Core_Boolean* RETURN, Core_ConfigurationIterator* SELF);

/*iterator api*/
Core_Result Core_ConfigurationIterator_getValue(Core_String** RETURN, Core_ConfigurationIterator* SELF);

/*iterator api*/
Core_Result Core_ConfigurationIterator_getKey(Core_String** RETURN, Core_ConfigurationIterator* SELF);
                       
#endif // CORE_CONFIGURATION_H_INCLUDED
