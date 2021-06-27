#include "./../../GUI/Reader/Reader.h"


#include "./../../GUI/Context.h"
#include "./../../GUI/Reader/Reader.package.h"


Machine_GUI_Widget* Machine_GUI_Reader_readWidget(Machine_GUI_Context* self, const Machine_Map* source) {
  Machine_String* s = NULL;
  Machine_GUI_GDL_Context* guiGdlContext = self->gdlContext;
  if (!Machine_GUI_Reader_hasString(self, source, guiGdlContext->KIND)) {
    Machine_setStatus(Machine_Status_SemanticalError);
    Machine_jump();
  }
  s = Machine_GUI_Reader_getString(source, guiGdlContext->KIND);
  if (Machine_String_isEqualTo(s, guiGdlContext->GROUP)) {
    return (Machine_GUI_Widget*)Machine_GUI_Reader_readGroup(self, source);
  }
  else if (Machine_String_isEqualTo(s, guiGdlContext->TEXTBUTTON)) {
    return (Machine_GUI_Widget*)Machine_GUI_Reader_readTextButton(self, source);
  }
  else if (Machine_String_isEqualTo(s, guiGdlContext->TEXTLABEL)) {
    return (Machine_GUI_Widget*)Machine_GUI_Reader_readTextLabel(self, source);
  }
  else if (Machine_String_isEqualTo(s, guiGdlContext->BORDER)) {
    return (Machine_GUI_Widget*)Machine_GUI_Reader_readBorder(self, source);
  }
  else {
    Machine_setStatus(Machine_Status_SemanticalError);
    Machine_jump();
  }
}
