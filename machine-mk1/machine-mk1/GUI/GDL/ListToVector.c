#include "./../../GUI/GDL/ListToVector.h"
#include "./../../GUI/GDL/ValueToReal.h"

Machine_Math_Vector3* Machine_GUI_GDL_listToVector3(const Machine_List* list) {
  size_t size = Machine_Collection_getSize((const Machine_Collection*)list);
  MACHINE_ASSERT(size == 3, Machine_Status_InvalidArgument);
  Machine_Value values[] = {
    Machine_List_getAt(list, 0),
    Machine_List_getAt(list, 1),
    Machine_List_getAt(list, 2),
  };
  Machine_Math_Vector3* vector = Machine_Math_Vector3_create();
  Machine_Math_Vector3_set(vector, Machine_GUI_GDL_valueToReal(&values[0]),
                                   Machine_GUI_GDL_valueToReal(&values[1]),
                                   Machine_GUI_GDL_valueToReal(&values[2]));
  return vector;
}

Machine_Math_Vector4* Machine_GUI_GDL_listToVector4(const Machine_List* list) {
  size_t size = Machine_Collection_getSize((const Machine_Collection*)list);
  MACHINE_ASSERT(size == 4, Machine_Status_InvalidArgument);
  Machine_Value values[] = {
    Machine_List_getAt(list, 0),
    Machine_List_getAt(list, 1),
    Machine_List_getAt(list, 2),
    Machine_List_getAt(list, 3),
  };
  Machine_Math_Vector4* vector = Machine_Math_Vector4_create();
  Machine_Math_Vector4_set(vector, Machine_GUI_GDL_valueToReal(&values[0]),
                                   Machine_GUI_GDL_valueToReal(&values[1]),
                                   Machine_GUI_GDL_valueToReal(&values[2]),
                                   Machine_GUI_GDL_valueToReal(&values[3]));
  return vector;
}
