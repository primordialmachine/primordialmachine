#if !defined(MACHINE_GUI_WIDGETLIST_H_INCLUDED)
#define MACHINE_GUI_WIDGETLIST_H_INCLUDED

#include "./../GUI/Widget.h"
#include "./../GUI/LayoutModel.h"

/// @brief A label to display a text.
/// @remarks The preferred size of a panel is computed by combining the rectangles of its child nodes.
/// @extends Machine.GUI.Widget
MACHINE_DECLARE_CLASSTYPE(Machine_GUI_WidgetList)

/// @brief Create an empty panel.
/// @return The panel.
Machine_GUI_WidgetList* Machine_GUI_WidgetList_create();

/// @brief Get the size of this widget list.
/// @param self This widget list.
/// @return The size.
size_t Machine_GUI_WidgetList_getSize(Machine_GUI_WidgetList* self);

/// @brief Get the widget at an index in this widget list.
/// @param self This widget list.
/// @param index The index.
/// @return The widget.
/// @error @a index is out of bounds.
Machine_GUI_Widget* Machine_GUI_WidgetList_getAt(Machine_GUI_WidgetList* self, size_t index);

/// @brief Append a widget to this widget list.
/// @param self This widget list.
/// @param widget The widget.
void Machine_GUI_WidgetList_append(Machine_GUI_WidgetList* self, Machine_GUI_Widget* widget);

/// @brief Translate the widgets.
/// @param self This list widgets.
/// @param t The translation vector.
static inline void Machine_GUI_WidgetList_translate(Machine_GUI_WidgetList* self, const Machine_Math_Vector2* t) {
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
    Machine_GUI_Widget* widget = (Machine_GUI_Widget*)Machine_GUI_WidgetList_getAt(self, i);
    // @todo Add and utilize Machine_GUI_Widget_translate(Machine_GUI_Widget*, const Machine_Math_Vector2 *).
    const Machine_Math_Vector2* oldPosition = Machine_GUI_Widget_getPosition(widget);
    Machine_Math_Vector2* newPosition = Machine_Math_Vector2_sum(oldPosition, t);
    Machine_GUI_Widget_setPosition(widget, newPosition);
  }
}

/// @brief Center the widgets horizontally around the given x-coordinate.
/// The y-positions of the widgets are not modified.
/// @param self This widget list.
/// @param x The x-coordinate.
static inline void Machine_GUI_WidgetList_centerColumn(Machine_GUI_WidgetList* self, float x) {
  Machine_Math_Vector2* delta = Machine_Math_Vector2_create();
  Machine_Math_Vector2* newPosition = Machine_Math_Vector2_create();
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
    Machine_GUI_Widget* widget = (Machine_GUI_Widget*)Machine_GUI_WidgetList_getAt(self, i);
    const Machine_Math_Rectangle2* bounds = Machine_GUI_Widget_getRectangle(widget);
    float cx = Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getCenter(bounds));
    float dx = x - cx;
    const Machine_Math_Vector2* oldPosition = Machine_GUI_Widget_getPosition(widget);
    Machine_Math_Vector2_set(delta, dx, 0.f);
    Machine_Math_Vector2_add(newPosition, oldPosition, delta);
    Machine_GUI_Widget_setPosition(widget, newPosition);
  }
}

/// @brief Center the widgets vertically around the given y-coordinate.
/// The x-positions of the widgets are not modified.
/// @param self This widget list.
/// @param y The y-coordinate.
static inline void Machine_GUI_WidgetList_centerRow(Machine_GUI_WidgetList* self, float y) {
  Machine_Math_Vector2* delta = Machine_Math_Vector2_create();
  Machine_Math_Vector2* newPosition = Machine_Math_Vector2_create();
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
    Machine_GUI_Widget* widget = (Machine_GUI_Widget*)Machine_GUI_WidgetList_getAt(self, i);
    const Machine_Math_Rectangle2* bounds = Machine_GUI_Widget_getRectangle(widget);
    float cy = Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getCenter(bounds));
    float dy = y - cy;
    const Machine_Math_Vector2* oldPosition = Machine_GUI_Widget_getPosition(widget);
    Machine_Math_Vector2_set(delta, 0.f, dy);
    Machine_Math_Vector2_add(newPosition, oldPosition, delta);
    Machine_GUI_Widget_setPosition(widget, newPosition);
  }
}

