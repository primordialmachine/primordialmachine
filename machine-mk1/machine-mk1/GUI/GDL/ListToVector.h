#if !defined(MACHINE_GUI_GDL_LISTTOVECTOR3_H_INCLUDED)
#define MACHINE_GUI_GDL_LISTTOVECTOR3_H_INCLUDED

#include "_Math.h"
#include "_Runtime.h"

/// @brief
/// Convert
/// a list with 3 values of type <code>Integer</code> or <code>Real</code>
/// to
/// a value of type <code>Math.Vector3</code>.
/// @param value A pointer to a value.
/// @return The value of type <code>Real</code>.
Machine_Math_Vector3* Machine_GUI_GDL_listToVector3(const Machine_List* list);

/// @brief
/// Convert
/// a list with 4 values of type <code>Integer</code> or <code>Real</code>
/// to
/// a value of type <code>Math.Vector4</code>.
/// @param value A pointer to a value.
/// @return The value of type <code>Real</code>.
Machine_Math_Vector4* Machine_GUI_GDL_listToVector4(const Machine_List* list);

#endif // MACHINE_GUI_GDL_LISTTOVECTOR3_H_INCLUDED
