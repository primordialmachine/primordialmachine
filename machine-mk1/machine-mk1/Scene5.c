#include "Scene5.h"



#include <stddef.h>
#include <malloc.h>

#include "UtilitiesGl.h"

#include "Fonts.h"
#include "Shape2.h"
#include "Images.h"
#include "Buffer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexDescriptor.h"
#include "Binding.h"
#include "GUI/TextLabel.h"
#include "GUI/TextButton.h"
#include "GUI/Border.h"
#include "GUI/WidgetList.h"
#include "GUI/Context.h"


static const float OUTER_BORDER_SIZE = 4.f;
static const float INNER_BORDER_SIZE = 16.f;


struct Scene5 {
  Scene parent;
  Machine_Fonts_Font* font;
  Machine_GUI_Context* context;
  /// @brief The main menu (start game, options, exit, credits).
  Machine_GUI_WidgetList* mainMenu;
  /// @brief Label #2.
  Machine_GUI_Widget* label2;
  /// @brief Label #3.
  Machine_GUI_Widget* label3;
};

void Scene5_destruct(Scene5* self);

static void Scene5_visit(Scene5* self) {
  if (self->font) {
    Machine_visit(self->font);
  }
  if (self->context) {
    Machine_visit(self->context);
  }
  if (self->mainMenu) {
    Machine_visit(self->mainMenu);
  }
  if (self->label2) {
    Machine_visit(self->label2);
  }
  if (self->label3) {
    Machine_visit(self->label3);
  }
}

MACHINE_DEFINE_CLASSTYPE(Scene5)
MACHINE_DEFINE_CLASSTYPE_EX(Scene5, Scene, &Scene5_visit, &Scene5_construct, NULL)

static Machine_GUI_Widget* createTextLabel(const char* text, Machine_Fonts_Font *font) {
  Machine_GUI_TextLabel* widget;
  //
  widget = Machine_GUI_TextLabel_create();
  Machine_GUI_TextLabel_setText(widget, Machine_String_create(text, strlen(text)));

  Machine_Math_Vector2* sz = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(sz, 5.5, 5.f);

  Machine_GUI_Border* border0 = Machine_GUI_Border_create();
  Machine_GUI_Border_setBorderWidth(border0, INNER_BORDER_SIZE);
  Machine_GUI_Border_setChild(border0, (Machine_GUI_Widget *)widget);
  Machine_Math_Vector3* color = Machine_Math_Vector3_create();
  Machine_Math_Vector3_set(color, 1.f, 1.f, 1.f);
  Machine_GUI_Border_setBorderColor(border0, color);

  Machine_GUI_Border *border1 = Machine_GUI_Border_create();
  Machine_GUI_Border_setBorderWidth(border1, OUTER_BORDER_SIZE);
  Machine_GUI_Border_setChild(border1, (Machine_GUI_Widget*)border0);
  
  return (Machine_GUI_Widget*)border1;
}

static Machine_GUI_Widget* createTextButton(const char* text, Machine_Fonts_Font* font) {
  Machine_GUI_TextButton* widget;
  //
  widget = Machine_GUI_TextButton_create();
  Machine_GUI_TextButton_setText(widget, Machine_String_create(text, strlen(text)));

  Machine_Math_Vector2* sz = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(sz, 5.5, 5.f);

  Machine_GUI_Border* border0 = Machine_GUI_Border_create();
  Machine_GUI_Border_setBorderWidth(border0, INNER_BORDER_SIZE);
  Machine_GUI_Border_setChild(border0, (Machine_GUI_Widget*)widget);
  Machine_Math_Vector3* color = Machine_Math_Vector3_create();
  Machine_Math_Vector3_set(color, 1.f, 1.f, 1.f);
  Machine_GUI_Border_setBorderColor(border0, color);

  Machine_GUI_Border* border1 = Machine_GUI_Border_create();
  Machine_GUI_Border_setBorderWidth(border1, OUTER_BORDER_SIZE);
  Machine_GUI_Border_setChild(border1, (Machine_GUI_Widget*)border0);

  return (Machine_GUI_Widget*)border1;
}


