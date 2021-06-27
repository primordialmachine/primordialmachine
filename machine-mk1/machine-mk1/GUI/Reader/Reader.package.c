#include "./../../GUI/Reader/Reader.package.h"

#include "./../../GUI/Reader/Reader.h"
#include "./../../GUI/Context.h"
#include "./../../GUI/GDL/ListToVector.h"

static void checkKind(Machine_GUI_Context* self, const Machine_Map* source, Machine_String* expected) {
  Machine_GUI_GDL_Context* context = self->gdlContext;
  if (!Machine_GUI_Reader_hasString(self, source, context->KIND)) {
    Machine_setStatus(Machine_Status_SemanticalError);
    Machine_jump();
  }
  Machine_String* received = Machine_GUI_Reader_getString(source, context->KIND);
  if (!Machine_String_isEqualTo(received, expected)) {
    Machine_setStatus(Machine_Status_SemanticalError);
    Machine_jump();
  }
}

Machine_GUI_LayoutModel* Machine_GUI_Reader_readLayout(Machine_GUI_Context* self, const Machine_Map* source) {
  Machine_GUI_GDL_Context* subContext = self->gdlContext;
  Machine_GUI_LayoutModel* model = Machine_GUI_LayoutModel_create();
  if (Machine_GUI_Reader_hasString(self, source, subContext->DIRECTION)) {
    Machine_String* temporary = Machine_GUI_Reader_getString(source, subContext->DIRECTION);
    if (Machine_String_isEqualTo(temporary, subContext->COLUMN)) {
      Machine_GUI_LayoutModel_setPrimaryDirection(model, Machine_GUI_Layout_Direction_Column);
    }
    else if (Machine_String_isEqualTo(temporary, subContext->COLUMNREVERSE)) {
      Machine_GUI_LayoutModel_setPrimaryDirection(model, Machine_GUI_Layout_Direction_ColumnReverse);
    }
    else if (Machine_String_isEqualTo(temporary, subContext->ROW)) {
      Machine_GUI_LayoutModel_setPrimaryDirection(model, Machine_GUI_Layout_Direction_Row);
    }
    else if (Machine_String_isEqualTo(temporary, subContext->ROWREVERSE)) {
      Machine_GUI_LayoutModel_setPrimaryDirection(model, Machine_GUI_Layout_Direction_RowReverse);
    }
    else {
      Machine_setStatus(Machine_Status_SemanticalError);
      Machine_jump();
    }
  }
  if (Machine_GUI_Reader_hasString(self, source, subContext->JUSTIFICATION)) {
    Machine_String* temporary = Machine_GUI_Reader_getString(source, subContext->JUSTIFICATION);
    if (Machine_String_isEqualTo(temporary, subContext->START)) {
      Machine_GUI_LayoutModel_setPrimaryJustification(model, Machine_GUI_Layout_Justification_Start);
    }
    else if (Machine_String_isEqualTo(temporary, subContext->CENTER)) {
      Machine_GUI_LayoutModel_setPrimaryJustification(model, Machine_GUI_Layout_Justification_Center);
    }
    else if (Machine_String_isEqualTo(temporary, subContext->END)) {
      Machine_GUI_LayoutModel_setPrimaryJustification(model, Machine_GUI_Layout_Justification_End);
    }
    else {
      Machine_setStatus(Machine_Status_LexicalError);
      Machine_jump();
    }
  }
  if (Machine_GUI_Reader_hasReal(self, source, subContext->INTERSPACING)) {
    Machine_Real temporary = Machine_GUI_Reader_getReal(source, subContext->INTERSPACING);
    Machine_GUI_LayoutModel_setPrimaryInterChildspacing(model, temporary);
  }
  return model;
}

Machine_GUI_Group* Machine_GUI_Reader_readGroup(Machine_GUI_Context* self, const Machine_Map* source) {
  Machine_GUI_GDL_Context* subContext = self->gdlContext;
  checkKind(self, source, subContext->GROUP);
  Machine_GUI_Group* widget = Machine_GUI_Group_create(self);
  if (Machine_GUI_Reader_hasList(self, source, subContext->CHILDREN)) {
    Machine_List* temporary1 = Machine_GUI_Reader_getList(self, source, subContext->CHILDREN);
    for (size_t i = 0, n = Machine_Collection_getSize((Machine_Collection*)temporary1); i < n; ++i) {
      Machine_Value temporary2 = Machine_List_getAt(temporary1, i);
      Machine_Map* temporary3 = (Machine_Map*)Machine_Value_getObject(&temporary2);
      Machine_GUI_Widget* childWidget = Machine_GUI_Reader_readWidget(self, (Machine_Map*)temporary3);

      // TODO: Should be Machine_GUI_Widget_appendChild.
      Machine_GUI_WidgetList_append(widget->children, childWidget);
      childWidget->parent = (Machine_GUI_Widget*)widget;
    }
  }
  if (Machine_GUI_Reader_hasMap(self, source, subContext->LAYOUT)) {
    Machine_Map* temporary1 = Machine_GUI_Reader_getMap(self, source, subContext->LAYOUT);
    Machine_GUI_LayoutModel* layout = Machine_GUI_Reader_readLayout(self, temporary1);
    Machine_GUI_Group_setLayoutModel(widget, layout);
  }
  return widget;
}

