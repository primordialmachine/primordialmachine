#if !defined(DX_PLAYER_DEFAULT_APPLICATION_PRESENTER_H_INCLUDED)
#define DX_PLAYER_DEFAULT_APPLICATION_PRESENTER_H_INCLUDED

#include "dx/application_presenter.h"

#include "dx/val/context.h"
#include "dx/console.h"
#include "dx/engine/console_commands.h"
#include "dx/engine/overlay.h"
#include "dx/font_manager.h"
#include "dx/fps_counter.h"
#include "dx/application.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.default_application_presenter",
                       dx_default_application_presenter,
                       dx_application_presenter);

static inline dx_default_application_presenter* DX_DEFAULT_APPLICATION_PRESENTER(void* p) {
  return (dx_default_application_presenter*)p;
}

struct dx_default_application_presenter {
  dx_application_presenter _parent;
  
  /// @brief
  /// @a true if the application should quit. @a false otherwise.
  /// The initial value is @a false.
  bool quit;
  
  /// @brief
  /// A strong reference to the application object.
  dx_application* application;
  
  /// @brief
  /// A strong reference to the user interface manager (UI manager).
  dx_ui_manager* ui_manager;

  Core_MessageQueue* message_queue;
  /// @brief A pointer to the Command Language interpreter.
  dx_cl_interpreter* cl_interpreter;
  /// @brief A pointer to the console.
  dx_console* console;
  /// @brief A pointer to the overlay.
  dx_overlay* overlay;
  /// @brief A pointer to an FPS counter.
  dx_fps_counter* fps_counter;
  /// @brief A list of scene presenters. Might be empty.
  dx_inline_object_array* scene_presenters;
  /// @brief The index of the scene to be displayed in the list of scenes.
  /// The initial value is @a 0. If the index is out of bounds, then no scene is displayed.
  Core_Size scene_index;
};

static inline dx_default_application_presenter_Dispatch* DX_DEFAULT_APPLICATION_PRESENTER_DISPATCH(void* p) {
  return (dx_default_application_presenter_Dispatch*)p;
}

struct dx_default_application_presenter_Dispatch {
  dx_application_presenter_Dispatch _parent;
};

Core_Result dx_default_application_presenter_construct(dx_default_application_presenter* SELF, dx_application* application, dx_cl_interpreter* cl_interpreter, Core_MessageQueue* message_queue);

Core_Result dx_default_application_presenter_create(dx_default_application_presenter** RETURN, dx_application* application, dx_cl_interpreter* cl_interpreter, Core_MessageQueue* message_queue);

#endif // DX_PLAYER_DEFAULT_APPLICATION_PRESENTER_H_INCLUDED