static void Scene5_startup(Scene5* self) {
  self->font = Machine_Fonts_createFont("RobotoSlab-Regular.ttf", 20);
  //
  self->context = Machine_GUI_Context_create();
  //
  self->mainMenu = Machine_GUI_WidgetList_create();
  Machine_GUI_WidgetList_append(self->mainMenu, createTextButton("Start Game", self->font));
  Machine_GUI_WidgetList_append(self->mainMenu, createTextButton("Options", self->font));
  Machine_GUI_WidgetList_append(self->mainMenu, createTextButton("Exit Game", self->font));
  Machine_GUI_WidgetList_append(self->mainMenu, createTextButton("Credits", self->font));
  //
  self->label2 = createTextLabel("Server Version 1.0\nClient Version 1.0", self->font);
  //
  self->label3 = createTextLabel("Nanobox IV\n400 units of unprimed nanites.", self->font);
}

/// @brief Compute the rectangle of all widgets.
/// @param widgets The list widgets.
/// @return The rectangle.
/// @error The widget list is empty.
static Machine_Math_Rectangle2* Machine_GUI_WidgetList_getRectangle(Machine_GUI_WidgetList* widgets) {
  size_t i = 0;
  Machine_GUI_Widget* widget = Machine_GUI_WidgetList_getAt(widgets, i); // Raises index out of bounds error if empty.
  Machine_Math_Rectangle2* rectangle = Machine_Math_Rectangle2_clone(Machine_GUI_Widget_getRectangle(widget));
  i++;
  for (size_t n = Machine_GUI_WidgetList_getSize(widgets); i < n; ++i) {
    widget = (Machine_GUI_Widget*)Machine_GUI_WidgetList_getAt(widgets, i);
    const Machine_Math_Rectangle2* widgetRectangle = Machine_GUI_Widget_getRectangle(widget);
    Machine_Math_Rectangle2_addRectangle(rectangle, widgetRectangle);
  }
  return rectangle;
}

static void updateText1(Scene5* scene, float width, float height) {
  // Compute component-wise maxima vector of the preferred sizes of the n buttons.
  Machine_Math_Vector2* preferredSize = Machine_Math_Vector2_clone(Machine_GUI_Widget_getPreferredSize(Machine_GUI_WidgetList_getAt(scene->mainMenu, 0)));
  for (size_t i = 1, n = Machine_GUI_WidgetList_getSize(scene->mainMenu); i < n; ++i) {
    Machine_Math_Vector2_maxima(preferredSize, preferredSize, Machine_GUI_Widget_getPreferredSize(Machine_GUI_WidgetList_getAt(scene->mainMenu, i)));
  }

  Machine_Math_Vector2* HALF = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(HALF, 0.5f, 0.5f);
  Machine_Math_Vector2* CANVAS_SIZE = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(CANVAS_SIZE, width, height);
  Machine_Math_Vector2* CANVAS_HALF_SIZE = Machine_Math_Vector2_product(CANVAS_SIZE, HALF);
  Machine_Math_Vector2* CANVAS_CENTER = Machine_Math_Vector2_clone(CANVAS_HALF_SIZE);

  //Machine_PointerArray* widgets = Machine_PointerArray_create();
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(scene->mainMenu); i < n; ++i) {
    Machine_GUI_Widget_setSize(Machine_GUI_WidgetList_getAt(scene->mainMenu, i), preferredSize);
  }
  Machine_GUI_WidgetList_centerVertically(scene->mainMenu, Machine_Math_Vector2_getX(CANVAS_CENTER));
  Machine_GUI_WidgetList_layoutColumn(scene->mainMenu, 5.f);
  Machine_Math_Rectangle2* rectangle = Machine_GUI_WidgetList_getRectangle(scene->mainMenu);
  
  const Machine_Math_Vector2* c = Machine_Math_Rectangle2_getCenter(rectangle);
  Machine_Math_Vector2* d = Machine_Math_Vector2_difference(CANVAS_CENTER, c);
  Machine_GUI_WidgetList_translate(scene->mainMenu, d);
}

