#if !defined(SCENE5_H_INCLUDED)
#define SCENE5_H_INCLUDED


#include "Scene.h"


MACHINE_DECLARE_CLASSTYPE(Scene5)

void Scene5_construct(Scene5* self, size_t numberOfArguments, const Machine_Value* arguments);

Scene5* Scene5_create();

#endif // SCENE5_H_INCLUDED