/// @brief Layout the widgets according to the specified layout model.
/// @param self This widget list.
/// @param model The layout model.
static inline void Machine_GUI_WidgetList_layout(Machine_GUI_WidgetList* self, float parentWidth, float parentHeight, float canvasWidth, float canvasHeight, Machine_GUI_LayoutModel* model) {
  // Set the size of all elements to the same value: That value is the component-wise maxima vector of the preferred sizes of all elements.
  Machine_Math_Vector2* preferredSize = Machine_Math_Vector2_clone(Machine_GUI_Widget_getPreferredSize(Machine_GUI_WidgetList_getAt(self, 0)));
  for (size_t i = 1, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
    //Machine_GUI_Widget_getHorizontalGrowth();
    //Machine_GUI_Widget_getVerticalGrowth();
    Machine_Math_Vector2_maxima(preferredSize, preferredSize, Machine_GUI_Widget_getPreferredSize(Machine_GUI_WidgetList_getAt(self, i)));
  }
  for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
    Machine_GUI_Widget_setSize(Machine_GUI_WidgetList_getAt(self, i), preferredSize);
  }
  
  switch (Machine_GUI_LayoutModel_getPrimaryDirection(model)) {
  case Machine_GUI_Layout_Direction_Column: {
    float y = 0.f;
    // Basically just lay them out consecutively.
    Machine_Math_Vector2* newPosition = Machine_Math_Vector2_create();
    for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
      Machine_GUI_Widget* widget = Machine_GUI_WidgetList_getAt(self, i);
      const Machine_Math_Vector2* oldPosition = Machine_GUI_Widget_getPosition(widget);
      Machine_Math_Vector2_set(newPosition, Machine_Math_Vector2_getX(oldPosition), y);
      Machine_GUI_Widget_setPosition(widget, newPosition);
      y += Machine_Math_Vector2_getY(Machine_GUI_Widget_getSize(widget)) + Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(model);
    }
    float t = y - Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(model);
    if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_Start) {
    } else if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_Center) {
      Machine_Math_Vector2* v = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(v, 0.f, (parentHeight - t) * 0.5f);
      Machine_GUI_WidgetList_translate(self, v);
    } else if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_End) {
      Machine_Math_Vector2* v = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(v, 0.f, (parentHeight - t));
      Machine_GUI_WidgetList_translate(self, v);
    } else {
      Machine_setStatus(Machine_Status_InvalidArgument);
      Machine_jump();
    }

    // Center around the column.
    Machine_GUI_WidgetList_centerColumn(self, parentWidth * 0.5f);

  } break;
  case Machine_GUI_Layout_Direction_ColumnReverse: {
    float y = parentHeight;
    Machine_Math_Vector2* newPosition = Machine_Math_Vector2_create();
    for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
      Machine_GUI_Widget* widget = Machine_GUI_WidgetList_getAt(self, i);
      y -= Machine_Math_Vector2_getY(Machine_GUI_Widget_getSize(widget));
      const Machine_Math_Vector2* oldPosition = Machine_GUI_Widget_getPosition(widget);
      Machine_Math_Vector2_set(newPosition, Machine_Math_Vector2_getX(oldPosition), y);
      Machine_GUI_Widget_setPosition(widget, newPosition);
      y -= Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(model);
    }

    float t = y + Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(model);
    if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_Start)
    {/**/}
    else if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_Center)
    {
      Machine_Math_Vector2* v = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(v, 0.f, -t * 0.5);
      Machine_GUI_WidgetList_translate(self, v);
    }
    else if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_End)
    {
      Machine_Math_Vector2* v = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(v, 0.f, -t);
      Machine_GUI_WidgetList_translate(self, v);
    }
    else
    {
      Machine_setStatus(Machine_Status_InvalidArgument);
      Machine_jump();
    }

    // Center around the column.
    Machine_GUI_WidgetList_centerColumn(self, parentWidth * 0.5f);

  } break;
  case Machine_GUI_Layout_Direction_Row: {
    float x = 0.f;
      Machine_Math_Vector2* newPosition = Machine_Math_Vector2_create();
      for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
        Machine_GUI_Widget* widget = Machine_GUI_WidgetList_getAt(self, i);
        const Machine_Math_Vector2* oldPosition = Machine_GUI_Widget_getPosition(widget);
        Machine_Math_Vector2_set(newPosition, x, Machine_Math_Vector2_getY(oldPosition));
        Machine_GUI_Widget_setPosition(widget, newPosition);
        x += Machine_Math_Vector2_getX(Machine_GUI_Widget_getSize(widget)) + Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(model);
      }

    float t = x - Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(model);
    if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_Start)
    {}
    else if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_Center)
    {
      Machine_Math_Vector2* v = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(v, (parentWidth - t) * 0.5f, 0.f);
      Machine_GUI_WidgetList_translate(self, v);
    }
    else if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_End)
    {
      Machine_Math_Vector2* v = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(v, (parentWidth - t), 0.f);
      Machine_GUI_WidgetList_translate(self, v);
    }
    else
    {
      Machine_setStatus(Machine_Status_InvalidArgument);
      Machine_jump();
    }

    // Center around the row.
    Machine_GUI_WidgetList_centerRow(self, parentHeight * 0.5f);

  } break;
  case Machine_GUI_Layout_Direction_RowReverse: {
    float x = parentWidth;
    Machine_Math_Vector2* newPosition = Machine_Math_Vector2_create();
    for (size_t i = 0, n = Machine_GUI_WidgetList_getSize(self); i < n; ++i) {
      Machine_GUI_Widget* widget = Machine_GUI_WidgetList_getAt(self, i);
      x -= Machine_Math_Vector2_getX(Machine_GUI_Widget_getSize(widget));
      const Machine_Math_Vector2* oldPosition = Machine_GUI_Widget_getPosition(widget);
      Machine_Math_Vector2_set(newPosition, x, Machine_Math_Vector2_getY(oldPosition));
      Machine_GUI_Widget_setPosition(widget, newPosition);
      x -= Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(model);
    }

    float t = x + Machine_GUI_LayoutModel_getPrimaryInterChildSpacing(model);
    if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_Start)
    {/**/}
    else if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_Center)
    {
      Machine_Math_Vector2* v = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(v, -t * 0.5f, 0.f);
      Machine_GUI_WidgetList_translate(self, v);
    }
    else if (Machine_GUI_LayoutModel_getPrimaryJustification(model) == Machine_GUI_Layout_Justification_End) {
      Machine_Math_Vector2* v = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(v, -t, 0.f);
      Machine_GUI_WidgetList_translate(self, v);
    }
    else {
      Machine_setStatus(Machine_Status_InvalidArgument);
      Machine_jump();
    }

    // Center around the row.
    Machine_GUI_WidgetList_centerRow(self, parentHeight * 0.5f);

  } break;
  default: {
    MACHINE_ASSERT_UNREACHABLE();
  } break;
  };
}

#endif // MACHINE_GUI_WIDGETLIST_H_INCLUDED