static void updateText2(Scene5* scene, float width, float height) {
  Machine_Math_Vector2* MARGIN = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(MARGIN, 5.f, 5.f);
  // Set the size to the best size.
  const Machine_Math_Vector2* size = Machine_GUI_Widget_getPreferredSize(scene->label2);
  Machine_GUI_Widget_setSize((Machine_GUI_Widget*)scene->label2, size);
  // Set the position to the margins.
  Machine_GUI_Widget_setPosition((Machine_GUI_Widget*)scene->label2, MARGIN);
}

static void updateText3(Scene5* scene, float width, float height) {
  Machine_Math_Vector2* MARGIN = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(MARGIN, 5.f, height * 0.5f);
  Machine_Math_Vector2* SIZE = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(SIZE, 128, 128);
  Machine_GUI_Widget_setSize((Machine_GUI_Widget*)scene->label3, SIZE);
  const Machine_Math_Rectangle2* bounds = Machine_GUI_Widget_getRectangle((Machine_GUI_Widget*)scene->label3);
  const Machine_Math_Vector2* leftTop = Machine_Math_Rectangle2_getPosition(bounds);
  Machine_Math_Vector2* delta = Machine_Math_Vector2_difference(MARGIN, leftTop);
  const Machine_Math_Vector2* oldPosition = Machine_GUI_Widget_getPosition((Machine_GUI_Widget*)scene->label3);
  Machine_Math_Vector2* newPosition = Machine_Math_Vector2_sum(oldPosition, delta);
  Machine_GUI_Widget_setPosition((Machine_GUI_Widget*)scene->label3, newPosition);
}

static void Scene5_onCanvasSizeChanged(Scene5* self, Machine_CanvasSizeChangedEvent* event) {
  Machine_GUI_Context_setCanvasSize(self->context, event->width, event->height);
  updateText1(self, event->width, event->height);
  updateText2(self, event->width, event->height);
  updateText3(self, event->width, event->height);
}

static void Scene5_update(Scene5* self, float width, float height) {
  // Set the viewport and clear its color buffer.
  Machine_UtilitiesGl_call(glViewport(0, 0, width, height));
  Machine_UtilitiesGl_call(glClear(GL_COLOR_BUFFER_BIT));

  Machine_GUI_Context_setCanvasSize(self->context, width, height);
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self->mainMenu); i < n; ++i) {
    Machine_GUI_Widget* widget = Machine_GUI_WidgetList_getAt(self->mainMenu, i);
    Machine_GUI_Widget_render(widget, self->context, width, height);
  }
  Machine_GUI_Widget_render((Machine_GUI_Widget*)self->label2, self->context, width, height);
  Machine_GUI_Widget_render((Machine_GUI_Widget*)self->label3, self->context, width, height);
}

static void Scene5_shutdown(Scene5* scene) {
  scene->font = NULL;
}

void Scene5_construct(Scene5* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Scene_construct((Scene*)self, numberOfArguments, arguments);
  ((Scene*)self)->onCanvasSizeChanged = (Scene_OnCanvaSizeChangedCallback*)&Scene5_onCanvasSizeChanged;
  ((Scene*)self)->onStartup = (Scene_OnStartupCallback*)&Scene5_startup;
  ((Scene*)self)->onUpdate = (Scene_OnUpdateCallback*)&Scene5_update;
  ((Scene*)self)->onShutdown = (Scene_OnShutdownCallback*)&Scene5_shutdown;
  Machine_setClassType((Machine_Object*)self, Scene5_getClassType());
}

void Scene5_destruct(Scene5* self) {
  self->font = NULL;
}

Scene5* Scene5_create() {
  Machine_ClassType* ty = Scene5_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 0;
  static const Machine_Value ARGUMENTS[] = { { Machine_ValueFlag_Void, Machine_Void_Void } };
  Scene5* scene = (Scene5*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return scene;
}