Machine_GUI_Border* Machine_GUI_Reader_readBorder(Machine_GUI_Context* self, const Machine_Map* source) {
  Machine_GUI_GDL_Context* subContext = self->gdlContext;
  checkKind(self, source, subContext->BORDER);
  Machine_GUI_Border* widget = Machine_GUI_Border_create(self);
  if (Machine_GUI_Reader_hasReal(self, source, subContext->BORDERWIDTH)) {
    Machine_Real temporary = Machine_GUI_Reader_getReal(source, subContext->BORDERWIDTH);
    Machine_GUI_Border_setBorderWidth(widget, temporary);
  }
  if (Machine_GUI_Reader_hasReal(self, source, subContext->LEFTBORDERWIDTH)) {
    Machine_Real temporary = Machine_GUI_Reader_getReal(source, subContext->LEFTBORDERWIDTH);
    Machine_GUI_Border_setLeftBorderWidth(widget, temporary);
  }
  if (Machine_GUI_Reader_hasReal(self, source, subContext->RIGHTBORDERWIDTH)) {
    Machine_Real temporary = Machine_GUI_Reader_getReal(source, subContext->RIGHTBORDERWIDTH);
    Machine_GUI_Border_setRightBorderWidth(widget, temporary);
  }
  if (Machine_GUI_Reader_hasReal(self, source, subContext->TOPBORDERWIDTH)) {
    Machine_Real temporary = Machine_GUI_Reader_getReal(source, subContext->TOPBORDERWIDTH);
    Machine_GUI_Border_setTopBorderWidth(widget, temporary);
  }
  if (Machine_GUI_Reader_hasReal(self, source, subContext->BOTTOMBORDERWIDTH)) {
    Machine_Real temporary = Machine_GUI_Reader_getReal(source, subContext->BOTTOMBORDERWIDTH);
    Machine_GUI_Border_setBottomBorderWidth(widget, temporary);
  }
  if (Machine_GUI_Reader_hasList(self, source, subContext->BORDERCOLOR)) {
    Machine_Math_Vector3* temporary = Machine_GUI_GDL_listToVector3(Machine_GUI_Reader_getList(self, source, subContext->BORDERCOLOR));
    Machine_Math_Vector4* temporary2 = Machine_Math_Vector4_create();
    Machine_Math_Vector4_set(temporary2, Machine_Math_Vector3_getX(temporary), Machine_Math_Vector3_getY(temporary), Machine_Math_Vector3_getZ(temporary), 1.f);
    Machine_GUI_Border_setBorderColor(widget, temporary2);
  }
  if (Machine_GUI_Reader_hasMap(self, source, subContext->CHILD)) {
    Machine_Map* temporary = Machine_GUI_Reader_getMap(self, source, subContext->CHILD);
    Machine_GUI_Widget* childWidget = Machine_GUI_Reader_readWidget(self, (Machine_Map*)temporary);
    Machine_GUI_Border_setChild(widget, childWidget);
  }
  return widget;
}

Machine_GUI_TextButton* Machine_GUI_Reader_readTextButton(Machine_GUI_Context* self, const Machine_Map* source) {
  Machine_GUI_GDL_Context* subContext = self->gdlContext;
  checkKind(self, source, subContext->TEXTBUTTON);
  Machine_GUI_TextButton* widget = Machine_GUI_TextButton_create(self);
  if (Machine_GUI_Reader_hasString(self, source, subContext->TEXT)) {
    Machine_String* temporary = Machine_GUI_Reader_getString(source, subContext->TEXT);
    Machine_GUI_TextButton_setText(widget, temporary);
  }
  return widget;
}

Machine_GUI_TextLabel* Machine_GUI_Reader_readTextLabel(Machine_GUI_Context* self, const Machine_Map* source) {
  Machine_GUI_GDL_Context* subContext = self->gdlContext;
  checkKind(self, source, subContext->TEXTLABEL);
  Machine_GUI_TextLabel* widget = Machine_GUI_TextLabel_create(self);
  if (Machine_GUI_Reader_hasString(self, source, subContext->TEXT)) {
    Machine_String* temporary = Machine_GUI_Reader_getString(source, subContext->TEXT);
    Machine_GUI_TextLabel_setText(widget, temporary);
  }
  return widget;
}
